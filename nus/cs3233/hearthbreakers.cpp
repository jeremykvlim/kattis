#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> S(n);
    auto sum = 0LL;
    for (int &si : S) {
        cin >> si;

        sum += si;
    }
    sort(S.begin(), S.end());

    vector<int> distinct, freq;
    for (int i = 0, j; i < n; i = j) {
        j = i + 1;
        while (j < n && S[j] == S[i]) j++;
        distinct.emplace_back(S[i]);
        freq.emplace_back(j - i);
    }
    int s = distinct.size();

    S.resize(s);
    vector<int> f(s);
    for (int i = 0; i < s; i++) {
        S[i] = distinct[s - 1 - i];
        f[i] = freq[s - 1 - i];
    }

    if (s == 1 && S[0] == 1) {
        cout << n;
        exit(0);
    }

    auto valid = [&](long long m) {
        int msb = 63 - countl_zero((unsigned long long) m);
        if (s > 2 * (msb + 1)) return false;

        vector<pair<long long, long long>> rocks{{m, 1}};
        for (int i = 0, j = 0; i < s; i++) {
            long long need = f[i];
            while (need) {
                if (j >= rocks.size()) return false;

                auto &[size, count] = rocks[j];
                if (size < S[i]) return false;
                if (size == S[i]) {
                    auto used = min(count, need);
                    count -= used;
                    need -= used;
                    if (!count) j++;
                } else {
                    auto temp = count;
                    count = 0;
                    if (size == 1) return false;
                    if (!(size & 1)) {
                        auto half = size / 2, c = temp * 2;

                        if (!rocks.empty() && rocks.back().first == half) rocks.back().second += c;
                        else rocks.emplace_back(half, c);
                    } else {
                        auto floor = size / 2, ceil = floor + 1;

                        if (!rocks.empty() && rocks.back().first == ceil) rocks.back().second += temp;
                        else rocks.emplace_back(ceil, temp);

                        if (!rocks.empty() && rocks.back().first == floor) rocks.back().second += temp;
                        else rocks.emplace_back(floor, temp);
                    }
                    j++;
                }
            }
        }
        return true;
    };

    auto m = LLONG_MAX;
    unordered_set<long long> seen;
    for (int i = 1; i <= n << 1; i <<= 1) {
        auto l = (long long) (S[0] - 1) * i, r = (long long) S[0] * i;
        for (int j = 0; j < i; j++)
            for (auto m0 : {l + j, r + j})
                if (sum <= m0 && m0 < m && seen.emplace(m0).second && valid(m0)) m = m0;
    }

    if (m <= 1e18) cout << m;
    else cout << "IMPOSSIBLE";
}
