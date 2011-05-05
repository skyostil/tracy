/**
 *  Tracer utility functions
 *  Copyright (c) 2011 Nokia
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */

#include "tracer.h"
#include "tracer_internal.h"
#include "tracer_util.h"
#include "tracer_platform.h"
#include <stdarg.h>
#include <stdio.h>

/* Debug output for dictionary routines */
/*#define DICT_DEBUG*/

/*
 *  String utilities
 */

TRbyte* triStringFormat(TRState* state, const TRbyte* format, ...)
{
    TR_ASSERT(state);
    TR_ASSERT(format);
    {
        /* Hardcoding the length is stupid, but on Symbian, there's nothing
         * like vsnprintf which could be used to figure out the length of
         * the output string, so we must guess. 
         */
        va_list args;
        TRint   length = 256;
        TRbyte* str = triMalloc(state, (length + 1) * sizeof(TRbyte));
    
        va_start(args, format);
        vsprintf(str, format, args);
        va_end(args);
    
        return str;
    }
}

TRbyte* triStringDuplicate(TRState* state, const TRbyte* s)
{
    TRuint length = triStringLength(s) + 1;
    TRbyte* s2    = triMalloc(state, length * sizeof(TRbyte));
    TRbyte* s3    = s2;
    
    while (length--)
    {
        *s3++ = *s++;
    }
    return s2;
}

void triStringCopy(TRbyte* dest, const TRbyte* source, TRuint maxLength)
{
    while (maxLength-- > 1 && *source)
    {
        *dest++ = *source++;
    }
    *dest = 0;
}

TRint triStringIndex(const TRbyte* string, const TRbyte* match)
{
    const TRbyte* s = string;
    TR_ASSERT(s);
    TR_ASSERT(match);
    
    while (*s)
    {
        if (triStringsEqual(s, match))
        {
            return s - string;
        }
        s++;
    }
    return -1;
}

/* 
 * Dictionary implementation (AVL tree)
 */
 
TRint triDictCompareKeys(const void* key1, const void* key2)
{
    if (key1 < key2)
        return -1;
    else if (key1 > key2)
        return 1;
    return 0;
}

TRDictNode* triDictCreateNode(TRDict* dict, const void* key, void* value)
{
    TRDictNode* newNode;
    newNode         = triMalloc(dict->state, sizeof(TRDictNode));
    
    if (newNode)
    {
        newNode->key    = key;
        newNode->value  = value;
        newNode->left   = 0;
        newNode->right  = 0;
        newNode->parent = 0;
#if defined(DICT_DEBUG)
        newNode->magic1 = 0xdadacafe;
        newNode->magic2 = 0xfeedface;
#endif
    }
    return newNode;
}

void triDictDestroyNode(TRDict* dict, TRDictNode* node)
{
    triFree(dict->state, node);
}

TRDict* triCreateDict(TRState* state)
{
    TRDict* dict = triMalloc(state, sizeof(TRDict));
    TR_ASSERT(dict);
    dict->state        = state;
    dict->size         = 0;
    dict->root         = 0;
    dict->compareKeys  = triDictCompareKeys;
    dict->createNode   = triDictCreateNode;
    dict->destroyNode  = triDictDestroyNode;
    return dict;
}

TRbool triDictVisitInternal(TRDict* dict, TRDictNode *root, TRDictVisitor visitor, void *arg)
{
    if (root)
    {
        TRDictNode* left  = root->left;
        TRDictNode* right = root->right;
        if (!visitor(dict, root, arg))
        {
            return TR_FALSE;
        }
        if (!triDictVisitInternal(dict, left,  visitor, arg))
        {
            return TR_FALSE;
        }
        if (!triDictVisitInternal(dict, right, visitor, arg))
        {
            return TR_FALSE;
        }
    }
    return TR_TRUE;
}

void triDictVisit(TRDict* dict, TRDictVisitor visitor, void *arg)
{
    TR_ASSERT(dict);
    triDictVisitInternal(dict, dict->root, visitor, arg);
}

TRbool triDictDestroyNodeVisitor(TRDict* dict, TRDictNode* node, void* arg)
{
    TR_UNUSED(arg);
    dict->destroyNode(dict, node);
    return TR_TRUE;
}

void triDestroyDict(TRDict* dict)
{
    if (!dict)
    {
        return;
    }

    triDictVisit(dict, triDictDestroyNodeVisitor, 0);
    dict->root      = 0;
    dict->size      = 0;
    triFree(dict->state, dict);
}


