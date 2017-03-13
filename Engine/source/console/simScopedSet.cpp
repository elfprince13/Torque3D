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

#include "simScopedSet.h"

#include <algorithm>
#include "assets/assetBase.h"

SimScopedSet::const_iterator SimScopedSet::findByName(StringTableEntry name) const {
   return std::find_if(begin(), end(), [&name](SimObject *o){
      return o->getName() == name;
   });
}


SimScopedSet::iterator SimScopedSet::findByName(StringTableEntry name) {
   return std::find_if(begin(), end(), [&name](SimObject *o){
      return o->getName() == name;
   });
}


SimObject * SimScopedSet::lookup(StringTableEntry name, SimScopedSet *&here) {
   const_iterator found = findByName(name);
   SimObject * ret;
   if(found == end()){
      if(mParent == nullptr){
         here = nullptr;
         ret = nullptr;
      } else {
         ret = mParent->lookup(name, here);
      }
   } else {
      here = this;
      ret = *found;
   }
   return ret;
}

void SimScopedSet::addObject(SimObject *o) {
   // This is going to be tricky because
   SimScopedSet *here;
   StringTableEntry oName = o->getName();
   StringTableEntry sName = this->getName();
   SimObject *exists = lookup(oName, here);
   switch (mLinkage) {
      case Const: if(exists != nullptr) {
         Con::errorf("Object with name %s already exists in scope of %s.", oName, sName);
         break;
      }
      case Shadow: {
         if(here == this) {
            Con::errorf("Object with name %s already exists in %s.", oName, sName);
            break;
         }
         SimSet::addObject(o);
      } break;
      case Capture: {
         if (exists == nullptr || (here == this)) {
            SimSet::addObject(o);
         } else if (exists != nullptr && here != nullptr) {
            here->addObject(o);
         } else {
            AssertFatal(false, "Inconsistent scoping state");
         }
      } break;
      case CopyOnWrite: {
         if(exists == nullptr) {
            SimSet::addObject(o);
         } else if(here == this){
            // TODO: The case where we implicitly copied something and are overriding it now.
            Con::errorf("Object with name %s already exists in %s.", oName, sName);
         } else {
            AssetBase *asset = dynamic_cast<AssetBase*>(o);
            AssetBase *oldAsset = dynamic_cast<AssetBase*>(exists);
            if(asset == nullptr){
               // This is a plain old SimObject so we shadow like usual
               SimSet::addObject(o);
            } else if (oldAsset != nullptr && asset->getAssetType() == oldAsset->getAssetType()) {
               // We need to be able to access the owning manager to enumerate dependent assets and update them.
               Con::errorf("We don't handle this case yet");
            } else {
               Con::errorf("Type mismatch overriding assets with CopyOnWrite linkage");
            }
         }
      } break;
      default:
         AssertFatal(false, "Unknown Linkage to parent scope");
   }

}
