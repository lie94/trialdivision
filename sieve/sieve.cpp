#include<vector>
#include<iostream>
#include<fstream>

#include "../settings.h"


/**
 * Limited by unsigned long long
 */
int main(int argc, char ** argv){
  if(argc != 1){
    std::cerr << "No arguments needed. Change HASH_TABLE_SIZE in 'settings.h' to change the number of primes calculated" << std::endl;
    return 1;
  }

  std::ofstream output;

  const char * name = "primelist.tmp";
  output.open(name, std::ios::binary);

  const unsigned long long N = sett::PRIME_LIST_SIZE;


  std::vector<bool> isprime(N + 1,true);
  isprime[1] = false;
  std::clog << "Started sieving" << std::endl;
  for(unsigned long long i = 2; i <= N; i++){

    if(isprime[i]){
      for(unsigned long long j = 2 * i; j <= N; j += i){
        //std::cout << j << std::endl;
        isprime[j] = false;

      }
    }

  }
  std::clog << "Started writing to " << name  << std::endl;

  for(unsigned long long i = 1; i <= N; i++){
    output << isprime[i];
  }

  std::clog << "Finished writing to " << name << std::endl;
  output.close();
  return 0;
}
