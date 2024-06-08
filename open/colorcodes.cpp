#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, p;
    cin >> n >> p;

    vector<int> s(p);
    for (int &si : s) cin >> si;

    auto next = [](int i) {
        int b = i | (i - 1) + 1;
        return b | ((((b & -b) / (i & -i)) >> 1) - 1);
    };

    vector<int> code(1 << n);
    vector<bool> visited(1 << n, false);
    visited[0] = true;
    for (int i = 1; i < 1 << n; i++) {
        for (int si : s)
            for (int j = (1 << si) - 1; j < 1 << n; j = next(j)) {
                int c = code[i - 1] ^ j;
                if (!visited[c]) {
                    code[i] = c;
                    visited[c] = true;
                    goto next;
                }
            }

        cout << "impossible";
        exit(0);
        next:;
    }

    for (int c : code) cout << bitset<16>(c).to_string().substr(16 - n) << "\n";
}
