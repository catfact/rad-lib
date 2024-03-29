
// a "weighted orbiter"
// after "A new method for finding cycles by semilinear control"
// D. Dmitrishin, G. Lesaja b , I. Skrinnik, A. Stokolos
// _Physics Letters A_ Dec 2018
//
// after R code by A. Radunskaya
//
// this class implements a 1-D iterated map,
// modified by the addition of an FIR in its feedback path

#ifndef RAD_WORB_H
#define RAD_WORB_H

#include <cstdlib>
#include <iostream>
#include <random>

#include "generator.h"

namespace rad {

class Worb : public Generator
{
  static constexpr int N = 2; // order of FIR

  std::default_random_engine randGen;
  std::uniform_real_distribution<float> randDist;

  int p;        // period
  double g;     // gamma parameter
  double noise; // noise amplitude
  double base;  // base param for logistic
  double x;     // state
  double a[N];  // coefficients

  // history length; limits max period
  // FIXME: it would be smarter to only store [order] history values,
  // and wait to store them as appropriate...
  static constexpr unsigned int bufSize = 32;
  static constexpr unsigned int bufMask = 31;
  double buf[bufSize];
  unsigned int bufIx;

  const double peek(unsigned int delay) const
  {
    delay = delay & bufMask;
    return buf[(bufIx - delay + bufSize) & bufMask];
  }

  void poke(double x)
  {
    bufIx = (bufIx + 1) & bufMask;
    buf[bufIx] = x;
  }

  float rand1()
  {
      return randDist(randGen);
  }

  // map function (noisy logistic)
  double fn(double x)
  {
    float y = ((base - noise) + noise * rand1()) * x * (1 - x);
    return y;
  }

  virtual double next() override
  {
    unsigned int d0 = 0;
    unsigned int d1 = p;
    unsigned int d2 = p - 1;
    unsigned int d3 = 2 * p - 1;

    const double x0 = peek(d0);
    const double x1 = peek(d1);
    const double x2 = peek(d2);
    const double x3 = peek(d3);

    double y = (1.f - g) * (a[0] * fn(x0) + a[1] * fn(x1)) + g * (x2 + x3) / static_cast<double>(N);

    poke(y);

    return y;
  }


public:
  void setPeriod(unsigned int p_)
  {
    p = p_;
    a[0] = (p + 1.0) / (p + 2.0);
    a[1] = 1.0 / (p + 2.0);
  }

  void setGamma(double g_)
  {
    g = g_;
  }


  void setBase(double b)
  {
    base = b;
  }


  void setNoise(double n)
  {
    noise = n;
  }

  void init(double x_, double base_, double noise_)
  {
    base = base_;
    noise = noise_;
    reset(x_);

  }

  void reset(double x_) { 
    double x = x_;
    for (int i = 0; i < bufSize; ++i)
    {
      buf[i] = x;
      x = fn(x);
      bufIx = i;
    }
  }

  void randomize() { 
    for (int i = 0; i < bufSize; ++i)
    {
      buf[i] = rand1();
    }
  }

  double* getBuffer() { return buf; }

  Worb(): randDist(0.f, 1.f) {
      setInterpType(Interpolator::Sine);
  }

};

}
#endif // RAD_WORB_H
