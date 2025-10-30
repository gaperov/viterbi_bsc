#include "bsc.hpp"

vector<int> bsc(float p, mt19937& rng, const vector<int>& coded) {
    bernoulli_distribution change(p);
    vector<int> out(coded.size());
    for (int i = 0; i < (int)coded.size(); i++) {
        bool err = change(rng);

        if (err) {
            out[i] = coded[i] ^ 1;
        } else {
            out[i] = coded[i];
        }
    }
    return out;
}