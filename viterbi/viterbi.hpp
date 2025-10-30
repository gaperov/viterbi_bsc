#ifndef VITERBI_H
#define VITERBI_H

#include <vector>
#include <string>

using namespace std;

/*
 *  Витерби-кодер и декодер (1/2, K=3), полиномы (5,7)
 *  Реализованы три класса:
 *  1. ViterbiCode   – описание сверточного кода
 *  2. ViterbiEncoder — кодер
 *  3. ViterbiDecoder — декодер
 */

// Класс ViterbiCode — хранит параметры кода и таблицы
class ViterbiCode {
    public:
        int k; // k - число входных бит за такт 
        int n; // n - число выходных бит за такт
        int K; // K - длина ограничения
        int m; // m - память (K-1)
        int S; // S - число состояний
        vector<vector<int>> G; // G - генераторы
        vector<vector<int>> next_state; // next_state - таблица переходов
        vector<vector<int>> out_sym; // out_sym - таблица выходов

        //Конструктор
        ViterbiCode(int k_, int n_, int K_, const vector<vector<int>>& G_);

        // создание кода по списку восьмиричных чисел
        static int parse_octal(const string& s);

        static ViterbiCode from_octal(int k, int n, int K, const vector<string>& oct);

        // функция XOR на четность
        static int parity(int x);

    private:
        // Построение таблиц переходов и выходов (только для количества входов = 1)
        void build_trellis();
 };

// Класс ViterbiEncoder — кодирует последовательность
class ViterbiEncoder {
    private:
        const ViterbiCode& code;
        int state;
    
    public:
        explicit ViterbiEncoder(const ViterbiCode& c);

        vector<int> encode(const vector<int>& info, bool terminate);
};

// Класс ViterbiDecoder — реализует алгоритм Витерби
class ViterbiDecoder {
    private:
        const ViterbiCode& code;

    public:
        explicit ViterbiDecoder(const ViterbiCode& c);

        vector<int> decode(const vector<int>& noisy, bool terminate);
};

#endif