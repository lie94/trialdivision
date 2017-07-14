// Author: Felix Hedenström
// Mail:   fhede@kth.se

#include <iostream>
#include <ctime>
#include <string>
#include <chrono>
#include <gmpxx.h>
#include <iomanip>

#include "trial.h"
#include "listtrial.h"
#include "settings.h"

typedef void (* Algorithm)(mpz_t N);

void printInstructions(){
  std::cerr << std::endl;
  std::cerr << "  ./test [F] [start] [end] [reps] [datapoints]" << std::endl;
  std::cerr << std::endl;
  std::cerr << "  F - Factorization algorithms:" << std::endl;
  std::cerr << "    'ST'  - Serial trial division with optimizations" << std::endl;
  std::cerr << "    'LST' - Serial trial division using a list of primes " << std::endl;
  std::cerr << "    'PT' - Parallel trial division" << std::endl;
  std::cerr << "    'LPT'  - Parallel trial division using a list of primes" << std::endl;
  std::cerr << std::endl;
  std::cerr << "  start - start factoring from this number" << std::endl;
  std::cerr << std::endl;
  std::cerr << "  end - end factoring when this number has been factored" << std::endl;
  std::cerr << std::endl;
  std::cerr << "  reps - factor each number this number of times" << std::endl;
  std::cerr << std::endl;
  std::cerr << "  datapoints - number of times elapsed time will be printed" << std::endl;

}

void testAlgorithm(const char * algorithm_name, const mpz_t start, const mpz_t end, const unsigned long long reps, const unsigned long long nr_of_updates, Algorithm a){
  mpz_t j;
  mpz_t temp;
  mpz_t update; // = ((end - start) + 1) / NR_OF_UPDATES;

  mpz_init(temp);
  mpz_init(update);

  mpz_sub(temp, end, start);
  //mpz_add_ui(temp, temp, 1);
  if(nr_of_updates != 0)
    mpz_tdiv_q_ui(update, temp, nr_of_updates);

  double elapsed_time = 0;

  unsigned long long count = 1;
  mpz_t limit, value;
  mpz_init(limit);
  mpz_init_set(value, start);
  mpz_sub(limit, end, start);

  std::cout << "#Start: " << mpz_get_ui(start) << std::endl;
  std::cout << "#End: " << mpz_get_ui(end) << std::endl;
  std::cout << "#Reps: " << reps << std::endl;
  std::cout << "#Update frequency: " << mpz_get_ui(update) << std::endl;

  std::cerr << algorithm_name << " started" << std::endl;

  for(mpz_init_set_ui(j,0); mpz_cmp(j,limit) <= 0; mpz_add_ui(j,j,1)){//j <= end; j++){

    // Prints an update NR_OF_UPDATES times with equal spacing

    if(nr_of_updates != 0 && (count > 1 || mpz_cmp_ui(j,0)) != 0 && mpz_divisible_p(j,update) != 0){
      std::cout << mpz_get_ui(j) << " " << (elapsed_time / reps) << std::endl;
      //std::cerr << algorithm_name << ": " << mpz_get_ui(j) << "/" << mpz_get_ui(limit) << std::endl;
      std::cerr << algorithm_name << ": " << count << "/" << nr_of_updates << " - " << elapsed_time <<std::endl;
      count++;
      flush(std::cout);
      flush(std::cerr);
      elapsed_time = 0;
      //std::cerr << "|" << std::setw(20) << 100 * (update_counter / double(nr_of_updates)) << "% | " << elapsed_time << " \tms |" <<std::endl;
    }

    std::chrono::time_point<std::chrono::system_clock> t_start, t_end;
    // Runs the algorithm once un-timed to make sure the cache is more similar every run

    a(value);

    t_start = std::chrono::system_clock::now();
    for(unsigned long long i = 0; i < reps; i++){//i++){
      a(value);
    }
    t_end = std::chrono::system_clock::now();

    mpz_add_ui(value, value, 1);

    std::chrono::duration<double> rep_time = t_end - t_start;
    elapsed_time += (rep_time).count() * 1000;
  }

  //trial::killthreadHandler();
  // Total time
  // Note that this is the only thing written to std::cout
  //std::cout << mpz_get_ui(end) << " " << elapsed_time << std::endl;

  std::cerr << algorithm_name << " done" << std::endl;
  flush(std::cerr);

  mpz_clear(j);
  mpz_clear(limit);
  mpz_clear(temp);
  mpz_clear(value);
  mpz_clear(update);

}

int main(int argc, char ** argv){
  if(argc != 6){
    std::cerr << std::endl;
    std::cerr << "  Needs 5 arguments" << std::endl;
    printInstructions();
    return 1;
  }


  mpz_t start, end;
  const unsigned long long nr_of_updates = std::stol(argv[5]);
  const unsigned long long reps = std::stol(argv[4]);

  mpz_init(start);
  if(mpz_set_str(start, argv[2], 0) == -1){
    std::cerr << "Could not read 'start' number " << std::endl;
    printInstructions();
    mpz_clear(start);
    mpz_clear(end);
    return 1;
  }
  mpz_init(end);

  if(mpz_set_str(end, argv[3], 0) == -1){
    std::cerr << "Could not read 'end' number " << std::endl;
    printInstructions();
    mpz_clear(end);
    mpz_clear(start);
    return 1;
  }


  if( mpz_cmp(start,end) > 0){ //start > end){
    std::cerr << "Start can't be larger than end" << std::endl;
    return 1;
  }

  if(std::string("ST").compare(argv[1]) == 0){
    //std::cerr << "Starting singlethread trialdivision" << std::endl;
    testAlgorithm(argv[1],start,end,reps,nr_of_updates,(Algorithm) (trial::serialTrial));

  }else if(std::string("LST").compare(argv[1]) == 0){
    //std::cerr << "Starting multithreaded trialdivision" << std::endl
    testAlgorithm(argv[1],start,end,reps,nr_of_updates,(Algorithm) (listtrial::serialTrial));
  }else if(std::string("PT").compare(argv[1]) == 0){
    //std::cerr << "Starting naive multithread trialdivision" << std::endl;

    testAlgorithm(argv[1],start,end,reps,nr_of_updates,(Algorithm) (trial::parallelTrial));

  }else if(std::string("LPT").compare(argv[1]) == 0){
    //std::cerr << "Starting naive multithread trialdivision" << std::endl;

    testAlgorithm(argv[1],start,end,reps,nr_of_updates,(Algorithm) (listtrial::parallelTrial));


  }else{

    std::cerr << std::endl;
    std::cerr << "You did not use one of the possible factorization algorithms." << std::endl;
    printInstructions();
    return 1;

  }

  mpz_clear(start);
  mpz_clear(end);

  return 0;
}
