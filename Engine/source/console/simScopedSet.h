//-----------------------------------------------------------------------------
// Copyright (c) 2017 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#ifndef ScopedSimSet_hpp
#define ScopedSimSet_hpp

#include "simSet.h"

class SimScopedSet : public SimSet {
public:
   typedef SimSet Parent;
   
   enum ParentLinkage {
      Const,
      Shadow,
      Capture,
      CopyOnWrite
   };
   
private:
   SimScopedSet * const mParent;
   const ParentLinkage mLinkage;
   
   const_iterator findByName(StringTableEntry name) const;
   iterator findByName(StringTableEntry name);
   
public:
   SimScopedSet(SimScopedSet * parent = nullptr, ParentLinkage linkage = Const) : SimSet(), mParent(parent), mLinkage(linkage) {}
   
   // Unfortunately here might be used later to modify something, so we can't const the whole method with duplicating it like findByName
   SimObject * lookup(StringTableEntry name, SimScopedSet *&here);
   
   virtual void addObject(SimObject *o) override;
   
   
};

#endif /* ScopedSimSet_hpp */
