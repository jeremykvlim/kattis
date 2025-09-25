#include <bits/stdc++.h>
using namespace std;

vector<int> prefix_function(const string &s) {
    vector<int> pi(s.size());
    for (int i = 1; i < s.size(); i++) {
        int j = pi[i - 1];
        while (j && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }

    return pi;
}

vector<int> z_function(const string &s) {
    int n = s.size();
    vector<int> z(n, 0);
    for (int i = 1, l = 0, r = 0; i < n; i++) {
        if (i < r) z[i] = min(r - i, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
    }

    return z;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    string s;
    cin >> n >> s;

    auto pi = prefix_function(s);
    stack<int> st;
    for (int i = n; i; i = pi[i - 1])
        if (i > pi[i - 1] * 2) st.emplace(i);

    auto z = z_function(s);
    while (!st.empty()) {
        int len = st.top();
        st.pop();

        if (len == n) {
            done:;
            cout << len;
            exit(0);
        }

        for (int i = 1, prev = len; i <= prev && i + len <= n; i++)
            if (len <= z[i]) {
                if (i + len == n) goto done;
                prev = i + len;
            }
    }
}
