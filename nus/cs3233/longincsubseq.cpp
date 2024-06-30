#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n) {
        vector<int> seq(n), tail(n, 0), prev(n, -1);
        for (int &e : seq) cin >> e;

        int len = 1;
        for (int i = 1; i < n; i++)
            if (seq[i] > seq[tail[len - 1]]) {
                prev[i] = tail[len - 1];
                tail[len++] = i;
            } else {
                int l = -1, r = len - 1, m;
                while (l + 1 < r) {
                    m = l + (r - l) / 2;
                    
                    if (seq[tail[m]] >= seq[i]) r = m;
                    else l = m;
                }

                tail[r] = i;
                prev[i] = r ? tail[r - 1] : -1;
            }
        cout << len << "\n";

        vector<int> lis(len);
        for (int i = tail[len - 1]; ~i; i = prev[i]) lis[--len] = i;
        for (int e : lis) cout << e << " ";
        cout << "\n";
    }
}
