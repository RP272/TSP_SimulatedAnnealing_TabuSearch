#ifndef RANDOMINTEGERGENERATOR_H
#define RANDOMINTEGERGENERATOR_H
#include <random>

class RandomIntegerGenerator
{
private:
    std::random_device rd;
    std::mt19937 rng;
    std::uniform_int_distribution<int> uni;
public:
    RandomIntegerGenerator(int min, int max);
    int generate();
};

#endif