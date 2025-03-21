#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, Q, s;
    cin >> n >> Q >> s;

    vector<int> q(s), c(Q);
    for (int &qi : q) cin >> qi;
    for (int &ci : c) cin >> ci;

    int sum = 0;
    vector<int> queued(Q, 0);
    while (n--) {
        int d;
        cin >> d;

        vector<int> windows(Q, 0);
        for (int qi : q) {
            int a;
            cin >> a;

            windows[qi - 1] += a;
        }

        for (int i = 0; i < Q; i++) {
            if (windows[i] > c[i]) {
                cout << "impossible";
                exit(0);
            }

            queued[i] += windows[i];
            if (queued[i] > c[i]) {
                sum -= queued[i] - c[i];
                if (sum < 0) {
                    cout << "impossible";
                    exit(0);
                }
                queued[i] = c[i];
            }
        }
        sum += d;
    }

    cout << (accumulate(queued.begin(), queued.end(), 0) > sum ? "impossible" : "possible");
}
