#include <bits/stdc++.h>
#include "avian.h"
using namespace std;

vector<string> encode(int C, int K, int N, string X) {
    vector<string> S(K);

    vector<vector<int>> C3_len3{{1, 2, 4}, {8, 16, 32}, {64, 128, 256}, {73, 146, 292}, {83, 166, 287}},
                        C3_len2{{1, 2}, {4, 8}, {16, 32}, {21, 42}, {27, 45}},
                        C2_len3{{1, 2, 4}, {8, 16, 32}, {9, 18, 36}, {10, 20, 35}},
                        C2_len2{{1, 2}, {4, 8}, {5, 10}, {6, 11}};
    for (int pos = 0, len; pos < N; pos += len) {
        len = pos + 3 == N ? 3 : 2;

        auto M = C == 3 ? (len == 2 ? C3_len2 : C3_len3) : (len == 2 ? C2_len2 : C2_len3);

        vector<bool> bits(9);
        for (int i = 0; i < C; i++)
            for (int j = 0; j < len; j++) bits[i * len + j] = X[i * N + j + pos] - '0';

        for (int i = 0; i < K; i++)
            for (int j = 0; j < len; j++) {
                bool b = false;
                for (int k = 0; k < len * C; k++)
                    if ((M[i][j] >> k) & 1) b ^= bits[k];
                S[i] += b + '0';
            }
    }

    return S;
}

string decode(int C, int K, int N, vector<string> Y, vector<int> I) {
    string X(C * N, '0');

    vector<vector<int>> C3_len3{{1, 2, 4}, {8, 16, 32}, {64, 128, 256}, {73, 146, 292}, {83, 166, 287}},
                        C3_len2{{1, 2}, {4, 8}, {16, 32}, {21, 42}, {27, 45}},
                        C2_len3{{1, 2, 4}, {8, 16, 32}, {9, 18, 36}, {10, 20, 35}},
                        C2_len2{{1, 2}, {4, 8}, {5, 10}, {6, 11}};
    for (int pos = 0, len; pos < N; pos += len) {
        len = pos + 3 == N ? 3 : 2;

        auto M = C == 3 ? (len == 2 ? C3_len2 : C3_len3) : (len == 2 ? C2_len2 : C2_len3);

        vector<int> masks(len * C);
        vector<bool> bits_rev(len * C);
        for (int i = 0; i < C; i++)
            for (int j = 0; j < len; j++) {
                masks[i * len + j] = M[I[i]][j];
                bits_rev[i * len + j] = Y[i][j + pos] - '0';
            }

        vector<int> bits(9);
        for (int i = 0; i < len * C; i++)
            for (int j = 0; j < 1 << (len * C); j++) {
                int m = 0;
                bool b = false;
                for (int k = 0; k < len * C; k++)
                    if ((j >> k) & 1) {
                        m ^= masks[k];
                        b ^= bits_rev[k];
                    }

                if (m == (1 << i)) bits[i] = b;
            }

        for (int i = 0; i < C; i++)
            for (int j = 0; j < len; j++) X[i * N + j + pos] = bits[i * len + j] + '0';
    }

    return X;
}
