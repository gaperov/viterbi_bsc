#include "viterbi.h"

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <iomanip>
#include <algorithm>
#include <bitset>
using namespace std;

int main() {
    vector<string> oct = {"5", "7"};
    ViterbiCode code = ViterbiCode::from_octal(1,2,3, oct);

    // значение перехода - значение выхода в кодере
    cout << "значение перехода - реверсивное значение выхода в кодере\n";
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 2; j++) {
            cout << std::bitset<3> (code.next_state[i][j]) << " - ";
            cout << std::bitset<2> (code.out_sym[i][j]) << '\n';
        }
    }

    // Исходная информация
    vector<int> info = {1, 0, 0, 1, 1, 0 , 1};
    bool terminate = true;

    cout << "Полученная информация\n";
    for (int i = 0; i < info.size(); i++) {
        cout << info[i];
    }

    ViterbiEncoder encoder(code);
    vector<int> coded = encoder.encode(info, terminate);

    cout << "\nЗакодированное сообщение\n";
    for (int i = 0; i < coded.size(); i++) {
        cout << coded[i];
        if (i%2 == 1) { cout << ' ';}
    }
    cout << '\n';
    ViterbiDecoder decoder(code);
    vector<int> decoded = decoder.decode(coded, terminate);

    cout << "\nДекодированное сообщение\n";
    for (int i = 0; i < decoded.size(); i++) {
        cout << decoded[i];
    }
    return 0;
}