#include <bits/stdc++.h>
using namespace std;

vector<int> prefix_function(string s) {
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

        auto pi = prefix_function(p);
        for (int i = pi.size(); i; i = pi[i - 1])
            if (2 * p.size() - pi[i - 1] <= n) len.emplace_back(pi[i - 1]);
    }

    sort(preds.begin(), preds.end(), [](auto p1, auto p2) {return p1.second < p2.second;});
    for (auto &[p, len] : preds) cout << p << "\n";
}
