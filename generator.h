//
// Created by e on 10/29/19.
//

#ifndef RAD_GENERATOR_HPP
#define RAD_GENERATOR_HPP

#include "interpolator.h"

namespace rad {

class Generator {

protected:
  // pointer current interpolator
  Interpolator *interp;
  // array of all interpolators
  Interpolator *interps[Interpolator::NumInterpTypes];

  Hold interpHold;
  Ramp interpRamp;
  Cube interpCube;
  Sine interpSine;
  Slew interpSlew;
  Smooth interpSmooth;
  Decay interpDecay;

  //-- generator state
  double phase;
  double inc;
  float rate;
  float sr;
  float interpSpeed;

  //-- arbitrary numerical index of this osc
  int id;

public:
  Generator() {
    interpSpeed = 1.f;
    interps[Interpolator::Hold] = &interpHold;
    interps[Interpolator::Ramp] = &interpRamp;
    interps[Interpolator::Cube] = &interpCube;
    interps[Interpolator::Sine] = &interpSine;
    interps[Interpolator::Slew] = &interpSlew;
    interps[Interpolator::Smooth] = &interpSmooth;
    interps[Interpolator::Decay] = &interpDecay;
  }

  void setInterpType(Interpolator::Type t) {
    Interpolator *newInterp;
    switch (t) {
    case Interpolator::Hold:
      newInterp = &interpHold;
      break;
    case Interpolator::Ramp:
      newInterp = &interpRamp;
      break;
    case Interpolator::Cube:
      newInterp = &interpCube;
      break;
    case Interpolator::Sine:
      newInterp = &interpSine;
      break;
    case Interpolator::Slew:
      newInterp = &interpSlew;
      break;
    case Interpolator::Smooth:
      newInterp = &interpSmooth;
      break;
    case Interpolator::Decay:
      newInterp = &interpDecay;
      break;
    default:
      newInterp = &interpHold;
    }
    newInterp->copyState(interp);
    interp = newInterp;
  }


  void process(int numFrames, float *buf) {
    for (int i = 0; i < numFrames; ++i) {
      phase += inc;
      while (phase > 1.0) {
        update();
        phase -= 1.0;
      }
      buf[i] += interp->update();
    }
  }

  void setRate(float rate_hz, float sr_hz) {
    sr = sr_hz;
    setRate(rate_hz);
  }

  void setInterpSpeed(float r) {
    interpSpeed = r;
    interp->setRate(rate * interpSpeed, sr);
  }

  void setRate(float rate_hz) {
    rate = rate_hz;
    inc = rate / sr;
    interp->setRate(rate_hz * interpSpeed, sr);
  }
private:
  virtual void update() = 0;

};


}

#endif // RAD_GENERATOR_HPP
