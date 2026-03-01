#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, s;
    cin >> n >> s;

    auto build = [&](int extra, int len_start, bool vowel_start) {
        int r = s - 4 * n - extra;
        if (r < 0 || (r & 15)) return;

        int consonants = r >> 4, vowels = n - consonants,
            blocks = consonants + vowel_start, pairs = vowels - blocks - len_start + 1;
        if (0 <= pairs && pairs < blocks) {
            string word(!vowel_start, 'b');
            for (int i = 0; i < blocks; i++) {
                int len = 1;
                if (!i) len = len_start;
                else if (pairs) {
                    len = 2;
                    pairs--;
                }

                word += string(len, 'a');
                if (i < blocks - 1) word += 'b';
            }
            cout << word;
            exit(0);
        }
    };
    build(0, 1, true);
    build(0, 2, false);
    build(5, 1, false);
    build(21, 2, true);
    cout << "IMPOSSIBLE";
}
