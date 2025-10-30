#ifndef BSC_H
#define BSC_H

#include <random>
using namespace std;

// Двоичный симметричный канал (BSC)
vector<int> bsc(float p, mt19937& rng, const vector<int>& coded) ;

#endif