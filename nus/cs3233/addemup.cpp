#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, s;
    cin >> n >> s;

    vector<unsigned long long> masks((s + 63) / 64, 0);
    while (n--) {
        int num;
        cin >> num;

        int turned;
        auto turn = [&]() {
            auto s = to_string(num);
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

        for (int v : {num, turned}) {
            if (!~v) break;
            if (1 <= s - v && s - v <= s) {
                int i = (s - v) / 64, j = (s - v) % 64;
                if (i < masks.size() && ((masks[i] >> j) & 1)) {
                    cout << "YES";
                    exit(0);
                }
            }
        }

        for (int v : {num, turned}) {
            if (!~v) break;
            if (1 <= v && v <= s) {
                int i = v / 64, j = v % 64;
                if (i < masks.size()) masks[i] |= (1ULL << j);
            }
        }
    }
    cout << "NO";
}
