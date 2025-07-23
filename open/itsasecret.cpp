#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        cin.ignore();

        string key;
        getline(cin, key);

        vector<string> texts(n);
        for (auto &t : texts) getline(cin, t);

        string order;
        vector<bool> used(26, false);
        used['J' - 'A'] = true;
        for (char ch : key)
            if (isalpha(ch)) {
                char c = toupper(ch);
                if (c == 'J') c--;
                if (!used[c - 'A']) {
                    used[c - 'A'] = true;
                    order += c;
                }
            }

        for (int c = 0; c < 26; c++)
            if (!used[c]) {
                used[c] = true;
                order += (char) c + 'A';
            }

        array<array<int, 2>, 26> pos;
        vector<string> table(5);
        for (int i = 0; i < 25; i++) {
            int r = i / 5, c = i % 5;
            table[r][c] = order[i];
            pos[order[i] - 'A'] = {r, c};
        }

        for (const auto &t : texts) {
            string message;
            for (char ch : t)
                if (isalpha(ch)) {
                    char c = toupper(ch);
                    if (c == 'J') c--;
                    message += c;
                }

            vector<pair<char, char>> digraphs;
            for (int i = 0; i < message.size(); i++) {
                char a = message[i], b = i + 1 < message.size() ? message[i + 1] : 'X';
                if (a == b) b = 'X';
                else i++;
                digraphs.emplace_back(a, b);
            }

            string encoded;
            for (auto [a, b] : digraphs) {
                if (a == 'X' && b == 'X') {
                    encoded += "YY";
                    continue;
                }

                auto [r1, c1] = pos[a - 'A'];
                auto [r2, c2] = pos[b - 'A'];
                if (r1 == r2) {
                    encoded += table[r1][(c1 + 1) % 5];
                    encoded += table[r2][(c2 + 1) % 5];
                } else if (c1 == c2) {
                    encoded += table[(r1 + 1) % 5][c1];
                    encoded += table[(r2 + 1) % 5][c2];
                } else {
                    encoded += table[r1][c2];
                    encoded += table[r2][c1];
                }
            }
            cout << encoded << "\n";
        }
        cout << "\n";
    }
}
