#include "viterbi.h"

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <iomanip>
#include <algorithm>
using namespace std;

int main() {
    vector<string> oct = {"5", "7"};
    ViterbiCode code = ViterbiCode::from_octal(1,2,3, oct);

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 2; j++) {
            cout << ViterbiCode::next_stage[i][j];
        }
    }

    return 0;
}