TRbool triDictGetKeysVisitor(TRDict* dict, TRDictNode* node, void* arg)
{
    TRArray* array = (TRArray*)arg;
    TR_UNUSED(dict);
    triArrayAppend(array, (void*)node->key);
    return TR_TRUE;
}

TRbool triDictGetValuesVisitor(TRDict* dict, TRDictNode* node, void* arg)
{
    TRArray* array = (TRArray*)arg;
    TR_UNUSED(dict);
    triArrayAppend(array, (void*)node->value);
    return TR_TRUE;
}

#if defined(DICT_DEBUG)
TRbool triDictVerifyVisitor(TRDict* dict, TRDictNode* node, void* arg)
{
    TR_UNUSED(arg);
    TR_UNUSED(dict);
#if defined(DICT_DEBUG)
    TR_ASSERT(node->magic1 = 0xdadacafe);
    TR_ASSERT(node->magic2 = 0xfeedface);
#endif
    if (node->left)  TR_ASSERT(node->left->parent  == node);
    if (node->right) TR_ASSERT(node->right->parent == node);
    return TR_TRUE;
}

TRbool triDictDumpVisitor(TRDict* dict, TRDictNode* node, void* arg)
{
    TR_UNUSED(dict);
    TR_UNUSED(arg);
    printf("--- %d = %d\n", node->key, node->value);
    return TR_TRUE;
}

void triDictDump(const TRDict* dict)
{
    triDictVisit((TRDict*)dict, triDictDumpVisitor, 0);
}

void triDictVerify(const TRDict* dict)
{
    triDictVisit((TRDict*)dict, triDictVerifyVisitor, 0);
}
#endif /* DICT_DEBUG */

TRint triDictGetNodeDepth(const TRDictNode* node)
{
    TRint ld = 0, rd = 0;
    if (node->left)
    {
        ld = 1 + triDictGetNodeDepth(node->left);
    }
    if (node->right)
    {
        rd = 1 + triDictGetNodeDepth(node->right);
    }
    return TR_MAX(ld, rd);
}

TRint triDictGetNodeBalance(const TRDictNode* node)
{
    TRint ld = 0, rd = 0;
    if (node->left)
    {
        ld = 1 + triDictGetNodeDepth(node->left);
    }
    if (node->right)
    {
        rd = 1 + triDictGetNodeDepth(node->right);
    }
    return ld - rd;
}

void triDictSetLeft(TRDictNode* node, TRDictNode* subNode)
{
    node->left = subNode;
    if (subNode)
    {
        subNode->parent = node;
    }
}

void triDictSetRight(TRDictNode* node, TRDictNode* subNode)
{
    node->right = subNode;
    if (subNode)
    {
        subNode->parent = node;
    }
}

void triDictReplaceChild(TRDict* dict, TRDictNode* parent, TRDictNode* oldNode, TRDictNode* newNode)
{
    TR_ASSERT(oldNode);
    oldNode->parent = 0;

    if (!parent)
    {
        dict->root           = newNode;
        if (newNode)
        {
            newNode->parent  = 0;
        }
    }
    else if (parent->left == oldNode)
    {
        triDictSetLeft(parent, newNode);
    }
    else if (parent->right == oldNode)
    {
        triDictSetRight(parent, newNode);
    }
    else
    {
        TR_ASSERT(!"Dictionary corrupted: Child not found");
    }
}

