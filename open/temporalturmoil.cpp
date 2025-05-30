#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    while (n--) {
        int i, j;
        cin >> i >> j;

        int T1 = 60 * 60 * 12 * i, T2 = 60 * 60 * ((T1 * 59) / (11 * 60 * 60) + j), t1, t2;
        if (T1 * 59 >= T2 * 11) {
            t1 = T1;
            t2 = 11;
        } else {
            t1 = T2;
            t2 = 59;
        }

        int s = ((t1 + t2 - 1) / t2 + (60 * 60 * 36)) % (60 * 60 * 24);
        cout << setw(2) << setfill('0') << s / (60 * 60) << ":"
             << setw(2) << setfill('0') << (s % (60 * 60)) / 60 << ":"
             << setw(2) << setfill('0') << s % 60 << "\n";
    }
}
