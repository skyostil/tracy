/**
 * Copyright (c) 2011 Nokia
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// INCLUDE FILES
#include "TracePlayerAppUi.h"
#include "TracePlayerContainer.h" 
#include <TracePlayer.rsg>
#include "traceplayer.hrh"

#include <avkon.hrh>

// ================= MEMBER FUNCTIONS =======================
//
// ----------------------------------------------------------
// CTracePlayerAppUi::ConstructL()
// ----------------------------------------------------------
//
void CTracePlayerAppUi::ConstructL()
    {
    BaseConstructL();
    iAppContainer = new (ELeave) CTracePlayerContainer;
    iAppContainer->SetMopParent(this);
    iAppContainer->ConstructL( ClientRect() );
    AddToStackL( iAppContainer );
    }

// ----------------------------------------------------
// CTracePlayerAppUi::~CTracePlayerAppUi()
// Destructor
// Frees reserved resources
// ----------------------------------------------------
//
CTracePlayerAppUi::~CTracePlayerAppUi()
{
    if ( iAppContainer )
        {
        RemoveFromStack( iAppContainer );
        delete iAppContainer;
        }
}

// ------------------------------------------------------------------------------
// CTracePlayerAppUi::::DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane)
//  This function is called by the EIKON framework just before it displays
//  a menu pane. Its default implementation is empty, and by overriding it,
//  the application can set the state of menu items dynamically according
//  to the state of application data.
// ------------------------------------------------------------------------------
//
void CTracePlayerAppUi::DynInitMenuPaneL(
    TInt /*aResourceId*/,CEikMenuPane* /*aMenuPane*/)
    {
    }

// ----------------------------------------------------
// CTracePlayerAppUi::HandleKeyEventL(
//     const TKeyEvent& aKeyEvent,TEventCode /*aType*/)
// Key event handler
// ----------------------------------------------------
//
TKeyResponse CTracePlayerAppUi::HandleKeyEventL(
    const TKeyEvent& /*aKeyEvent*/ ,TEventCode /*aType*/)
{
    return EKeyWasNotConsumed;
}

// ----------------------------------------------------
// CTracePlayerAppUi::HandleCommandL(TInt aCommand)
// Command handler
// ----------------------------------------------------
//
void CTracePlayerAppUi::HandleCommandL(TInt aCommand)
    {
    switch ( aCommand )
        {
        case EAknSoftkeyBack:
        case EEikCmdExit:
            {
            Exit();
            break;
            }
        default:
            break;      
        }
    }

// End of File  
