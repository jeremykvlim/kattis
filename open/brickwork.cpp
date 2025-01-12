#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, w;
    cin >> n >> w;

    vector<int> b(n);
    for (int &bi : b) {
        cin >> bi;

        if (bi == w) {
            cout << "possible\n1\n" << w << "\n1\n" << w;
            exit(0);
        }
    }
    sort(b.begin(), b.end());
    b.erase(unique(b.begin(), b.end()), b.end());
    n = b.size();

    bitset<(int) 3e5 + 1> dp(1);
    vector<int> valid;
    for (int i = n - 1; ~i; i--) {
        if (!(w % b[i])) valid.emplace_back(b[i]);
        for (int j = 1; j * b[i] < w; j++)
            if (dp[w - j * b[i]]) {
                vector<int> bricks(j, i);
                int built = j * b[i], remaining = w - built;
                for (int k = i + 1; k < n && remaining > 0; k++)
                    while (remaining > 0 && dp[remaining - b[k]]) {
                        remaining -= b[k];
                        bricks.emplace_back(k);
                    }
                cout << "possible\n";

                if (built % b[bricks[j]]) {
                    cout << bricks.size() << "\n";
                    for (int k : bricks) cout << b[k] << " ";
                    cout << "\n";

                    reverse(bricks.begin() + 1, bricks.begin() + j + 1);
                    rotate(bricks.begin(), bricks.begin() + 1, bricks.end());

                    cout << bricks.size() << "\n";
                    for (int k : bricks) cout << b[k] << " ";
                } else {
                    cout << bricks.size() - j + built / b[bricks[j]] << "\n";
                    for (int _ = 0; _ < built / b[bricks[j]]; _++) cout << b[bricks[j]] << " ";
                    for (int k = j; k < bricks.size(); k++) cout << b[bricks[k]] << " ";
                    cout << "\n";

                    rotate(bricks.begin(), bricks.begin() + 1, bricks.end());

                    cout << bricks.size() << "\n";
                    for (int k : bricks) cout << b[k] << " ";
                }
                exit(0);
            }
        for (int j = b[i]; j <= w; j <<= 1) dp |= (dp << j);
    }

    if (valid.size() >= 2) {
        valid.resize(2);
        cout << "possible\n";
        for (int l : valid) {
            cout << w / l << "\n";
            for (int _ = 0; _ < w / l; _++) cout << l << " ";
            cout << "\n";
        }
    } else cout << "impossible";
}

