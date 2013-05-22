//
//  SynthesisContext.h
//  TonicLib
//
//  Created by Nick Donaldson on 5/21/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#ifndef __TonicLib__SynthesisContext__
#define __TonicLib__SynthesisContext__

#include "TonicCore.h"

namespace Tonic{
  
  namespace Tonic_{
    
    // Context passed down from root BufferFiller graph object to all sub-generators
    // Synchronizes signal flow in cases when generator output is shared between multiple inputs
    struct SynthesisContext_{
      
      // Number of frames elapsed since program start
      // unsigned long will last 38+ days before overflow at 44.1 kHz
      unsigned long elapsedFrames;
      
      // System time in seconds
      double elapsedTime;
      
      // If true, generators will be forced to compute fresh output
      // TODO: Not fully implmenented yet -- ND 2013/05/20
      bool forceNewOutput;
      
      // Every context has a mutex.
      TONIC_MUTEX_T   mutex;
      
      SynthesisContext_() : elapsedFrames(0), elapsedTime(0), forceNewOutput(true){
        TONIC_MUTEX_INIT(mutex);
      }
      
      ~SynthesisContext_() {
        TONIC_MUTEX_DESTROY(mutex);
      }
      
      void tick() {
        elapsedFrames += kSynthesisBlockSize;
        elapsedTime = (double)elapsedFrames/sampleRate();
        forceNewOutput = false;
      };
      
      void lockMutex(){
        TONIC_MUTEX_LOCK(mutex);
      }
      
      void unlockMutex(){
        TONIC_MUTEX_UNLOCK(mutex);
      }
      
    };
    
  } // namespace Tonic_
    
  class SynthesisContext : public TonicSmartPointer<Tonic_::SynthesisContext_>{
    
  public:
    
    SynthesisContext(){
      obj = new Tonic_::SynthesisContext_();
    }
    
    // allow dereference directly to object for this smart pointer
    Tonic_::SynthesisContext_ * operator->() const{
      return obj;
    }
    
  };

  // Dummy context for in-place usage and initialization;
  static const SynthesisContext DummyContext;

}


#endif /* defined(__TonicLib__SynthesisContext__) */
