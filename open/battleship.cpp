#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int w, h, n;
        cin >> w >> h >> n;

        array<int, 2> ships{0, 0};
        array<vector<string>, 2> players{vector<string>(h), vector<string>(h)};
        for (int i = h - 1; ~i; i--) {
            cin >> players[0][i];

            for (char c : players[0][i]) ships[0] += c == '#';
        }

        for (int i = h - 1; ~i; i--) {
            cin >> players[1][i];

            for (char c : players[1][i]) ships[1] += c == '#';
        }

        while (n > 0 && ships[0] && ships[1])
            for (int i = 0; i < 2; i++)
                while (n-- > 0) {
                    int x, y;
                    cin >> x >> y;

                    if (players[i ^ 1][y][x] == '#') {
                        players[i ^ 1][y][x] = '_';
                        if (!--ships[i ^ 1])break;
                    } else break;
                }

        while (n-- > 0) {
            int _, __;
            cin >> _ >> __;
        }

        if ((ships[0] && ships[1]) || (!ships[0] && !ships[1])) cout << "draw\n";
        else if (ships[0]) cout << "player one wins\n";
        else cout << "player two wins\n";
    }
}
