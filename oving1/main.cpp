#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <algorithm>
#include <functional>

void findPrime( // fairly naìve iterative method. For every number, check divisibility for all numbers below square root.
        unsigned long long start,
        unsigned long long stop,
        std::unique_ptr<std::vector<unsigned long long>>& results
        ){
    results = std::make_unique<std::vector<unsigned long long>>();
    for(unsigned long long cur_num = start; cur_num < stop; ++cur_num){
        for(auto i = static_cast<unsigned long long int>(std::sqrt(cur_num)); i > 1; --i){
            if(cur_num % i == 0){
                goto outer;
            }
        }
        results->push_back(cur_num);
        outer:;
    }
}

int main(int argc, char* argv[]) {
    unsigned long long start = 2;
    unsigned long long stop = 10000000;
    unsigned short threadCount = 4;
    switch (argc) {
        case 1:
            break;
        case 2:
            stop = std::stoi(argv[1]);
            break;
        case 4:
            threadCount = std::stoi(argv[3]);
            start = std::stoi(argv[1]);
            stop = std::stoi(argv[2]);
            break;
        case 3:
            start = std::stoi(argv[1]);
            stop = std::stoi(argv[2]);
            break;

        default: // > 4
            std::cerr << "Too many arguments" << std::endl;
            std::cerr << "Format: oving1 START STOP THREADS" << std::endl;
            break;
    }

    std::vector<std::thread> threads;
    std::vector<std::unique_ptr<std::vector<unsigned long long>>> results(threadCount);

    unsigned long remaining = stop-start;
    double frac = 2./3.;

    for(unsigned short i = 0; i < std::size(results)-1; ++i){
        unsigned long long substart = stop - remaining;
        auto substop = static_cast<unsigned long long int>(substart + frac * remaining);
        remaining -= static_cast<unsigned long long int>(frac*remaining);
        //std::cout << substart << " " << substop << std::endl;
        threads.emplace_back(findPrime, substart, substop, std::ref(results[i]));
    }

    //std::cout << stop-remaining << " " << stop << std::endl;
    threads.emplace_back(findPrime, stop - remaining, stop, std::ref(results[threadCount-1]));
    for(auto &i : threads) i.join();

    std::vector<unsigned long long> final;
    for(auto &i : results){
        final.insert(std::end(final), std::begin(*i), std::end(*i));
    }
    std::sort(final.begin(), final.end());
    for(auto &i : final){
        //std::cout << i << std::endl;
    }

    return 0;
}