void triDictRebalance(TRDict* dict, TRDictNode* node)
{
    TRDictNode* parent = node->parent;
    TRDictNode* child;

#if defined(DICT_DEBUG)
    printf("Rebalancing %p: balance now %d\n", node, triDictGetNodeBalance(node));
    triDictVerify(dict);
#endif

    switch (triDictGetNodeBalance(node))
    {
    case 2:
        {
            child = node->left;
#if defined(DICT_DEBUG)
            printf("Rebalancing %p: left balance %d\n", node, triDictGetNodeBalance(child));
#endif
            switch (triDictGetNodeBalance(child))
            {
            case 1:
                {
                    /* Left-left rotation */
                    TRDictNode* subTree = child->right;
                    triDictReplaceChild(dict, node->parent, node, child);
                    triDictSetRight(child, node);
                    triDictSetLeft(node, subTree);
                    break;
                }
            case -1:
                {
                    /* Left-right rotation */
                    TRDictNode* subChild = child->right;
                    TRDictNode* subLeft  = subChild->left;
                    TRDictNode* subRight = subChild->right;
                    triDictReplaceChild(dict, node->parent, node, subChild);
                    triDictSetLeft(subChild, child);
                    triDictSetRight(subChild, node);
                    triDictSetLeft(node, subRight);
                    triDictSetRight(child, subLeft);
                    break;
                }
            case 0:
                {
                    /* Right rotation */
                    triDictReplaceChild(dict, node->parent, node, child);
                    triDictSetLeft(node, child->right);
                    triDictSetRight(child, node);
                    break;
                }
            default:
                TR_ASSERT(!"Dictionary corrupted: Left subtree not balanced");
            }
            break;
        }
    case -2:
        {
            child = node->right;
#if defined(DICT_DEBUG)
            printf("Rebalancing %p: right balance %d\n", node, triDictGetNodeBalance(child));
#endif
            switch (triDictGetNodeBalance(child))
            {
            case -1:
                {
                    /* Right-right rotation */
                    TRDictNode* subTree = child->left;
                    triDictReplaceChild(dict, node->parent, node, child);
                    triDictSetLeft(child, node);
                    triDictSetRight(node, subTree);
                    break;
                }
            case 1:
                {
                    /* Right-left rotation */
                    TRDictNode* subChild = child->left;
                    TRDictNode* subLeft  = subChild->left;
                    TRDictNode* subRight = subChild->right;
                    triDictReplaceChild(dict, node->parent, node, subChild);
                    triDictSetLeft(subChild, node);
                    triDictSetRight(subChild, child);
                    triDictSetLeft(child, subRight);
                    triDictSetRight(node, subLeft);
                    break;
                }
            case 0:
                {
                    /* Left rotation */
                    triDictReplaceChild(dict, node->parent, node, child);
                    triDictSetRight(node, child->left);
                    triDictSetLeft(child, node);
                    break;
                }
            default:
                TR_ASSERT(!"Dictionary corrupted: Right subtree not balanced");
            }
            break;
        }
    case  0:
    case -1:
    case  1:
        break;
    default:
        TR_ASSERT(!"Dictionary corrupted: Node not balanced");
    }
    
    if (parent)
    {
        triDictRebalance(dict, parent);
    }
#if defined(DICT_DEBUG)
    triDictVerify(dict);
#endif
}

void triDictSet(TRDict* dict, const void* key, void* value)
{
    TR_ASSERT(dict);
    {
        TRDictNode* node = dict->root;

#if defined(DICT_DEBUG)
        printf("Setting %d, size %d\n", key, dict->size);
        triDictVerify(dict);
        triDictDump(dict);
#endif
        
        if (!node)
        {
            dict->root = dict->createNode(dict, key, value);
            dict->size++;
            return;
        }
        
        while (1)
        {
            TRint c = dict->compareKeys(node->key, key);
            
            if (c < 0)
            {
                if (!node->left)
                {
                    triDictSetLeft(node, dict->createNode(dict, key, value));
                    triDictRebalance(dict, node);
                    dict->size++;
                    return;
                }
                node = node->left;
            }
            else if (c > 0)
            {
                if (!node->right)
                {
                    triDictSetRight(node, dict->createNode(dict, key, value));
                    triDictRebalance(dict, node);
                    dict->size++;
                    return;
                }
                node = node->right;
            }
            else
            {
                node->value = value;
                return;
            }
        }
    }
}

void* triDictGet(const TRDict* dict, const void* key)
{
    TR_ASSERT(dict);
    {
        const TRDictNode* node = dict->root;
        
#if defined(DICT_DEBUG)
        printf("Getting %d, size %d\n", key, dict->size);
        triDictVerify(dict);
        triDictDump(dict);
#endif

        while (node)
        {
            TRint c = dict->compareKeys(node->key, key);

            if (c < 0)
            {
                node = node->left;
            }
            else if (c > 0)
            {
                node = node->right;
            }
            else
            {
                return node->value;
            }
        }
        return 0;
    }
}

int triDictContains(const TRDict* dict, const void* key)
{
    TR_ASSERT(dict);
    {
        const TRDictNode* node = dict->root;
        
#if defined(DICT_DEBUG)
        printf("Querying %d, size %d\n", key, dict->size);
        triDictVerify(dict);
        triDictDump(dict);
#endif

        while (node)
        {
            TRint c = dict->compareKeys(node->key, key);

            if (c < 0)
            {
                node = node->left;
            }
            else if (c > 0)
            {
                node = node->right;
            }
            else
            {
                return 1;
            }
        }
        return 0;
    }
}

