//
// Created by e on 5/26/20.
//

#ifndef RAD_CUBIC_H
#define RAD_CUBIC_H

#include "generator.h"

namespace rad {
class Cubic : public Generator {
  double a;
  double x;

private:
  virtual double next() override {
    x = a * x * x * x + (1.0 - a) * x;
    return x;
  }

public:
  void setParam(double a_) { a = a_;}
  void setState(double x_) { x = x_;}
};
}

#endif // RAD_CUBIC_H
