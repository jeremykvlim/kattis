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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    string b;
    while (cin >> t >> b) {
        cin.ignore();

        auto pi = prefix_function(b);
        while (t--) {
            string s;
            getline(cin, s);

            int k = 0;
            auto temp = s;
            vector<int> match(s.size());
            for (int i = 0, j = 0; i < s.size(); i++) {
                while (j && s[i] != b[j]) j = pi[j - 1];
                if (s[i] == b[j]) j++;

                temp[k] = s[i];
                match[k++] = j;

                if (j == b.size()) {
                    k -= b.size();
                    j = !k ? 0 : match[k - 1];
                }
            }

            temp.resize(k);
            cout << temp << "\n";
        }
    }
}
