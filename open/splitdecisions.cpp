#include <bits/stdc++.h>
using namespace std;

struct Hash {
    static uint64_t encode(tuple<string, string, int, int> t) {
        auto encoded = 0ULL;
        auto [s1, s2, i1, i2] = t;
        for (auto e : {s1, s2})
            for (auto c : e) encoded = (encoded << 8) | c;
        encoded = (encoded << 8) | i1;
        encoded = (encoded << 8) | i2;
        return encoded;
    }

    static uint64_t h(uint64_t key) {
        auto hash = key + 0x9e3779b97f4a7c15;
        hash = (hash ^ (hash >> 30)) * 0xbf58476d1ce4e5b9;
        hash = (hash ^ (hash >> 27)) * 0x94d049bb133111eb;
        hash = hash ^ (hash >> 31);
        return hash;
    }

    size_t operator()(tuple<string, string, int, int> t) const {
        static uint64_t SEED = chrono::steady_clock::now().time_since_epoch().count();
        return h(encode(t) + SEED);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<string> words(n);
    for (auto &w : words) cin >> w;
    sort(words.begin(), words.end());
    words.erase(unique(words.begin(), words.end()), words.end());

    unordered_map<tuple<string, string, int, int>, int, Hash> pairs;
    for (int i = 0; i < words.size(); i++)
        for (int j = i + 1; j < words.size(); j++) {
            if (words[i].size() != words[j].size()) continue;

            vector<int> diff;
            for (int k = 0; k < words[i].size(); k++)
                if (words[i][k] != words[j][k]) diff.emplace_back(k);

            if (diff.size() == 2 && diff[0] + 1 == diff[1]) {
                auto s1 = words[i].substr(diff[0], 2), s2 = words[j].substr(diff[0], 2);
                pairs[{min(s1, s2), max(s1, s2), diff[0], words[i].size()}]++;
            }
        }

    int count = 0;
    for (auto [p, c] : pairs)
        if (c == 1) count++;

    cout << count;
}
