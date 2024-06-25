#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int s, n;
        cin >> s >> n;

        vector<int> history(n + 2 * s, -1);
        for (int i = 0; i < n; i++) cin >> history[i + s];

        vector<int> count(s, 0);
        vector<bool> reshuffle(n + s + 1, false);
        int songs = 0, positions = 0;
        for (int i = 0; i < n + s + 1; i++) {
            if (songs == s || i < s && songs == i || i > n && songs == n + s - i) reshuffle[i] = true;
            if (i == n + s) break;
            if (history[i] && !(--count[history[i] - 1])) songs--;
            if (history[i + s] && !(count[history[i + s] - 1]++)) songs++;
        }

        for (int i = 0; i < s; i++) {
            for (int j = i; j < n + s + 1; j += s)
                if (!reshuffle[j]) goto next;

            positions++;
            next:;
        }

        cout << (positions == n + 1 ? s : positions) << "\n";
    }
}
