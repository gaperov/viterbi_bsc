#ifndef VITERBI_H
#define VITERBI_H

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <iomanip>
#include <algorithm>
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
        int k; //число входных бит за такт
        int K; //длина ограничения
        int n; //число выходных бит за такт
        int m; //память (K-1)
        int S; //число состояний
        vector<vector<int>> G; //генераторы
        vector<vector<int>> next_state; //таблица переходов
        vector<vector<int>> out_sym; //таблица выходов

        //Конструктор
        ViterbiCode(int k_, int n_, int K_, const vector<vector<int>>& G_)
            : k(k_), n(n_), K(K_), m(K_ - 1), G(G_) {
            S = 1 << (k*m);
            build_trellis();
        }

        // создание кода по списку восьмиричных чисел
        static int parse_octal(const string& s) {
            int v = 0;
            for (char c : s) {
                if (c >= '0' && c <= '7') {
                    v = (v << 3 | c - '0');
                }
            }
            return v;
        }

        static ViterbiCode from_octal(int k, int n, int K, vector<string>& oct) {
            vector<vector<int>> G(k, vector<int> (n));
            int t = 0;
            for (int i = 0; i < k; i++) {
                for (int j = 0; j < n; j++) {
                    G[i][j] = parse_octal(oct[t++]);
                }
            }
            return ViterbiCode(k, n, K, G);
        }

        // функция XOR на четность
        static int parity(int x) {
            int p = 0;
            while(x) {
                p ^= (x & 1);
                x >>= 1;
            }
            return p;
        }

    private:
        // Построение таблиц переходов и выходов (только для количества входов = 1)
        void build_trellis() {
            int U = 1 << k;
            next_state.assign(S, vector<int>(U));
            out_sym.assign(S, vector<int>(U));

            for (int s = 0; s < S; s++) {
                for (int u = 0; u < U; u++) {
                    int out = 0;
                    int next_s = 0;

                    int i = 0; //указатель на номер входа
                    int reg = u | (s << 1);

                    for (int j = 0; j < n; j++) {
                        out ^= (parity(reg & G[i][j]) << j);
                    }
                    next_s |= reg;
                

                    next_state[s][u] = next_s;
                    out_sym[s][u] = out;
                }
            }
        }
 };

// Класс ViterbiEncoder — кодирует последовательность
class ViterbiEncoder {
    private:
        const ViterbiCode& code;
        int state;
    
    public:
        ViterbiEncoder(const ViterbiCode& c) : code(c) {state = 0;}

        vector<int> encode(vector<int>& info, bool terminate) {
            vector<int> out;
            out.reserve((info.size()) * (code.n));

            for (int t = 0; t < info.size(); t++) {
                int u = 0;
                u |= info[t];
    
                int y = code.out_sym[state][u];
                state = (code.next_state[state][u] & 3);

                for (int j = 0; j < code.n; j++) {
                    out.push_back((y >> j) & 1);
                }
            }

            if (terminate && code.m > 0) {
                for (int r = 0; r < code.m; r++) {
                    int y = code.out_sym[state][0];
                    state = code.next_state[state][0] & 3;
                    for (int j = 0; j < code.n; j++) {
                        out.push_back((y >> j) & 1);
                    }
                }
            }
            return out;
        }
};

// Класс ViterbiDecoder — реализует алгоритм Витерби
class ViterbiDecoder {
    private:
        const ViterbiCode& code;

    public:
        ViterbiDecoder(const ViterbiCode& c): code(c) {}

        vector<int> decode(vector<int>& noisy, bool terminate) {
            int T = noisy.size() / code.n;
            int U = 1 << code.k;
            int S = code.S;
            int INF = 1000000;
            
            vector<int> cur(S, INF), nxt(S, INF);
            vector<int> pred(T * S);
            vector<int> inbit(T * S);

            cur[0] = 0;

            for (int t = 0; t < T; t++) {
                fill(nxt.begin(), nxt.end(), INF);

                int sym = 0;
                for (int j = 0; j < code.n; j++) {
                    sym |= noisy[t * code.n + j] << j;
                }
                //cout << ' ' << sym << ' ';

                for (int s = 0; s < S; s++) {
                    if (cur[s] >= INF) continue;

                    for (int u = 0; u < U; u++) {
                        int ns = code.next_state[s][u] & 3;
                        int y = code.out_sym[s][u];

                        int bm = 0;
                        int diff = y ^ sym;
                        for (int j = 0; j < code.n; j++) {
                            bm += (diff >> j) & 1;
                        }

                        int cand = cur[s] + bm;
                        if (cand < nxt[ns]) {
                            nxt[ns] = cand;
                            pred[t * S + ns] = s;
                            inbit[t * S + ns] = u;
                        }
                    }
                }
                cur.swap(nxt);
            }
            int fs = terminate ? 0 : min_element(cur.begin(), cur.end()) - cur.begin();

            vector<int> u_rev;
            u_rev.reserve(T * code.k);
            int s = fs;
            for (int t = T - 1; t >= 0; t--) {
                int u = inbit[t * S + s];
                u_rev.push_back(u & 1);
                s = pred[t * S + s];
            }
            reverse(u_rev.begin(), u_rev.end());

            if (terminate && code.m > 0) {
                int tail = code.m * code.k;
                u_rev.resize(u_rev.size() - tail);
            }

            return u_rev;
        }
};

#endif