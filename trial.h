#ifndef F_TRIAL
#define F_TRIAL

#include<vector>
#include<gmpxx.h>

#include"util.h"
#include"settings.h"

namespace trial{

  //void threadTrial(mpz_t N_0, mpz_t start, mpz_t end, std::vector<factor> * factors);
  void spanTrial(mpz_t N_0, mpz_t start, mpz_t end, std::vector<factor> * factors);
  void removeFactor2(mpz_t & N, std::vector<factor> * factors);
  void parallelTrial(const mpz_t & N_0);
  void serialTrial(const mpz_t & N_0);
  void killthreadHandler();
  void createThreadHandler();

}

#endif
