#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int d, w;
    cin >> d >> w;

    vector<string> die(d);
    for (auto &s : die) cin >> s;

    unordered_set<string> dict;
    while (w--) {
        string s;
        cin >> s;

        sort(s.begin(), s.end());
        dict.emplace(s);
    }

    vector<bool> state(1 << (3 * d), false);
    auto dfs = [&](auto &&self, int d, int i = 0, string s = "") -> bool {
        if (!~d) {
            sort(s.begin(), s.end());
            return state[i] = dict.count(s);
        }

        bool possible = false;
        for (int f = 1; f <= 6; f++) possible |= self(self, d - 1, f + (i << 3), s + die[d][f - 1]);
        return possible;
    };

    if (!dfs(dfs, d - 1)) {
        cout << "impossible";
        exit(0);
    }

    vector<double> E(1 << (3 * d), 0);
    auto pull = [&](auto &&self, int mask, int d, int i = 0) -> void {
        if (!~d) {
            double sum = 0;
            for (int f = 1; f <= 6; f++) sum += E[i + (f << 3 * __lg(mask))];
            E[i] = min(E[i], sum / 6);
            return;
        }

        int next = ~mask & (1 << d) - 1 ? __lg(~mask & (1 << d) - 1) : -1;
        for (int f = 1; f <= 6; f++) self(self, mask, next, i + (f << 3 * d));
    };

    auto push = [&](auto &&self, int mask, int d, int i = 0) -> void {
        if (!~d) {
            for (int f = 1; f <= 6; f++)
                E[i + (f << (3 * __lg(mask)))] = state[i + (f << (3 * __lg(mask)))] ? 0 : min(E[i + (f << (3 * __lg(mask)))], E[i]);
            return;
        }

        int next = ~mask & (1 << d) - 1 ? __lg(~mask & (1 << d) - 1) : -1;
        for (int f = 1; f <= 6; f++) self(self, mask, next, i + (f << 3 * d));
    };

    for (;;) {
        auto temp = E[0] - !state[0];
        for (int m = 1; m < 1 << d; m++) pull(pull, m, m != (1 << d) - 1 ? __lg(m ^ (1 << d) - 1) : -1);
        for (int m = (1 << d) - 1; m; m--) push(push, m, m != (1 << d) - 1 ? __lg(m ^ (1 << d) - 1) : -1);

        if (abs(temp - E[0]) < 1e-9) {
            cout << fixed << setprecision(6) << E[0] + !state[0];
            exit(0);
        }

        for (int i = 0; i < 1 << (3 * d); i++) E[i] += !state[i];
    }
}
