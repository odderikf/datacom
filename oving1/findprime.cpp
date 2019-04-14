#include "findprime.hpp"

void findPrime( // fairly naìve iterative method. For every number, check divisibility for all numbers below square root.
        unsigned long long start,
        unsigned long long stop,
        unsigned long long step,
        std::unique_ptr<std::vector<unsigned long long>>& results
        ){
    results = std::make_unique<std::vector<unsigned long long>>();
    for(unsigned long long cur_num = start; cur_num < stop; cur_num += step){
        bool is_prime = cur_num % 2 == 1;
        auto max = static_cast<unsigned long long int>(std::sqrt(cur_num));
        if (max % 2 == 1) ++max;
        for(unsigned long long i = 3; i <= max; i += 1){
            if(cur_num % i == 0){
                is_prime = false;
                break;
            }
        }
        if (is_prime) results->push_back(cur_num);
    }
}