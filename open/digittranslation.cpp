#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 9302023;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    vector<string> forms{"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    vector<int> len(s.size() + 1);
    vector<long long> count(s.size() + 1, 1);
    for (int i = 1; i <= s.size(); i++) {
        len[i] = len[i - 1] + 1;
        count[i] = count[i - 1];

        for (auto f : forms) {
            if (i < f.size()) continue;

            for (int j = 0; j < f.size(); j++)
                if (s[i - f.size() + j] != f[j]) goto next;

            if (len[i - f.size()] + 1 < len[i]) {
                len[i] = len[i - f.size()] + 1;
                count[i] = count[i - f.size()];
            } else if (len[i - f.size()] + 1 == len[i])
                count[i] = (count[i] + count[i - f.size()]) % MODULO;

            next:;
        }
    }

    cout << len.back() << "\n" << count.back();
}
