#include <bits/stdc++.h>
using namespace std;

vector<long long> rabin_karp(string &s, int len) {
    auto pow = 1LL;
    int p = 31, mod = 1e9 + 7;
    for (int i = 0; i < len; i++) pow = (pow * p) % mod;

    vector<long long> pref(s.size() + 1, 0), hashes;
    for (int i = 0; i < s.size(); i++) {
        pref[i + 1] = ((pref[i] * p) % mod + s[i]) % mod;

        if (i + 1 >= len) {
            auto h = pref[i + 1] - (pref[i + 1 - len] * pow) % mod;
            if (h < 0) h += mod;

            hashes.emplace_back(h);
        }
    }

    return hashes;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    int q;
    cin >> s >> q;

    vector<vector<tuple<long long, int, int>>> queries(s.size() + 1);
    for (int i = 0; i < q; i++) {
        string t;
        int k;
        cin >> t >> k;

        if (t.size() <= queries.size())
            queries[t.size()].emplace_back(rabin_karp(t, t.size())[0], k, i);
    }

    vector<int> pos(q, -1);
    for (int len = 1; len < s.size(); len++) {
        if (queries[len].empty()) continue;

        auto hashes = rabin_karp(s, len);

        unordered_map<long long, vector<int>> indices;
        for (int i = 0; i < hashes.size(); i++) indices[hashes[i]].emplace_back(i);

        for (auto [h, k, i] : queries[len])
            if (indices.count(h))
                if (indices[h].size() >= k) pos[i] = indices[h][k - 1] + 1;
    }

    for (int p : pos) cout << p << "\n";
}
