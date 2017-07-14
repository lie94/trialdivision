#include <gmpxx.h>
#include <fstream>
#include <iostream>

#include "listtrial.h"
#include "settings.h"
#include "trial.h"
#include "util.h"
#include "threadhandler.h"

std::vector<unsigned long long> * listtrial::p_isprime;
threadhandler * lt_th;
std::vector<factor> listtrial::factors;

void listtrial::initiateListTrial(){
  std::ifstream f_primelist;
  f_primelist.open("primelist.tmp", std::ios::binary);
  p_isprime = new std::vector<unsigned long long>();

  for(unsigned long long i = 1; i <= sett::PRIME_LIST_SIZE; i++){
    char c;
    f_primelist.get(c);
    if(c == '1'){
      (*p_isprime).push_back(i);
    }
  }

  f_primelist.close();

}
/**
 * Finds the last index of p_isprime that contains a prime lower than sqrt(N);
 */
unsigned long long listtrial::binsearch(unsigned long long sqrtN){
  return binsearchHelp(sqrtN, 0, sqrtN);
}

unsigned long long listtrial::binsearchHelp(unsigned long long target, unsigned long long lower_bound, unsigned long long upper_bound){

  if(upper_bound - lower_bound > 1){
    unsigned long long average = (lower_bound + upper_bound) / 2;
    if((*p_isprime)[average] > target){
      return binsearchHelp(target, lower_bound, average);
    }else{
      return binsearchHelp(target, average, upper_bound);
    }
  }else{
    //std::cout << "isprime[" << lower_bound << "] = " << (*p_isprime)[lower_bound] << std::endl;
    return lower_bound;

  }
}



/**
 * BUGG: Does not work for numbers with root larger than sett::PRIME_LIST_SIZE
 */
void listtrial::parallelTrial(const mpz_t N_0){

  if(p_isprime == nullptr){
    initiateListTrial();
  }

  std::vector<unsigned long long> & isprime = *p_isprime;

  if(lt_th == nullptr){

    lt_th = new threadhandler(p_isprime);

  }else{
    lt_th->threadfactors.clear();
  }

  mpz_t N;
  mpz_init_set(N,N_0);

  if(mpz_cmp_ui(N_0, 0) == 0){
    return;
  }


  mpz_t sqrtN;

  mpz_init(sqrtN);
  mpz_sqrt(sqrtN,N); //OBS might be wrong use

  unsigned long long i_limit;

  if(mpz_cmp_ui(sqrtN, sett::PRIME_LIST_SIZE) >= 0){

    i_limit = sett::PRIME_LIST_SIZE;

  }else{

    i_limit = binsearch(mpz_get_ui(sqrtN));

  }

  mpz_clear(sqrtN);

  mpz_init_set_ui(lt_th->limits[0], 0);

  unsigned long long i_delta = i_limit / sett::MAX_THREAD_COUNT;
  if(i_delta < 1){
    i_delta = 2;
  }

  for(unsigned int i = 0; i < sett::MAX_THREAD_COUNT; i++){

    mpz_init(lt_th->limits[i + 1]);
    mpz_add_ui(lt_th->limits[i + 1], lt_th->limits[i], i_delta);
    //std::cout << "limits: " << mpz_get_ui(lt_th->limits[i + 1]) << std::endl;
  }

  lt_th->start(N);

  lt_th->join();

  if(mpz_cmp_ui(lt_th->N,1) != 0){

    lt_th->threadfactors.push_back(factor(lt_th->N));

  }

  if(sett::PRINT_ANSWER){

    showFactors(N_0,lt_th->threadfactors);

  }

  if(sett::INTERNAL_TEST){
    testFactors(N_0,lt_th->threadfactors);
  }

  mpz_clear(N);
  return;
}

void listtrial::serialTrial(const mpz_t N_0){

  if(p_isprime == nullptr){
    initiateListTrial();
  }

  std::vector<unsigned long long> & isprime = *p_isprime;

  factors.clear();
  if(mpz_cmp_ui(N_0, 0) == 0){
    return;
  }
  mpz_t N, limit, temp;//,i;
  mpz_init_set(N,N_0);

  //trial::removeFactor2(N,factors);

  mpz_init(limit);
  mpz_sqrt(limit,N);

  unsigned long long i_limit;

  if(mpz_cmp_ui(limit, sett::PRIME_LIST_SIZE) >= 0){
    i_limit = sett::PRIME_LIST_SIZE;
  }else{
    i_limit = mpz_get_ui(limit);
  }

  mpz_init(temp);
  for(unsigned long long divisor : isprime){

    if(divisor > i_limit){
      break;
    }

    if(mpz_divisible_ui_p(N,divisor) != 0){

      mpz_set_ui(temp, divisor);
      factor f = temp;
      mpz_divexact(N, N, temp);

      while(mpz_divisible_p(N,temp) != 0){

        mpz_add_ui(f.power, f.power,1);
        mpz_divexact(N, N, temp);

      }

      factors.push_back(f);

      if(mpz_cmp_ui(N, 1) == 0){
        break;
      }

      mpz_sqrt(limit,N);

      if(mpz_cmp_ui(limit, sett::PRIME_LIST_SIZE) >= 0){
        i_limit = sett::PRIME_LIST_SIZE;
      }else{
        i_limit = mpz_get_ui(limit);
      }

    }
  }

  mpz_clear(temp);

  if(mpz_cmp_ui(limit, sett::PRIME_LIST_SIZE) >= 0){
    mpz_t start;
    mpz_init_set_ui(start, i_limit);
    trial::spanTrial(N, start, limit, &factors);
  }

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

  return;
}
