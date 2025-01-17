//
// Created by odderikf on 2/8/19.
//

#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include "findprime.hpp"

int main(int argc, char* argv[]) {
    unsigned long long start = 1;
    unsigned long long stop = 100000;
    unsigned short threadCount = 4;
    switch (argc) {
        case 1:
            break;
        case 2:
            stop = static_cast<unsigned long long int>(std::stoi(argv[1]));
            break;
        case 4:
            threadCount = static_cast<unsigned short>(std::stoi(argv[3]));
            start = static_cast<unsigned long long int>(std::stoi(argv[1]));
            stop = static_cast<unsigned long long int>(std::stoi(argv[2]));
            break;
        case 3:
            start = static_cast<unsigned long long int>(std::stoll(argv[1]));
            stop = static_cast<unsigned long long int>(std::stoll(argv[2]));
            break;

        default: // > 4
            std::cerr << "Too many arguments" << std::endl;
            std::cerr << "Format: oving1 START STOP THREADS" << std::endl;
            break;
    }

    std::vector<unsigned long long> final;
    if(2 >= start){ // 2 is a special case since i skip evens
        final.emplace_back(2);
        start = 3;
    }
    if(start % 2 == 0) ++start; // so it starts on odd
    std::vector<std::thread> threads;
    std::vector<std::unique_ptr<std::vector<unsigned long long>>> results(threadCount);

    for(unsigned short i = 0; i < threadCount; ++i){
        threads.emplace_back(findPrime, start + 2*i, stop, 2*threadCount, std::ref(results[i]));
    }
    for(auto &i : threads) i.join();

    for(auto &i : results){
        final.insert(std::end(final), std::begin(*i), std::end(*i));
    }
    std::sort(final.begin(), final.end());
    for(auto &i : final){
        std::cout << i << std::endl;
    }

    return 0;
}