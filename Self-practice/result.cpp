#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    while (cin >> s && s != "END") {
        vector<long long> line;
        vector<char> ops;
        stringstream ss(s);
        long long x;
        char op;
        while (ss >> x) {
            line.emplace_back(x);
            ss >> op;
            ops.emplace_back(op);
        }

        int size = line.size();
        vector<vector<long long>> dpMax(size, vector<long long>(size, 0)), dpMin(size, vector<long long>(size, LLONG_MAX));
        for (int i = 0; i < size; i++) dpMin[i][i] = dpMax[i][i] = line[i];

        for (int h = 1; h < size; h++)
            for (int i = 0, j = i + h; i + h < size; i++, j++)
                for (int k = i; k < j; k++) {
                    auto a = dpMax[i][k], b = dpMax[k + 1][j], c = dpMin[i][k], d = dpMin[k + 1][j];
                    if (ops[k] == '+') {
                        dpMax[i][j] = max({dpMax[i][j], a + b});
                        dpMin[i][j] = min({dpMin[i][j], c + d});
                    } else {
                        dpMax[i][j] = max({dpMax[i][j], a * b, c * d});
                        dpMin[i][j] = min({dpMin[i][j], a * b, c * d});
                    }
                }


        cout << dpMin[0][size - 1] << ' ' << dpMax[0][size - 1] << '\n';
    }
}
