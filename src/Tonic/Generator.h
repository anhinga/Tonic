//
//  Generator.h
//  Tonic
//
//  Created by Morgan Packard on 1/23/13.
//
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__Generator__
#define __Tonic__Generator__

#include "TonicFrames.h"
#include "SynthesisContext.h"

namespace Tonic {

  namespace Tonic_{

    class Generator_{
      
    public:
      
      Generator_();
      virtual ~Generator_();
      
      virtual void tick( TonicFrames& frames, const SynthesisContext context );
      
      bool isStereoOutput(){ return isStereoOutput_; };
      
      void lockMutex();
      void unlockMutex();
      
      // set stereo/mono - changes number of channels in outputFrames_
      // subclasses should call in constructor to determine channel output
      virtual void setIsStereoOutput( bool stereo );
      
    protected:
      
      // override point for defining generator behavior
      // subclasses should implment to fill frames with new data
      virtual void computeSynthesisBlock( const SynthesisContext context ) = 0;
      
      bool            isStereoOutput_;
      TonicFrames     outputFrames_;
      unsigned long   lastFrameIndex_;
      
      // Smart pointer to the last context that this generator has seen
      SynthesisContext context_;
      
    };
    
    inline void Generator_::tick(TonicFrames &frames, const SynthesisContext context ){
      
      // check context to see if we need new frames
      if (context->forceNewOutput || lastFrameIndex_ != context->elapsedFrames){
        context_ = context;
        computeSynthesisBlock(context);
        lastFrameIndex_ = context->elapsedFrames;
      }
    
      // copy synthesis block to frames passed in
      frames.copy(outputFrames_);
      
    }
    
    inline void Generator_::lockMutex(){
      context_->lockMutex();
    }
    
    inline void Generator_::unlockMutex(){
      context_->unlockMutex();
    }
    
    /////////////////////

    class PassThroughGenerator_ : public Tonic_::Generator_{
    public:
      void computeSynthesisBlock( const SynthesisContext context ) {};
    };

  }

  
  class Generator : public TonicSmartPointer<Tonic_::Generator_>{

  public:
    
    inline bool isStereoOutput(){
      return obj->isStereoOutput();
    }
    
    virtual void tick(TonicFrames& frames, const SynthesisContext context){
      obj->tick(frames, context);
    }

  };
  
  template<class GenType>
  class TemplatedGenerator : public Generator{
  protected:
    GenType* gen(){
      return static_cast<GenType*>(obj);
    }
  public:
    TemplatedGenerator(){
      obj = new GenType();
    }
  };
  
  class PassThroughGenerator : public TemplatedGenerator<Tonic_::PassThroughGenerator_>{
  public:
  
  };
  
}

// Each generator should have three flavors of setter -- one that accepts a float, one that accepts a
// ControlGenerator, and one that accepts a Generator. This macro will automatically build those three
// setters

#include "FixedValue.h"

#define createGeneratorSetters(generatorClassName, methodNameInGenerator, methodNameInGenerator_) \
                                                                                        \
                                                                                        \
  generatorClassName& methodNameInGenerator(Generator arg){                             \
    this->gen()->lockMutex();                                                           \
    this->gen()->methodNameInGenerator_(arg);                                           \
    this->gen()->unlockMutex();                                                         \
    return static_cast<generatorClassName&>(*this);                                     \
  }                                                                                     \
                                                                                        \
  generatorClassName& methodNameInGenerator(float arg){                                 \
    return methodNameInGenerator( FixedValue(arg) );                                    \
  }                                                                                     \
                                                                                        \
  generatorClassName& methodNameInGenerator(ControlGenerator arg){                      \
    return methodNameInGenerator(  FixedValue().setValue(arg) );                        \
  }



#endif /* defined(__Tonic__Generator__) */
