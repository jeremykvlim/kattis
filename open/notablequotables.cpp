#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string code{istreambuf_iterator<char>{cin}, istreambuf_iterator<char>{}};
    int n = code.size();

    vector<char> order{'"', '\'', '!', '@', '#', '$', '%', '^', '&', '*'};
    for (char c = 'a'; c <= 'z'; c++) order.emplace_back(c);
    for (char c = 'A'; c <= 'Z'; c++) order.emplace_back(c);

    unordered_map<char, bool> valid;
    for (char d : order) valid[d] = true;

    string original;
    auto add = [&](const auto &delim, const auto &s) {
        original += delim;
        original += s;
        original += delim;
    };

    for (int pos = 0; pos < n;) {
        char c1 = code[pos], c2 = code[pos + 2];

        auto quote = [&](char c, int p) {
            int i = p;
            while (i < n)
                if (code[i] == '\\' && i + 1 < n) i += 2;
                else if (code[i] == c) break;
                else i++;

            if (i >= n) {
                original += c1;
                pos++;
                return;
            }

            string s;
            unordered_map<char, bool> seen;
            for (int j = p; j < i; j++)
                if (code[j] == '\\' && j + 1 < i && code[j + 1] == c) {
                    s += c;
                    seen[c] = true;
                    j++;
                } else {
                    s += code[j];
                    seen[code[j]] = true;
                }

            char delim;
            for (char d : order)
                if (!seen[d]) {
                    delim = d;
                    break;
                }

            if (delim != '"' && delim != '\'') original += "Q=";
            add(delim, s);
            pos = i + 1;
        };

        if (c1 == '"' || c1 == '\'') quote(c1, pos + 1);
        else if (c1 == 'Q' && pos + 2 < n && code[pos + 1] == '=' && valid[c2]) quote(c2, pos + 3);
        else {
            original += c1;
            pos++;
        }
    }
    cout << original;
}
