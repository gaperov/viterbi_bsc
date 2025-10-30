#include "viterbi.h"
#include "bsc.h"

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <bitset>
using namespace std;

int main() {
    vector<string> oct = {"5", "7"};
    ViterbiCode code = ViterbiCode::from_octal(1,2,3, oct);

    // значение перехода - значение выхода в кодере
    cout << "\nЗначение перехода - реверсивное значение выхода в кодере\n";
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 2; j++) {
            cout << std::bitset<3> (code.next_state[i][j]) << " - ";
            cout << std::bitset<2> (code.out_sym[i][j]) << '\n';
        }
    }

    // Исходная информация
    vector<int> info = {1, 0, 0, 1, 1, 0 , 1, 1};
    bool terminate = true;

    cout << "\nИсходная информация\n";
    for (int i = 0; i < info.size(); i++) {
        cout << info[i];
    }

    // Кодирование информации
    ViterbiEncoder encoder(code);
    vector<int> coded = encoder.encode(info, terminate);

    cout << "\n\nЗакодированное сообщение\n";
    for (int i = 0; i < coded.size(); i++) {
        cout << coded[i];
        if (i%2 == 1) { cout << ' ';}
    }

    // Прохождение через канал
    mt19937 rng(15);
    float p = 0.1;
    vector<int> noisy = bsc(p, rng, coded);

    cout << "\n\nСообщение после прохождения канала BSC\n";
    for (int i = 0; i < noisy.size(); i++) {
        cout << noisy[i];
        if (i%2 == 1) {cout << ' ';}
    }

    // Декодирование информации
    ViterbiDecoder decoder(code);
    vector<int> decoded = decoder.decode(noisy, terminate);

    cout << "\n\nДекодированное сообщение\n";
    for (int i = 0; i < decoded.size(); i++) {
        cout << decoded[i];
    }
    return 0;
}