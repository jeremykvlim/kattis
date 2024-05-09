#include <bits/stdc++.h>
using namespace std;

vector<int> kmp(string s) {
    vector<int> pi(s.size());
    for (int i = 1; i < s.size(); i++) {
        int j = pi[i - 1];
        while (j && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }

    return pi;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, s;
    cin >> n >> s;

    vector<pair<string, vector<int>>> preds(s);
    for (auto &[p, len] : preds) {
        cin >> p;

        auto pi = kmp(p);
        for (int j = pi.size() - 1; j > 0; j = pi[j] - 1)
            if (2 * p.size() - pi[j] <= n) len.emplace_back(pi[j]);
    }

    sort(preds.begin(), preds.end(), [](auto p1, auto p2) {return p1.second < p2.second;});
    for (auto &[p, len] : preds) cout << p << "\n";
}
