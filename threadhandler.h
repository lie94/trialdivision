#ifndef F_THREADHANDLER
#define F_THREADHANDLER

#include<thread>
#include<gmpxx.h>
#include<mutex>

#include"util.h"
#include"settings.h"

class threadhandler{
  bool run;
  bool quit;
  bool done[sett::MAX_THREAD_COUNT];
  std::thread threads[sett::MAX_THREAD_COUNT];
  std::mutex m;

  std::vector<unsigned long long> * primelist;

  void threadTrialWithList(mpz_t N_0, mpz_t start, mpz_t end, std::vector<factor> * factors);
  void threadTrial(mpz_t N_0, mpz_t start, mpz_t end, std::vector<factor> * factors);

  void * trialVersion(mpz_t, mpz_t, mpz_t, std::vector<factor> *);

public:
  mpz_t N;

  std::vector<factor> threadfactors;
  mpz_t limits[sett::MAX_THREAD_COUNT + 1];

  threadhandler(std::vector<unsigned long long> * p_list);
  threadhandler();
  ~threadhandler();

  void start(mpz_t N);
  void join();
  void init();
  friend void threadManager(threadhandler * thr,
                     const unsigned int ID,
                     void (threadhandler::*function)(mpz_t, mpz_t, mpz_t, std::vector<factor> *));
  //friend void threadManager(threadhandler * thr, const unsigned int thread_ID);
};

void threadManager(threadhandler * thr,
                   const unsigned int ID,
                   void (threadhandler::*function)(mpz_t, mpz_t, mpz_t, std::vector<factor> *));

//void threadManager(threadhandler * thr, const unsigned int thread_ID);

#endif
