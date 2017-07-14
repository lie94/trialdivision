// Author: Felix Hedenström
// Mail:   fhede@kth.se

// Contains headers for the util.cpp file
// Util contains useful non-algorithm specific functions
// as well as the 'factor'-class which is used for all implemented
// factoring functions

#ifndef F_UTIL
#define F_UTIL
#include<vector>
#include<iostream>
#include<gmpxx.h>



class factor{
public:
  // The base of the factor
  // 2 ^ 3 -  number 2 is the base and
  mpz_t base;
  // The power
  mpz_t power;
  factor(const mpz_t N, const mpz_t p);
  factor(const mpz_t N);
  factor(const factor & f);
  ~factor();
  void resolve(mpz_t & result);
  friend std::ostream& operator<<(std::ostream& os, const factor & f);
  bool operator<(const factor & f);
  factor & operator=(const factor & f);
private:
  factor();
};

/**
 * Displays all factors for N
 * assuming they are contained in
 */
void showFactors(const mpz_t N, std::vector<factor> factors);

/**
 * Tests that all factors in 'factors' mutliply to become
 * N
 */
void testFactors(const mpz_t N, const std::vector<factor> factors);

/**
 * Combines the factors of two vectors and puts the result in the first vector
 * N is divided by any factors in b that does not occur in a
 */
//void combine(std::vector<factor> & a, const std::vector<factor> & b, mpz_t & N);

/**
 * Makes sure there are no relative primes in the vector
 * The larger of the relative primes will be removed
 */
void removeMultiples(std::vector<factor> * a, mpz_t & N);

#endif
