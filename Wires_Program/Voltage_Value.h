#ifndef VOLTAGE_VALUE_H
#define VOLTAGE_VALUE_H

#include <Arduino.h>

class RangedInt {
  int value;
  int minVal;
  int maxVal;

public:
  // Default constructor (required for arrays)
  RangedInt() : value(0), minVal(0), maxVal(0) {}

  // Normal constructor
  RangedInt(int minV, int maxV, int initial = 0) {
    minVal = minV;
    maxVal = maxV;
    set(initial);
  }

  // Set value with clamping
  void set(int v) {
    if (v < minVal) value = minVal;
    else if (v > maxVal) value = maxVal;
    else value = v;
  }

  // Get value
  int get() const {
    return value;
  }

  // Range check
  bool isInRange(int v) const {
    return (v >= minVal && v <= maxVal);
  }

  // Conversion to int
  operator int() const {
    return value;
  }

  // Manual copy operator (because min/max are no longer const)
  RangedInt& operator=(const RangedInt& other) {
    if (this != &other) {
      value = other.value;
      minVal = other.minVal;
      maxVal = other.maxVal;
    }
    return *this;
  }
};

#endif
