#include <bits/stdc++.h>
using namespace std;

int plate(int P[6]) {
    static vector<int> count(1771561, 0), p6{1, 11, 121, 1331, 14641, 161051};

    vector<int> indices(1 << 6);
    int r = 1;
    for (int i = 0; i < 6; i++)
        if (~P[i]) {
            int offset = (P[i] + 1) * p6[i];
            for (int j = 0; j < r; j++) indices[j + r] = indices[j] + offset;
            r <<= 1;
        }

    for (int i = 0; i < r; i++) count[indices[i]]++;
    return count[indices[r - 1]] - 1;
}