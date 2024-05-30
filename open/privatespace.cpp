#include <bits/stdc++.h>
using namespace std;

struct Hash {
    static uint64_t h(uint64_t key) {
        auto hash = key + 0x9e3779b97f4a7c15;
        hash = (hash ^ (hash >> 30)) * 0xbf58476d1ce4e5b9;
        hash = (hash ^ (hash >> 27)) * 0x94d049bb133111eb;
        hash = hash ^ (hash >> 31);
        return hash;
    }

    size_t operator()(const vector<int> &v) const {
        auto key = 0ULL;
        for (int e : v) key ^= e + 0x9e3779b9 + (key << 6) + (key >> 2);
        return h(key);
    }
};

int calculate(vector<int> groups, unordered_map<vector<int>, int, Hash> &dp, vector<int> pref) {
    if (dp.count(groups)) return dp[groups];

    int least = pref[13];
    for (int i = 0; i < groups.size(); i++)
        if (groups[i]) {
            groups[i]--;
            int required = calculate(groups, dp, pref);
            if (required == pref[13]) continue;

            groups[i]++;
            auto x = upper_bound(pref.begin() + i + 1, pref.end(), required);
            least = min(least, (*x - required > i + 1 ? max(required, *(x - 1)) : *x) + i + 2);
        }

    return dp[groups] = least;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> groups(n);
    unordered_map<vector<int>, int, Hash> dp{{groups, 0}};
    int people = 0;
    for (int i = 0; i < n; i++) {
        cin >> groups[i];

        people += groups[i] * (i + 1);
    }

    vector<int> pref(14);
    iota(pref.begin(), pref.end(), 0);
    partial_sum(pref.begin(), pref.end(), pref.begin(), [](int x, int y) {return x + y + 1;});

    if (people > pref[13]) cout << "impossible";
    else {
        int x = upper_bound(pref.begin(), pref.end(), calculate(groups, dp, pref) - 1) - pref.begin();
        cout << (x > 12 ? "impossible" : to_string(x));
    }
}
