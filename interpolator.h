//
// Created by e on 4/22/20.
//

#ifndef RAD_INTERPOLATOR_H
#define RAD_INTERPOLATOR_H

#include <cmath>
namespace rad {
class Interpolator {
public:
  typedef enum {
    Hold,
    Ramp,
    Cube,
    Sine,
    Slew,
    Smooth,
    Decay,
    NumInterpTypes
  } Type;

protected:
  enum { BufSize = 16, BufMask = 15 };

  // protected:
public:
  float buf[BufSize];
  unsigned int idx;
  double phase;
  double inc;
  float rate;
  float sr;

public:
  Interpolator() : idx(0), phase(0.0), inc(0.0) {
    for (int i = 0; i < BufSize; ++i) {
      buf[i] = 0.f;
    }
  }

  // set the expected update rate in hz
  virtual void setRate(float rate_hz, float sr_hz) {
    rate = rate_hz;
    sr = sr_hz;
    inc = (double)rate / (double)sr;
  }

  // set the expected update rate in hz
  virtual void setSampleRate(float sr_hz) {
    sr = sr_hz;
    setRate(rate, sr);
  }

  // return current value, increment phase
  virtual float update() {
    double z = calc();
    phase += inc;
    while (phase >= 1.) {
      phase -= 1.0;
    }
    return z;
  }

  // reset the phase-update period
  virtual void reset() { phase = 0.0; }

  // calculate the current value
  virtual float calc() const { return peek(0); }

  // push a new input value
  void push(float z) {
    // pre-increment the buffer index
    idx = (idx + 1) & BufMask;
    buf[idx] = z;
    reset();
  }

  // return buffer value, with zero-based offset into the past
  float peek(unsigned int delay) const {
    return buf[(idx + BufSize - delay) & BufMask];
  }

  // copy the realtime mutable state from another interpolator instance to this one
  void copyState(Interpolator *other) {
    this->idx = other->idx;
    this->phase = other->phase;
    this->setRate(other->rate, other->sr);
    for (int i = 0; i < BufSize; ++i) {
      this->buf[i] = other->buf[i];
    }
  }
};

///--------------
//--- subclasses
//-- Hold: no interpolation
class Hold : public Interpolator {
  float calc() const override { return peek(0); }
};

//-- Ramp: constant-time linear ramps
class Ramp : public Interpolator {
  float calc() const override {
    const float a = peek(1);
    const float b = peek(0);
    return a + phase * (b - a);
  }
};

//-- Cube: constant-time cubic sections
class Cube : public Interpolator {
  float calc() const override {
    const float y0 = peek(3);
    const float y1 = peek(2);
    const float y2 = peek(1);
    const float y3 = peek(0);
    const float x = phase;
    return (((0.5 * (y3 - y0) + 1.5 * (y1 - y2)) * x +
             (y0 - 2.5 * y1 + 2. * y2 - 0.5 * y3)) *
                x +
            0.5 * (y2 - y0)) *
               x +
           y1;
  }
};

//-- Sine: constant-time raised cosine sections
class Sine : public Interpolator {
  float calc() const override {
    const float a = peek(1);
    const float b = peek(0);
    const float f = cosf((float)((phase + 1) * M_PI)) * 0.5f + 0.5f;
    return a + (b - a) * f;
  }
};

//-- Slew: constant-rate linear ramp
class Slew : public Interpolator {
  float calc() const override {
    // TODO
    return peek(0);
  }
};

//-- Smooth: constant-rate exponential smoother
class Smooth : public Interpolator {
  float calc() const override {
    // TODO
    return peek(0);
  }
};

//-- Decay: exponential return to zero
class Decay : public Interpolator {
  static float tau2pole(float t, float sr, float ref = -6.9) {
    return exp(ref / (t * sr));
  }

  void setRate(float r, float sr) override { inc = tau2pole(1 / r, sr); }

  float myCalc(const float f) const { return peek(0) * f; }

  float update() override {
    float f = 1.f - phase;
    float z = myCalc(f);
    f *= inc;
    phase = 1.f - f;
    return z;
  }

  // probably won't be called
  float calc() const override { return myCalc(1.f - phase); }

  // reset decay envelope when new input arrives
  /// FIXME: phase should be normalized to same range as other interp types
  /* void reset() override {  */
  /*     phase = 1.0; */
  /* } */
};
}

#endif // RAD_INTERPOLATOR_H
