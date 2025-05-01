#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int s, d, p;
    cin >> s >> d >> p;

    for (int hp = 20;; hp -= 10) {
        int potions;
        if (hp > 10) potions = 0;
        else if (hp > 2) potions = 1;
        else potions = 2;

        bool last = p < potions;
        for (int t = 1; t <= 4; t++)
            if (t <= 4 - (last ? 0 : potions)) {
                d -= 4;
                if (d <= 0) {
                    cout << "Steve wins";
                    exit(0);
                }

                if (!--s) {
                    cout << "broken sword";
                    exit(0);
                }
            } else {
                p--;
                hp = min(20, hp + 8);
            }

        if (last) {
            cout << "not enough potions";
            exit(0);
        }
    }
}
