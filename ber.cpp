#include "ber.hpp"

int bit_err_rate(const vector<int>& info, const vector<int>& decoded) {
    int err = 0;
    for (int i = 0; i < (int)info.size(); i++) {
        if (info[i] != decoded[i]) {
            err++;
        }
    }
    return err;
}