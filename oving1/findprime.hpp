//
// Created by odderikf on 2/8/19.
//

#ifndef OVING1_MAIN_HPP
#define OVING1_MAIN_HPP

#include <cmath>
#include <vector>
#include <memory>

void findPrime( // fairly naìve iterative method. For every number, check divisibility for all numbers below square root.
        unsigned long long start,
        unsigned long long stop,
        unsigned long long step,
        std::unique_ptr<std::vector<unsigned long long>>& results
);

#endif //OVING1_MAIN_HPP