TRbool triDictRemove(TRDict* dict, const void* key)
{
    TR_ASSERT(dict);
    {
        TRDictNode* node = dict->root;

#if defined(DICT_DEBUG)
        printf("Removing %d, size %d\n", key, dict->size);
        triDictVerify(dict);
        triDictDump(dict);
#endif
        
        while (node)
        {
            TRint c = dict->compareKeys(node->key, key);
            if (c < 0)
            {
                node = node->left;
            }
            else if (c > 0)
            {
                node = node->right;
            }
            else
            {
                TRDictNode* parent;
                TRDictNode* child;

                /* 
                 *  The node has both left and right subtrees. Find the largest node in the left
                 *  subtree and replace it with the node to be deleted.
                 */
                if (node->left && node->right)
                {
                    const void*    tmpKey;
                    void*          tmpValue;

                    child = node->left;
                    while (child->right) child = child->right;

                    tmpKey       = child->key;
                    tmpValue     = child->value;
                    child->key   = node->key;
                    child->value = node->value;
                    node->key    = tmpKey;
                    node->value  = tmpValue;
                    node         = child;
                }

                parent = node->parent;
                child  = node->left ? node->left : node->right;

                /*
                 *  If the node has either a left or right subtree, replace the node with that.
                 *  If there is no child node, we can remove the node directly.
                 */
                if (parent)
                {
                    triDictReplaceChild(dict, parent, node, child);
                    triDictRebalance(dict, parent);
                }
                else
                {
                    dict->root = child;
                    if (child)
                    {
                        child->parent = 0;
                    }
                }

                dict->size--;
                dict->destroyNode(dict, node);
                
#if defined(DICT_DEBUG)
                triDictVerify(dict);
#endif
                
                return TR_TRUE;
            }
        }
    }
    return TR_FALSE;
}

TRuint triDictSize(const TRDict* dict)
{
    TR_ASSERT(dict);
    
    return dict->size;
}

TRArray* triDictGetKeys(TRDict* dict)
{
    TR_ASSERT(dict);
    {
        TRArray* array = triCreateArray(dict->state);

        if (!array)
        {
            dict->state->error = TR_OUT_OF_MEMORY;
            return 0;
        }
        triDictVisit(dict, triDictGetKeysVisitor, array);
        return array;
    }
}

TRArray* triDictGetValues(TRDict* dict)
{
    TR_ASSERT(dict);
    {
        TRArray* array = triCreateArray(dict->state);

        if (!array)
        {
            dict->state->error = TR_OUT_OF_MEMORY;
            return 0;
        }
        triDictVisit(dict, triDictGetValuesVisitor, array);
        return array;
    }
}

/*
 * String dictionary implementation
 */

TRint triStrDictCompareKeys(const void* key1, const void* key2)
{
    const TRbyte* s1 = key1;
    const TRbyte* s2 = key2;
    TRbyte b1, b2;
    
    while (*s1 || *s2)
    {
        b1 = *s1++;
        b2 = *s2++;
        if (b1 < b2)
          return -1;
        else if (b1 > b2)
          return 1;
    }
    return 0;
}

TRDictNode* triStrDictCreateNode(TRDict* dict, const void* key, void* value)
{
    TRDictNode* newNode = triDictCreateNode(dict, key, value);
    newNode->key        = triStringDuplicate(dict->state, (const TRbyte*)key);
    return newNode;
}

void triStrDictDestroyNode(TRDict* dict, TRDictNode* node)
{
    triFree(dict->state, (void*)node->key);
    triDictDestroyNode(dict, node);
}

TRStrDict* triCreateStrDict(TRState* state)
{
    TRStrDict* dict    = triCreateDict(state);
    dict->compareKeys  = triStrDictCompareKeys;
    dict->createNode   = triStrDictCreateNode;
    dict->destroyNode  = triStrDictDestroyNode;
    return dict;
}

void triStrDictSet(TRStrDict* dict, const TRbyte* key, void* value)
{
    triDictSet(dict, (void*)key, value);
}

void* triStrDictGet(const TRStrDict* dict, const TRbyte* key)
{
    return triDictGet(dict, (void*)key);
}

TRbool triStrDictRemove(TRStrDict* dict, const TRbyte* key)
{
    return triDictRemove(dict, (const void*)key);
}

/* 
 * Integer dictionary implementation
 */

TRint triIntDictCompareKeys(const void* key1, const void* key2)
{
    TRint k1 = (TRint)key1;
    TRint k2 = (TRint)key2;
    if (k1 < k2)
      return -1;
    else if (k1 > k2)
      return 1;
    return 0;
}

