#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, t;
    cin >> n >> k >> t;

    vector<int> p3(k + 1, 1);
    for (int i = 1; i <= k; i++) p3[i] = p3[i - 1] * 3;

    int t_max = p3[k];
    vector<vector<int>> tritsets(t_max, vector<int>(k));
    vector<int> rev(t_max);
    for (int i = 0; i < t_max; i++) {
        int first_set_trit = -1;
        for (int j = 0, temp = i; j < k && temp; j++) {
            int trit = temp % 3;
            if (first_set_trit == -1 && trit) first_set_trit = trit;

            tritsets[i][j] = trit;
            temp /= 3;
        }

        if (first_set_trit == 1) {
            auto rev_ts = tritsets[i];
            for (int &trit : rev_ts) trit = (3 - trit) % 3;

            int j = 0;
            for (int K = 0; K < k; K++) j += rev_ts[K] * p3[K];
            rev[i] = j;
            rev[j] = i;
        }
    }

    vector<vector<int>> dot(t_max, vector<int>(t_max));
    for (int i = t_max - 1; ~i; i--)
        for (int j = 0; j < t_max; j++) {
            int d = 0;
            for (int l = 0; l < k; l++) d += (tritsets[i][l] * tritsets[j][l]) % 3;

            dot[i][j] = d % 3;
        }

    unordered_map<char, int> ctoi{{'A', 0}, {'E', 1}, {'I', 2}};
    unordered_map<int, char> itoc{{0, '0'}, {1, '+'}, {2, '-'}};

    vector<long long> f(t_max, 0);
    vector<int> ids;
    while (t--) {
        string s;
        cin >> s;
        
        int i = 0;
        for (int K = 0; K < k; K++) i += ctoi[s[K]] * p3[K];
        ids.emplace_back(i);
        cin >> f[i];
    }

    vector<complex<long long>> F(t_max, 0), w{{1, 0}, {0, 1}, {-1, -1}};
    for (int j = 0; j < t_max; j++)
        for (int i : ids) F[j] += f[i] * w[dot[i][j]];

    vector<int> tzs(t_max);
    for (int i = 0; i < t_max; i++) tzs[i] = __builtin_ctzll(abs((!F[i].imag() || F[i].real() == F[i].imag()) ? F[i].real() : F[i].imag()));

    vector<int> configs;
    for (int i = t_max - 1; i; i--)
        while (configs.size() < n) {
            vector<int> total(3, 0), z;
            for (int j = 0; j < t_max; j++) {
                if (!dot[i][j]) z.emplace_back(j);
                total[dot[i][j]] += tzs[j];
            }

            if (total[0] <= total[1]) break;

            configs.emplace_back(i);
            for (int j : z) tzs[j]--;
        }
    configs.resize(n);

    auto combine = [&](int i, int j) {
        if (!i) return j;
        if (!j) return i;

        int l = 0;
        for (int K = 0; K < k; K++) l += (tritsets[i][K] + tritsets[j][K]) % 3 * p3[K];
        return l;
    };

    vector<long long> dp(t_max, -1);
    dp[0] = 0;
    for (int i = 0; i < n; i++) {
        auto temp = dp;
        for (int j = 0; j < t_max; j++) {
            if (!~dp[j]) continue;

            int l = combine(rev[configs[i]], j);
            if (!~temp[l]) temp[l] = dp[j] ^ (1LL << i);
        }
        dp = temp;
    }

    for (auto mask : dp) {
        if (!~mask) continue;

        vector<int> levers(n);
        for (int i = 0; i < n; i++) levers[i] = (mask & (1LL << i)) ? rev[configs[i]] : configs[i];

        vector<long long> freq(t_max, 0);
        freq[0] = 1;
        for (int i : levers) {
            auto temp = freq;
            for (int j = 0; j < t_max; j++) temp[combine(i, j)] += freq[j];
            freq = temp;
        }
        
        if (freq == f) {
            for (int i : levers) {
                for (int trit : tritsets[i]) cout << itoc[trit];
                cout << "\n";
            }
            break;
        }
    }
}
