#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> data;
    int x;
    while (cin >> x) {
        data.emplace_back(x);
        char _;
        cin >> _;
    }

    int n = data.size();
    for (int len = min(256, n / 2); len >= 64; len--) {
        string S(256, 0), T(256, 0);
        for (int i = 0; i < len; i++) {
            S[data[i]]++;
            T[data[i + len]]++;
        }

        unordered_map<string, int> seen;
        pair<int, int> indices{n + 1, n + 1};
        for (int s = 0, t = len; t <= n - len; s++, t++) {
            seen[S] = s;
            auto it = seen.find(T);
            if (it != seen.end()) indices = min(indices, make_pair(it->second, t));

            if (t < n - len) {
                S[data[s]]--;
                S[data[s + len]]++;
                T[data[t]]--;
                T[data[t + len]]++;
            }
        }

        if (indices != make_pair(n + 1, n + 1)) {
            cout << len << "\n" << indices.first << "\n" << indices.second;
            exit(0);
        }
    }
    cout << "no key";
}