TRIntDict* triCreateIntDict(TRState* state)
{
    TRIntDict* dict    = triCreateDict(state);
    dict->compareKeys  = triIntDictCompareKeys;
    dict->createNode   = triDictCreateNode;
    dict->destroyNode  = triDictDestroyNode;
    return dict;
}

void triIntDictSet(TRIntDict* dict, TRint key, void* value)
{
    triDictSet(dict, (void*)key, value);
}

void* triIntDictGet(const TRIntDict* dict, TRint key)
{
    return triDictGet(dict, (void*)key);
}

int triIntDictContains(const TRDict* dict, const void* key)
{
    TR_ASSERT(dict);
    {
        const TRDictNode* node = dict->root;
        
#if defined(DICT_DEBUG)
        printf("Querying %d, size %d\n", key, dict->size);
        triDictVerify(dict);
        triDictDump(dict);
#endif

        while (node)
        {
            TRint c = dict->compareKeys(node->key, key);

            if (c < 0)
            {
                node = node->left;
            }
            else if (c > 0)
            {
                node = node->right;
            }
            else
            {
                return 1;
            }
        }
        return 0;
    }
}

TRbool triIntDictRemove(TRIntDict* dict, TRint key)
{
    return triDictRemove(dict, (const void*)key);
}

/*
 *  Array implementation
 */
 
TRArray* triCreateArray(TRState* state)
{
    TRArray* array  = triMalloc(state, sizeof(TRArray));
    TR_ASSERT(array);
    array->state    = state;
    array->size     = 0;
    array->values   = 0;
    return array;
}

void triDestroyArray(TRArray* array)
{
    if (!array)
    {
        return;
    }
    
    triFree(array->state, array->values);
    array->values    = 0;
    array->size      = 0;
    triFree(array->state, array);
}

void triArrayAppend(TRArray* array, void* value)
{
    TR_ASSERT(array);
    triArraySet(array, array->size, value);
}

void triArraySet(TRArray* array, TRuint index, void* value)
{
    TR_ASSERT(array);
    
    if (array->size < index + 1)
    {
        TRuint diff = (index + 1) - array->size, i;
        array->size = index + 1;
        array->values = triRealloc(array->state, array->values, sizeof(array->values[0]) * array->size);
        TR_ASSERT_ERROR(array->state, array->values, TR_OUT_OF_MEMORY);

        /* Reset the new values to zero. The last value is set to the new value later */
        for (i = array->size - diff; i < array->size - 1; i++)
        {
           array->values[i] = 0;
        }
    }
    array->values[index] = value;
}

TRbool triArrayRemove(TRArray* array, const void* value)
{
    TRuint i;
    TRbool found = TR_FALSE;
    TR_ASSERT(array);

    for (i = 0; i < array->size; i++)
    {
        if (array->values[i] == value)
        {
            /* Compact the array */
            for (; i < array->size - 1; i++)
            {
                array->values[i] = array->values[i + 1];
            }
            array->size--;
            array->values = triRealloc(array->state, array->values, sizeof(array->values[0]) * array->size);
            i--;
            found = TR_TRUE;
        }
    }
    
    return found;
}

/*
 *  CRC32 implementation
 */

TRuint rotateRight(TRuint n, TRuint bits)
{
    return (n >> bits) | ((n << (sizeof(TRuint) * 8 - bits)));
}

TRuint triCalculateCRC32(TRState* state, TRuint length, const TRbyte* data)
{
    TRuint i, crc = ~0;
    TR_ASSERT(state);
    TR_ASSERT(data);
    
    /* Calculate the CRC table if it doesn't exist yet */
    if (!state->crcTable)
    {
        state->crcTable = triMalloc(state, 256 * sizeof(TRint));
        if (!state->crcTable)
        {
            state->error = TR_OUT_OF_MEMORY;
            return 0;
        }

        /* The following is from the GNU Classpath CRC32 implementation by Per Bothner */
        for (i = 0; i < 256; i++)
        {
            TRint c = i, k;
            for (k = 7; k >= 0; k--)
            {
                if (c & 1)
                    c = 0xedb88320 ^ rotateRight(c, 1);
                else
                    c = rotateRight(c, 1);
            }
            state->crcTable[i] = c;
        }    
    }
    
    /* Calculate the checksum */
    for (i = 0; i < length; i++)
    {
        crc = state->crcTable[(crc ^ data[i]) & 0xff] ^ rotateRight(crc, 8);
    }
    return ~crc;
}
