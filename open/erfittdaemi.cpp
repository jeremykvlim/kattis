#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    cin.ignore();

    vector<vector<pair<int, bool>>> indices(n + 1);
    for (int i = 0; i < m; i++) {
        string s;
        getline(cin, s);

        stringstream ss(s);
        for (int _ = 0; _ < 3; _++) {
            string x;
            getline(ss, x, ',');

            bool NOT = x[0] == '!';
            indices[stoi(NOT ? x.substr(1) : x)].emplace_back(i, NOT);
        }
    }

    vector<int> numers{0, 4, 6, 7}, remaining(m, 3);
    for (int x = 1; x <= n; x++) {
        int t = 0, f = 0;
        for (auto [i, NOT] : indices[x])
            if (~remaining[i]) {
                if (NOT) {
                    t += numers[remaining[i] - 1] - numers[remaining[i]];
                    f += 8 - numers[remaining[i]];
                } else {
                    t += 8 - numers[remaining[i]];
                    f += numers[remaining[i] - 1] - numers[remaining[i]];
                }
            }

        cout << (t >= f ? "SATT\n" : "OSATT\n");
        for (auto [i, NOT] : indices[x])
            if (~remaining[i]) {
                if ((t >= f) != NOT) remaining[i] = -1;
                else remaining[i]--;
            }
    }
}
