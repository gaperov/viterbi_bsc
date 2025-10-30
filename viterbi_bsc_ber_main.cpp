#include "viterbi/viterbi.hpp"
#include "BSC/bsc.hpp"
#include "BER/ber.hpp"

#include <iostream>
#include <vector>
#include <random>
#include <bitset>
#include <fstream>

using namespace std;

vector<double> linspace(double start, double finish, int num) {
    vector<double> res;
    if (num == 0) {
        return res;
    } else if (num == 1) {
        res.push_back(start);
    } else {
        double step = (finish - start) / (num - 1);
        for (int i = 0; i < num; i++) {
            res.push_back(start + step * i);
        }
    }
    return res;
}

int main() {
    vector<string> oct = {"5", "7"};
    ViterbiCode code = ViterbiCode::from_octal(1,2,3, oct);

    // Исходная информация
    vector<int> info = {1, 0, 0, 1, 1, 0 , 1, 1};
    bool terminate = true;

    // Кодирование информации
    ViterbiEncoder encoder(code);
    vector<int> coded = encoder.encode(info, terminate);

    // Подключаем random
    mt19937 rng(4321);

    // Декодирование информации
    ViterbiDecoder decoder(code);

    // ---------------------------------------------------
    // Построение csv-таблицы от множества вероятностей pp
    // ---------------------------------------------------

    // Создаем CSV-файл
    ofstream file("results/ber.csv");
    file << "Probability,ber\n";

    // Создаем вектор вероятностей и и для каждой вероятности рассчитываем BER
    vector<double> pp = linspace(0, 1, 100);
    for (int i = 0; i < (int)pp.size(); i++) {
        vector<int> noisy_pp = bsc(pp[i], rng, coded);
        vector<int> decoded_pp = decoder.decode(noisy_pp, terminate);
        int err_pp = bit_err_rate(info, decoded_pp);
        double ber_pp = (double)err_pp/info.size();
        file << pp[i] << ',' << ber_pp << '\n';
    }
    return 0;
}