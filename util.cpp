// Author: Felix Hedenström
// Mail:   fhede@kth.se

#include<vector>
#include<iostream>
#include<math.h>
#include<gmpxx.h>

#include"util.h"
#include"settings.h"

/*
 * Most used constuctor - defaults the power to be 1
 */
factor::factor(const mpz_t N){

  mpz_init(base);
  mpz_init(power);
  mpz_set(base,N);
  mpz_set_ui(power,1);

}

/*
 * Copies the vaules stored within 'f'
 */
factor::factor(const factor & f){

  mpz_init(base);
  mpz_init(power);
  mpz_set(base,f.base);
  mpz_set(power,f.power);

}

factor::~factor(){

  mpz_clear(base);
  mpz_clear(power);

}

factor::factor(const mpz_t N, const mpz_t p) {

  mpz_init(base);
  mpz_init(power);
  mpz_set(base,N);
  mpz_set(power,p);

}

factor & factor::operator=(const factor & f){

  mpz_set(base, f.base);
  mpz_set(power, f.power);
  return *this;

}


/*
 * Calculates the real number value of the factor by explicitly resolving the value of
 * base ^ power and returning its value
 */
void factor::resolve(mpz_t & result){
  mpz_set_ui(result,1);
  mpz_t i;
  mpz_init(i);
  for(mpz_set_ui(i,0); mpz_cmp(i, power) < 0; mpz_add_ui(i,i,1)){
    mpz_mul(result, result, base);
  }
  mpz_clear(i);
}

/**
 * Does not support unlimited sized ints
 */
void testFactors(const mpz_t N_0, const std::vector<factor> factors){
  mpz_t testN, temp;
  mpz_init(testN);

  mpz_set_ui(testN, 1);

  mpz_init(temp);
  for(factor f : factors){
    f.resolve(temp);
    mpz_mul(testN, testN,temp);
  }
  mpz_clear(temp);

  if(mpz_cmp(testN,N_0) != 0){
    for(int i = 0; i < 100; i ++   ){
      std::cerr << "!!!!ERRORR!!!! - N = " << mpz_get_ui(N_0) << std::endl;
      std::cerr << "!!!!ERRORR!!!! - testN = " << mpz_get_ui(testN) << std::endl;
    }
    for(factor f : factors){
      std::cerr << f << std::endl;
    }
  }
  mpz_clear(testN);
}


/*
 * Used to print factors with normal syntax
 * Example:
 *  factor f1(2,3);
 *  std::cout << f1 << std::endl;
 * TODO Printing does not support unlimited sized ints yet
 */
std::ostream& operator<<(std::ostream & os, const factor & f){
  if(mpz_cmp_ui(f.power,1) == 0){

    os << mpz_get_ui(f.base);

  }else{

    os << mpz_get_ui(f.base) << " ^ " << mpz_get_ui(f.power);

  }
  return os;
}

bool factor::operator<(const factor & f){
  return mpz_cmp(this->base, f.base) < 0;
}

void removeMultiples(std::vector<factor> * a, mpz_t & N){
  mpz_t temp;
  mpz_init(temp);
  if(a->size() == 0){
    return;
  }
  std::sort(a->begin(), a->end());

  for(unsigned long long i = 0; i < a->size(); i++){
    //std::cout << "Loop nr "<< i << std::endl;
    if(mpz_cmp_ui((*a)[i].base,2) == 0){
      continue;
    }

    (*a)[i].resolve(temp);

    mpz_divexact(N, N, temp);

    for(unsigned long long j = i + 1; j < a->size(); j++){
      //if(gcd(fa.number,fb.number) != 1){

      // mpz_divisible_p(a,b) returning non-zero -> a is divisible by b
      if(mpz_divisible_p((*a)[j].base, (*a)[i].base) != 0){
        a->erase(a->begin() + j);
        //std::cout << "sizea " << (*a).size() << std::endl;
        j--;
      }
    }
  }
  mpz_clear(temp);
}

/*
void combine(std::vector<factor> & a, const std::vector<factor> & b, mpz_t & N){
  int size = a.size();
  // If 'a' is empty, make 'a' a copy of 'b'

  mpz_t temp;
  mpz_init(temp);

  if(size == 0){

    for(factor fb : b){

      a.push_back(fb);
      // Remove all factors found from N

      fb.resolve(temp);
      mpz_divexact(N, N, temp);

    }
    mpz_clear(temp);
    return;
  }

  for(factor fb : b){
    bool commonDivisors = false;
    //Check all factors in a and make sure they have no common divisors
    for(factor fa : a){
      //if(gcd(fa.number,fb.number) != 1){
      int   k = mpz_cmp(fa.base,fb.base);

      flush(std::cout);
      // mpz_divisible_p(a,b) returning non-zero -> a is divisible by b
      if((k > 0 && mpz_divisible_p(fa.base, fb.base) != 0) || (k < 0 && mpz_divisible_p(fb.base, fa.base) != 0)){

        commonDivisors = true;
        break;

      }
    }
    if(!commonDivisors){

      a.push_back(fb);
      fb.resolve(temp);
      mpz_divexact(N, N, temp);

    }
  }

  mpz_clear(temp);
  return;

}
*/

/*
 * Prints all factors contained within 'factors' as well as the number 'N'
 */
void showFactors(const mpz_t N, std::vector<factor> factors){
  std::cout << "Factors of " << mpz_get_ui(N) << std::endl;
  std::cout << "--------------------" << std::endl;

  for(unsigned int i = 0; i < factors.size(); i++){

    std::cout << i + 1 << ":\t" << factors[i] << std::endl;

  }

  std::cout << "####################" << std::endl;
  flush(std::cout);
}
