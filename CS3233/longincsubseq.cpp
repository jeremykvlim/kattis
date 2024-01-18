#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n) {
        vector<int> sequence(n), tail(n, 0), prev(n, -1);
        for (int i = 0; i < n; i++) cin >> sequence[i];

        int len = 1;
        for (int i = 1; i < n; i++) {
            if (sequence[i] > sequence[tail[len - 1]]) {
                prev[i] = tail[len - 1];
                tail[len++] = i;
            } else {
                int l = -1, r = len - 1, m;
                while (l + 1 < r) {
                    m = l + (r - l) / 2;
                    if (sequence[tail[m]] >= sequence[i]) r = m;
                    else l = m;
                }
                
                tail[r] = i;
                prev[i] = r > 0 ? tail[r - 1] : -1;
            }
        }

        cout << len << "\n";
        vector<int> lis(len);
        for (int i = tail[len - 1]; i >= 0; i = prev[i]) lis[--len] = i;
        for (auto &i : lis) cout << i << " ";
        cout << "\n";
    }
}
