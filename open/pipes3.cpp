#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    while (n--) {
        int R, C;
        cin >> R >> C;
        cin.ignore();
        R *= 2;
        C *= 2;

        vector<string> floors(R + 1);
        for (auto &f : floors) getline(cin, f);

        array<unordered_map<int, int>, 2> dp;
        dp[0][0] = 0;
        int cost = INT_MAX;
        for (int i = 1, k = 0; i <= R; i += 2) {
            for (int j = 1; j <= C; j += 2, k ^= 1) {
                dp[k ^ 1].clear();

                for (auto [prev, c] : dp[k]) {
                    auto update = [&](int curr, int v = 0) {
                        int &cost = dp[k ^ 1][curr];
                        cost = cost ? min(cost, dp[k][prev] + v) : dp[k][prev] + v;
                    };
                    
                    int curr = prev;
                    if ((curr & (3 << C)) && (curr & 3)) {
                        int left = curr & 3, up = (curr & (3 << C)) >> C;
                        curr ^= left;
                        curr ^= (up << C);

                        if (left == 1 && up == 2) {
                            if (i == R - 1 && j == C - 1) cost = min(cost, c);
                            continue;
                        }

                        if (left != 2 || up != 1) {
                            bool rev = left != 2 || up != 2;
                            for (int count = 0, l = (!rev ? 2 : C - 2); (!rev ? l < C : l > 0); (!rev ? l += 2 : l -= 2)) {
                                if (((curr >> l) & 3) == (!rev ? 2 : 1)) count--;
                                else if (((curr >> l) & 3) == (!rev ? 1 : 2)) count++;

                                if (count == 1) {
                                    curr ^= (3 << l);
                                    break;
                                }
                            }
                        }
                        update(curr << 2);
                    } else if (!(curr & (3 << C)) && !(curr & 3)) {
                        if (i < R - 2 && j < C - 2) update((curr << 2) | 6, (floors[i][j + 1] - '0') + (floors[i + 1][j] - '0'));
                    } else {
                        int left = curr & 3, up = (curr & (3 << C)) >> C;

                        if (left) curr ^= left;
                        else {
                            curr ^= (up << C);
                            left = up;
                        }

                        if (i < R - 2) update((curr << 2) | (left << 2), floors[i + 1][j] - '0');
                        if (j < C - 2) update((curr << 2) | left, floors[i][j + 1] - '0');
                    }
                }
            }
        }

        cout << cost << "\n";
    }
}
