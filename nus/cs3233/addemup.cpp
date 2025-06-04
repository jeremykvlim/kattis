#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, s;
    cin >> n >> s;

    vector<unsigned long long> masks((s + 63) >> 6, 0);
    while (n--) {
        int card;
        cin >> card;

        int turned;
        auto turn = [&]() {
            auto s = to_string(card);
            reverse(s.begin(), s.end());

            string t;
            for (char c : s) {
                switch (c) {
                    case '0':
                        t += '0';
                        break;
                    case '1':
                        t += '1';
                        break;
                    case '2':
                        t += '2';
                        break;
                    case '5':
                        t += '5';
                        break;
                    case '6':
                        t += '9';
                        break;
                    case '8':
                        t += '8';
                        break;
                    case '9':
                        t += '6';
                        break;
                    default:
                        turned = -1;
                        return;
                }
            }

            turned = stoi(t);
        };
        turn();

        for (int v : {card, turned}) {
            if (!~v) break;
            if (s > v) {
                int i = (s - v) >> 6, j = (s - v) & 63;
                if (i < masks.size() && (masks[i] >> j) & 1) {
                    cout << "YES";
                    exit(0);
                }
            }
        }

        for (int v : {card, turned}) {
            if (!~v) break;
            if (1 <= v && v <= s) {
                int i = v >> 6, j = v & 63;
                if (i < masks.size()) masks[i] |= 1ULL << j;
            }
        }
    }
    cout << "NO";
}
