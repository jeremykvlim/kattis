#include<bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n) {
        vector<int> sequence(n), tail(n, 0), prev(n, -1);
        for (int i = 0; i < n; i++) cin >> sequence[i];

        int longest = 1;
        for (int i = 1; i < n; i++) {
            if (sequence[i] > sequence[tail[longest - 1]]) {
                prev[i] = tail[longest - 1];
                tail[longest++] = i;
            } else {
                int l = -1, r = longest - 1;
                while (l + 1 < r) {
                    int m = l + (r - l) / 2;
                    if (sequence[tail[m]] >= sequence[i]) r = m;
                    else l = m;
                }
                
                tail[r] = i;
                prev[i] = r > 0 ? tail[r - 1] : -1;
            }
        }

        cout << longest << "\n";
        vector<int> lis(longest);
        for (int i = tail[longest - 1]; i >= 0; i = prev[i]) lis[--longest] = i;
        for (auto &i : lis) cout << i << " ";
        cout << "\n";
    }
}
