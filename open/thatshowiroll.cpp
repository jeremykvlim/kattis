#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<array<int, 6>> states;
    for (int r1 = 0; r1 < 21; r1++)
        for (int r2 = r1 + 1; r2 < 22; r2++)
            for (int r3 = r2 + 1; r3 < 23; r3++)
                for (int r4 = r3 + 1; r4 < 24; r4++)
                    for (int r5 = r4 + 1; r5 < 25; r5++)
                        for (int r6 = r5 + 1; r6 < 26; r6++) states.push_back({r1, r2, r3, r4, r5, r6});

    int t;
    cin >> t;

    while (t--) {
        int l;
        cin >> l;

        vector<bool> root(26, false);
        vector<int> adj_mask(26, 0);
        while (l--) {
            string s;
            cin >> s;

            int r = s[0] - 'a';
            root[r] = true;
            if (s.size() > 1) {
                int c = s[1] - 'a';
                adj_mask[r] |= 1 << c;
            }
        }

        double score = -1;
        array<int, 6> choice;
        for (auto rolls : states) {
            int m = 0;
            for (int r : rolls) m |= 1 << r;

            double root_sum = 0;
            for (int r : rolls)
                if (root[r]) root_sum += max(1., (2. * __builtin_popcount(adj_mask[r] & m)) / 6);

            auto curr = root_sum / 6;
            if (score + 1e-6 < curr) {
                score = curr;
                choice = rolls;
            }
        }

        string s;
        for (int r : choice) s += (char) ('a' + r);
        cout << fixed << setprecision(6) << s << "\n" << score << "\n";
    }
}
