#include <bits/stdc++.h>
using namespace std;

int read() {
    string s;
    cin >> s;
    
    return s == "?" ? -1 : stoi(s);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;

    while (n--) {
        int t = read(), w = read(), d = read(), l = read(), p = read();
        if (t == -1 && l == -1) l = 0;

        for (int won = w == -1 ? 0 : w; won <= (w == -1 ? 100 : w); won++)
            for (int drew = d == -1 ? 0 : d; drew <= (d == -1 ? 100 : d); drew++) {
                int total = t == -1 ? won + drew + l : t, lost = l == -1 ? t - won - drew : l, points = p == -1 ? 3 * won + drew : p;

                if (total == won + drew + lost && 0 <= total && total <= 100 && 0 <= lost && lost <= 100 && points == 3 * won + drew) 
                    cout << total << " " << won << " " << drew << " " << lost << " " << points << "\n";
            }
    }
}
