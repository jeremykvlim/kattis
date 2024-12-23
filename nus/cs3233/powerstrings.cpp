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

    string s;
    while (cin >> s && s != ".") {
        int n = s.size();
        auto pi = prefix_function(s);

        if (!pi[n - 1] || n % (n - pi[n - 1])) cout << "1\n";
        else cout << max(n / (n - pi[n - 1]), 1) << "\n";
    }
}
