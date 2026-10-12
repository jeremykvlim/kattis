#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    vector<string> T(26);
    for (auto &t : T) cin >> t;

    long long K;
    int M;
    cin >> K >> M;

    vector<long long> m(M);
    for (auto &mi : m) cin >> mi;

    int p2 = -1;
    vector<vector<long long>> len(51, vector<long long>(26, 0));
    for (int k = 0; k <= 50; k++) {
        if (!k) {
            fill(len[k].begin(), len[k].end(), 1);
            continue;
        }

        for (int c = 0; c < 26; c++) {
            for (char ch : T[c]) {
                len[k][c] += len[k - 1][ch - 'a'];
                if (len[k][c] >= 1e15) {
                    len[k][c] = 1e15;
                    break;
                }
            }
        }

        if (all_of(len[k].begin(), len[k].end(), [&](auto l) { return l == 1e15; })) {
            p2 = k;
            len.resize(p2 + 1);
            break;
        }
    }

    vector<int> next(26);
    for (int c = 0; c < 26; c++) next[c] = T[c][0] - 'a';

    vector<int> jump(26);
    auto steps = max(0LL, K - p2);
    for (int c = 0; c < 26; c++) {
        vector<int> pos(26, -1), path;
        int v = c;
        for (; !~pos[v]; v = next[v]) {
            path.emplace_back(v);
            pos[v] = path.size() - 1;
        }

        if (steps < path.size()) jump[c] = path[steps];
        else {
            int i = pos[v];
            jump[c] = path[i + (steps - i) % (path.size() - i)];
        }
    }

    auto letter = [&](int c1, long long p) {
        auto k = K;
        if (k > p2) {
            c1 = jump[c1];
            k = p2;
        }

        for (; k; k--) {
            for (char ch : T[c1]) {
                int c2 = ch - 'a';
                if (p > len[k - 1][c2]) p -= len[k - 1][c2];
                else {
                    c1 = c2;
                    break;
                }
            }
        }
        return (char) (c1 + 'a');
    };

    if (K >= p2)
        for (auto mi : m) cout << letter(s[0] - 'a', mi) << "\n";
    else {
        vector<long long> pref(s.size() + 1, 0);
        for (int i = 0; i < s.size(); i++) pref[i + 1] = min((long long) 1e15, pref[i] + len[K][s[i] - 'a']);

        for (auto mi : m) {
            int i = lower_bound(pref.begin() + 1, pref.end(), mi) - pref.begin() - 1;
            cout << letter(s[i] - 'a', mi - pref[i]) << "\n";
        }
    }
}