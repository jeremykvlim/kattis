#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    mt19937 rng(random_device{}());
    while (T--) {
        int n;
        cin >> n;

        vector<int> order(n);
        iota(order.begin(), order.end(), 0);
        shuffle(order.begin(), order.end(), rng);

        vector<int> p(n);
        iota(p.begin(), p.end(), 0);
        for (int i = 0; i < n; i++) p[order[i]] = order[(i + 1) % n];

        auto query = [&](const auto &a) -> string {
            cout << "? ";
            for (int ai : a) cout << ai + 1 << " ";
            cout << "\n" << flush;

            string s;
            cin >> s;
            return s;
        };
        auto s = query(p);

        string t;
        for (int i = 0; i < n; i++) t += s[order[i]];
        t += t;

        int k = 0;
        for (int i = 0, j, len = 0; i < n; i = j + 1) {
            for (; i < n && t[i] != '1'; i++);
            if (i == n) break;

            for (j = i; j - i < n && t[j] == '1'; j++);

            if (len < j - i) {
                len = j - i;
                k = i;
            }
        }
        k = order[k];

        vector<int> temp(n);
        auto check = [&]() {
            fill(temp.begin(), temp.end(), k);
            s = query(temp);
            if (count_if(s.begin(), s.end(), [&](char c) { return c == '1'; }) * 2 > n) return k + 1;
            return -1;
        };
        int x = check();
        if (x != -1) {
            cout << "! " << x << "\n" << flush;
            continue;
        }

        for (int i = 0, j, len = 0; i < n; i = j + 1) {
            for (; i < n && t[i] != '1'; i++);
            if (i == n) break;

            for (j = i; j - i < n && t[j] == '1'; j++);

            if (len < j - i && s[order[i]] == '0') {
                len = j - i;
                k = order[i];
            }
        }
        cout << "! " << check() << "\n" << flush;
    }
}
