#include <cassert>
#include <fstream>
#include <iostream>
#include "findprime.hpp"

int main(){
    std::vector<unsigned long long> primes =
        {3, 5, 7, 11 , 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109};
    std::unique_ptr<std::vector<unsigned long long>> foundprimes;
    findPrime(3, 110, 1, foundprimes);
    for(auto i = primes.begin(), j = foundprimes->begin(); i < primes.end(); i++, j++){
        assert(*i == *j);
    }
}