//
//  BufferFiller.cpp
//  TonicDemo
//
//  Created by Nick Donaldson on 2/9/13.

//
// See LICENSE.txt for license and usage information.
//


#include "BufferFiller.h"

namespace Tonic {
  
  BufferFiller::BufferFiller() : bufferReadPosition_(0){
    outputFrames_.resize(kSynthesisBlockSize, 2);
    outputGen = PassThroughGenerator();
  }
  
  //! TODO I think this should use a mutex
  void BufferFiller::addControlGenToTick(ControlGenerator gen){
    explicitTicGens.push_back(gen);
  }
  
}