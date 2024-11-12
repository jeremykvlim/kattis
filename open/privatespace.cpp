#include <bits/stdc++.h>
using namespace std;

struct Hash {
    size_t operator()(const vector<int> &v) const {
        auto h = 0ULL;
        for (int e : v) h ^= e + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

int calc(vector<int> groups, unordered_map<vector<int>, int, Hash> &dp, vector<int> pref) {
    if (dp.count(groups)) return dp[groups];

    int least = pref[13];
    for (int i = 0; i < groups.size(); i++)
        if (groups[i]) {
            groups[i]--;
            int req = calc(groups, dp, pref);
            if (req == pref[13]) continue;

            groups[i]++;
            auto x = upper_bound(pref.begin() + i + 1, pref.end(), req);
            least = min(least, (*x - req > i + 1 ? max(req, *(x - 1)) : *x) + i + 2);
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
        int x = upper_bound(pref.begin(), pref.end(), calc(groups, dp, pref) - 1) - pref.begin();
        cout << (x > 12 ? "impossible" : to_string(x));
    }
}
