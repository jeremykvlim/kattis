#include <bits/stdc++.h>
using namespace std;

bool check(int i, char c, string &key, vector<string> &answers) {
    if (key[i] != '.') return key[i] == c;
    key[i] = c;
    for (auto &s : answers)
        if (s[i] == c)
            for (int j = 0; j < s.size(); j++)
                if (j != i && s[j] != 'X' && !check(j, s[j] ^ 'F' ^ 'T', key, answers)) return false;
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<string> answers(n);
    for (auto &s : answers) cin >> s;

    string key(k, '.');
    bool exists = true;
    for (int i = 0; i < k && exists; i++)
        if (key[i] == '.')
            for (int j = 0; exists; j++) {
                auto save = key;
                if (check(i, "FT"[j], key, answers)) break;
                
                key = save;
                if (j) exists = false;
            }

    cout << (exists ? key : "-1");
}
