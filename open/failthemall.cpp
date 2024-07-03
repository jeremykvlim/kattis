#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<string> answers(n);
    for (auto &s : answers) cin >> s;

    string key(k, '.');

    auto check = [&](auto &&self, int i, char c) -> bool {
        if (key[i] != '.') return key[i] == c;

        key[i] = c;
        for (auto &s : answers)
            if (s[i] == c)
                for (int j = 0; j < s.size(); j++)
                    if (j != i && s[j] != 'X' && !self(self, j, s[j] ^ 'F' ^ 'T')) return false;

        return true;
    };

    for (int i = 0; i < k; i++)
        if (key[i] == '.')
            for (char c : {'F', 'T'}) {
                auto temp = key;
                if (check(check, i, c)) break;
                else if (c == 'T') {
                    cout << -1;
                    exit(0);
                }
                
                key = temp;
            }

    cout << key;
}
