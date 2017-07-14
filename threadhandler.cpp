#include<thread>
#include<mutex>

#include"threadhandler.h"
#include"trial.h"
#include"settings.h"

threadhandler::threadhandler(std::vector<unsigned long long> * p_list){
  mpz_init(N);

  primelist = p_list;

  m.lock();
  this->quit = false;
  this->run = false;
  for(unsigned long i = 0; i < sett::MAX_THREAD_COUNT; i++){
    this->threads[i] = std::thread(threadManager, this, i, &threadhandler::threadTrialWithList);
    this->done[i] = true;

  }
  m.unlock();
}


threadhandler::threadhandler(){
  mpz_init(N);

  m.lock();
  this->quit = false;
  this->run = false;
  for(unsigned long i = 0; i < sett::MAX_THREAD_COUNT; i++){

    this->threads[i] = std::thread(threadManager, this, i, &threadhandler::threadTrial);
    this->done[i] = true;

  }

  m.unlock();

}

void threadhandler::threadTrialWithList(mpz_t N_0, mpz_t start, mpz_t end, std::vector<factor> * factors){

  unsigned long long i_start = mpz_get_ui(start);
  unsigned long long i_end = mpz_get_ui(end);

  mpz_t temp;
  mpz_init(temp);

  mpz_t l_N;
  mpz_init_set(l_N, N_0);

  for(unsigned long long i = i_start; i < i_end && mpz_cmp_ui(this->N, i) > 0; i++){

    if(mpz_divisible_ui_p(l_N,(*primelist)[i]) != 0){
      m.lock();
      mpz_set_ui(temp,(*primelist)[i]);
      factor f = temp;
      mpz_divexact(this->N, this->N, temp);

      while(mpz_divisible_p(this->N,temp) != 0){

        mpz_add_ui(f.power, f.power,1);
        mpz_divexact(this->N, this->N, temp);
      }

      factors->push_back(f);
      m.unlock();
    }

  }

  mpz_clear(temp);
  mpz_clear(l_N);
}


void threadhandler::threadTrial(mpz_t N_0, mpz_t start, mpz_t end, std::vector<factor> * factors){

  mpz_t N;
  mpz_init_set(N,N_0);

  mpz_t i;
  mpz_init(i);

  for(mpz_set(i,start); mpz_cmp(i,end) < 0; mpz_add_ui(i,i,2)){

    if(mpz_divisible_p(N,i) != 0){

      factor f = i;
      mpz_divexact(N, N, i);

      while(mpz_divisible_p(N,i) != 0){

        mpz_add_ui(f.power, f.power,1);
        mpz_divexact(N, N, i);

      }

      m.lock();
      factors->push_back(f);
      m.unlock();
    }

  }

  mpz_clear(N);
  mpz_clear(i);

}

/**
 * Manages each thread
 * thr is pointing to the threadhandler which created the manager
 * ID is the thread ID
 * threadhandler::*function is the function that should be used to factor the number
 */
void threadManager(threadhandler * thr,
                   const unsigned int ID,
                   void (threadhandler::*function)(mpz_t, mpz_t, mpz_t, std::vector<factor> *)){

  while(!(thr->quit)){

    if(thr->run && !(thr->done[ID])){

      (thr->*function)(thr->N, thr->limits[ID], thr->limits[ID + 1], &(thr->threadfactors));

      thr->m.lock();
      thr->done[ID] = true;
      thr->m.unlock();

    }

  }
  return;
}

void threadhandler::start(mpz_t N_0){

  m.lock();
  mpz_set(N, N_0);

  for(unsigned int i = 0; i < sett::MAX_THREAD_COUNT; i++){
    //threadfactors.clear();
    done[i] = false;
  }

  run = true;
  m.unlock();

}

void threadhandler::join(){

  for(unsigned long int i = 0; i < sett::MAX_THREAD_COUNT; i++){
    bool waiting = true;

    while(waiting){

      if(done[i] || !run){
        waiting = false;
      }

    }

    m.lock();
    mpz_clear(limits[i]);
    m.unlock();
  }

  m.lock();
  mpz_clear(limits[sett::MAX_THREAD_COUNT]);
  run = false;
  m.unlock();


}

threadhandler::~threadhandler(){
  m.lock();
  quit = true;
  m.unlock();

  mpz_clear(N);
  for(unsigned long i = 0; i < sett::MAX_THREAD_COUNT; i++){
    threads[i].join();
  }

}
