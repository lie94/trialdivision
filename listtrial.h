#ifndef F_HASHTRIAL
#define F_HASHTRIAL

#include<gmpxx.h>

#include "util.h"

namespace listtrial{

  extern std::vector<unsigned long long> * p_isprime;
  extern std::vector<factor> factors;

  void initiateListTrial();
  void serialTrial(const mpz_t N);
  void parallelTrial(const mpz_t N);

  unsigned long long binsearch(unsigned long long sqrtN);

  unsigned long long binsearchHelp(unsigned long long target, unsigned long long lower_bound, unsigned long long upper_bound);

}

#endif
