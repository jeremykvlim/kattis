#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int d = 12, s, p, y, j;
    while (cin >> s >> p >> y >> j) {
        int Y = d + j - p - y, yertle = Y / 3, spot = y + yertle, puff = p + yertle;

        if (Y % 3 == 1) {
            spot += (y - p <= s);
            puff += (y - p > s);
        } else if (Y % 3 == 2) {
            spot++;
            puff++;
        }

        cout << spot << " " << puff << " " << yertle << "\n";
    }
}
