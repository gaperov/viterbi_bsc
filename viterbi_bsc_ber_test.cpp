#include "viterbi/viterbi.hpp"
#include "BSC/bsc.hpp"
#include "BER/ber.hpp"

#include <iostream>
#include <vector>
#include <random>
#include <bitset>
#include <fstream>

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
    for (int i = 0; i < (int)info.size(); i++) {
        cout << info[i];
    }

    // Кодирование информации
    ViterbiEncoder encoder(code);
    vector<int> coded = encoder.encode(info, terminate);

    cout << "\n\nЗакодированное сообщение\n";
    for (int i = 0; i < (int)coded.size(); i++) {
        cout << coded[i];
        if (i%code.n == code.n-1) { cout << ' ';}
    }

    // Прохождение через канал
    mt19937 rng(4321);
    float p = 0.15;
    vector<int> noisy = bsc(p, rng, coded);

    cout << "\n\nСообщение после прохождения канала BSC\n";
    for (int i = 0; i < (int)noisy.size(); i++) {
        cout << noisy[i];
        if (i%code.n == code.n-1) {cout << ' ';}
    }

    // Декодирование информации
    ViterbiDecoder decoder(code);
    vector<int> decoded = decoder.decode(noisy, terminate);

    cout << "\n\nДекодированное сообщение\n";
    for (int i = 0; i < (int)decoded.size(); i++) {
        cout << decoded[i];
    }

    // Bit Error Rate
    int err = bit_err_rate(info, decoded);
    float ber = (float)err/info.size();
    cout << "\n\nBit Error Rate\n";
    cout << "Для p = " << p << '\n';
    cout << "Количество ошибочных бит: " << err << " - " << "BER: " << ber << "\n\n";

    return 0;
}