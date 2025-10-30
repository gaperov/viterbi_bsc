#ifndef BSC_H
#define BSC_H

#include <random>

// Двоичный симметричный канал (BSC)
vector<int> bsc(float p, mt19937& rng, vector<int>& coded) {
    bernoulli_distribution change(p);
    vector<int> out(coded.size());
    for (int i = 0; i < coded.size(); i++) {
        bool err = change(rng);

        if (err) {
            out[i] = coded[i] ^ 1;
        } else {
            out[i] = coded[i];
        }
    }
    return out;
}
#endif