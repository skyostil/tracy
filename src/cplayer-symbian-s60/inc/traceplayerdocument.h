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
#ifndef TRACEPLAYERDOCUMENT_H
#define TRACEPLAYERDOCUMENT_H

// INCLUDES
#include <akndoc.h>
   
// FORWARD DECLARATIONS
class  CEikAppUi;

// CLASS DECLARATION

/**
 * Document class that is just used as the container for the application
 * (as required by the Symbian UI application architecture).
 */
class CTracePlayerDocument : public CAknDocument
    {
    public: // Constructors and destructor

        /**
         * Factory method for creating a new CTracePlayerDocument object.
         */
        static CTracePlayerDocument* NewL(CEikApplication& aApp);

        /**
         * Destructor. Does nothing.
         */
        virtual ~CTracePlayerDocument();

    private:  // Functions from base classes

        /**
         * C++ constructor. Just passes the given application reference to the baseclass.
         */
        CTracePlayerDocument(CEikApplication& aApp);

        /**
         * Second phase constructor. Does nothing.
         */
        void ConstructL();

    private: // New functions

        /**
         * From CEikDocument, creates and returns CTracePlayerAppUi application UI object.
         */
        CEikAppUi* CreateAppUiL();
    };

#endif

// End of File

