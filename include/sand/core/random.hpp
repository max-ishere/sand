#pragma once
#include <random>

class StandardRandom {
public:
  auto operator()() { return rand(); }
};

typedef StandardRandom FastRandom;

class SecureRandom {
public:
  auto operator()() {
#warning "Secure random is compiled with C rand()!"
    return rand();
  }
};
