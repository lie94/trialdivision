// Author: Felix Hedenström
// Mail:   fhede@kth.se

#include<iostream>
#include<math.h>
#include<vector>
#include<thread>
#include<gmpxx.h>

#include"util.h"
#include"trial.h"
#include"settings.h"
#include"threadhandler.h"

threadhandler * th;

std::vector<factor> factors;

void trial::parallelTrial(const mpz_t & N_0){
  if(mpz_cmp_ui(N_0, 0) == 0){
    return;
  }

  if(th == nullptr){

    th = new threadhandler();

  }else{
    th->threadfactors.clear();
  }

  mpz_t N;
  mpz_init_set(N,N_0);

  removeFactor2(N,&(th->threadfactors));

  mpz_t sqrtN,span,threadSearchSpace;

  mpz_init(sqrtN);
  mpz_sqrt(sqrtN,N); //OBS might be wrong use

  mpz_init(span);
  mpz_sub_ui(span, sqrtN, 3);

  mpz_init(threadSearchSpace);
  mpz_tdiv_q_ui(threadSearchSpace, span, sett::MAX_THREAD_COUNT);

  mpz_clear(span);


  if(mpz_cmp_ui(threadSearchSpace,1) <= 0){
    mpz_set_ui(threadSearchSpace, 2);
  }

  //unsigned int thread_count = sett::MAX_THREAD_COUNT;

  mpz_init_set_ui(th->limits[0], 3);

  for(unsigned int i = 0; i < sett::MAX_THREAD_COUNT; i++){

    mpz_init(th->limits[i + 1]);
    mpz_add(th->limits[i + 1], th->limits[i], threadSearchSpace);

  }

  mpz_clear(sqrtN);
  mpz_clear(threadSearchSpace);

  th->start(N);


  th->join();

  //std::cout << th->threadfactors.size() << std::endl;
  /*
  for(factor f : th->threadfactors){
    std::cout << "Pre-remove: " << f << std::endl;
  }
  */

  removeMultiples(&(th->threadfactors), N);
  //combine(factors, th->threadfactors, N);


  if(mpz_cmp_ui(N,1) != 0){

    th->threadfactors.push_back(factor(N));

  }

  if(sett::PRINT_ANSWER){

    showFactors(N_0,th->threadfactors);

  }

  if(sett::INTERNAL_TEST){
    testFactors(N_0,th->threadfactors);
  }
  mpz_clear(N);
  return;
}


void trial::spanTrial(mpz_t N_0, mpz_t start, mpz_t end, std::vector<factor> * factors){

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

      factors->push_back(f);

    }

  }

  mpz_clear(N);
  mpz_clear(i);
}



/**
 * Single thread trial division
 * using optimizations only possible with single thread trialdivision
 * Not working for: 21
 */
void trial::serialTrial(const mpz_t & N_0){
  //std::vector<factor> factors;
  factors.clear();
  if(mpz_cmp_ui(N_0, 0) == 0){
    return;
  }
  mpz_t N, limit,i;
  mpz_init_set(N,N_0);

  removeFactor2(N,&factors);

  mpz_init(limit);
  mpz_sqrt(limit,N);
  // i < N
  // If i is larger than the remainder of division of all factors (N), then
  // must be either
  mpz_init(i);

  for(mpz_set_ui(i,3); mpz_cmp(i,limit) <= 0 && mpz_cmp_ui(N,1) != 0; mpz_add_ui(i,i,2)){

    if(mpz_divisible_p(N,i) != 0){

      factor f = i;
      mpz_divexact(N, N, i);

      while(mpz_divisible_p(N,i) != 0){

        mpz_add_ui(f.power, f.power,1);
        mpz_divexact(N, N, i);

      }

      factors.push_back(f);
      mpz_sqrt(limit,N);
    }

  }
  mpz_clear(i);
  mpz_clear(limit);

  if(mpz_cmp_ui(N,1) != 0){
    factors.push_back(factor(N));
  }
  mpz_clear(N);

  if(sett::INTERNAL_TEST){
    testFactors(N_0,factors);
  }

  if(sett::PRINT_ANSWER){

    showFactors(N_0,factors);

  }

}



void trial::removeFactor2(mpz_t & N, std::vector<factor> * factors){
  mpz_t mpz_factor;
  mpz_init_set_ui(mpz_factor,2);

  if(mpz_divisible_ui_p(N,2) != 0){

    factor f = mpz_factor;
    mpz_divexact_ui(N, N, 2);

    while(mpz_divisible_ui_p(N,2) != 0){

      mpz_add_ui(f.power, f.power,1);
      mpz_divexact_ui(N, N, 2);

    }

    factors->push_back(f);

  }
  mpz_clear(mpz_factor);

}
