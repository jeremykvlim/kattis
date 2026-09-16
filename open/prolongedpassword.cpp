#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct JumpTable {
    int lg;
    vector<vector<int>> lift;
    vector<vector<T>> sum;

    JumpTable(long long n, int m, const vector<int> &next, const vector<T> &w) {
        lg = __lg(n) + 1;
        lift.assign(lg, vector<int>(m));
        lift[0] = next;
        sum.assign(lg, vector<T>(m));
        sum[0] = w;

        for (int b = 1; b < lg; b++)
            for (int i = 0; i < m; i++) {
                int j = lift[b - 1][i];
                lift[b][i] = lift[b - 1][j];
                sum[b][i] = sum[b - 1][i] + sum[b - 1][j];
            }
    }

    pair<T, int> jump_up(int v, long long k) const {
        T cost = 0;
        for (int b = 0; b < lg; b++)
            if ((k >> b) & 1) {
                cost += sum[b][v];
                v = lift[b][v];
            }
        return {cost, v};
    }

    pair<T, int> jump_down(int v, T bound) const {
        T cost = 0;
        for (int b = lg - 1; ~b; b--)
            if (lift[b][v] <= bound) {
                cost += sum[b][v];
                v = lift[b][v];
            }
        return {cost, v};
    }
};

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

    JumpTable<int> jt(K + 1, 26, next, vector<int>(26, 0));
    auto letter = [&](int c1, long long k, long long p) {
        if (k > p2) {
            c1 = jt.jump_up(c1, k - p2).second;
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
        for (auto mi : m) cout << letter(s[0] - 'a', K, mi) << "\n";
    else {
        vector<long long> pref(s.size() + 1, 0);
        for (int i = 0; i < s.size(); i++) pref[i + 1] = min((long long) 1e15, pref[i] + len[K][s[i] - 'a']);

        for (auto mi : m) {
            int i = lower_bound(pref.begin() + 1, pref.end(), mi) - pref.begin() - 1;
            cout << letter(s[i] - 'a', K, mi - pref[i]) << "\n";
        }
    }
}
