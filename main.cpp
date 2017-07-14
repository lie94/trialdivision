// Author: Felix Hedenström
// Mail:   fhede@kth.se

#include<iostream>
#include<string>
#include<ctime>
#include<gmpxx.h>

#include"trial.h"
#include"util.h"
#include"settings.h"
#include "listtrial.h"

#define SHOWSTATS false

int main(int argc, char ** argv){
  if(argc < 2){
    std::cerr << "Not enough arguments" << std::endl;
    return 1;
  }else if(argc >= 3){
    std::cerr << "Too many arguments" << std::endl;
    return 1;
  }
  mpz_t N;
  mpz_init(N);
  if(mpz_set_str(N, argv[1], 0) == -1){
    std::cerr << "Could not read as number " << std::endl;
    return 1;
  }
  //std::cout << "I read the number: " << mpz_get_ui(N) << std::endl;

  //trial::singleThreadTrial(N);
  //trial::naiveMultiThreadTrial(N);
  //hashtrial::singleThreadTrial(N);
  listtrial::serialTrial(N);

  mpz_clear(N);

  return 0;
}
