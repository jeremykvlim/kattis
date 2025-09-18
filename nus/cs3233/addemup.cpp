#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, s;
    cin >> n >> s;

    bitset<(int) 2e8 + 1> bs;
    while (n--) {
        int card;
        cin >> card;

        int turned;
        auto turn = [&]() {
            auto str = to_string(card);
            reverse(str.begin(), str.end());

            string t;
            for (char c : str) {
                switch (c) {
                    case '0': t += '0'; break;
                    case '1': t += '1'; break;
                    case '2': t += '2'; break;
                    case '5': t += '5'; break;
                    case '6': t += '9'; break;
                    case '8': t += '8'; break;
                    case '9': t += '6'; break;
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
            if (s > v && bs[s - v]) {
                cout << "YES";
                exit(0);
            }
        }

        for (int v : {card, turned}) {
            if (!~v) break;
            if (0 < v && v <= s) bs[v] = true;
        }
    }

    cout << "NO";
}
