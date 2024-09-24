#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int k;
        cin >> k;

        vector<char> steps(2 * k);
        for (auto &s : steps) cin >> s;

        int s = 0;
        vector<int> points{0, 0}, rounds{k, k}, end{-1, -1};
        for (; s < 2 * k; s++) {
            if (steps[s] == 'E') points[s & 1]++;
            rounds[s & 1]--;
            if (points[0] + rounds[0] < points[1] || points[1] + rounds[1] < points[0]) {
                end[~s & 1] = s++;
                break;
            }
        }

        for (int c = 0; c < s; c++) {
            char a = '_', b = '_';
            for (int i = c; i < s; i++)
                if (end[i & 1] != i) {
                    if ((i & 1 ? a : b) != '_' && (i & 1 ? a : b) != steps[i]) goto next;
                    (i & 1 ? a : b) = steps[i];
                }

            cout << s << " " << c << "\n";
            break;
            next:;
        }
    }
}
