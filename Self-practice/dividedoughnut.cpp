#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int count1;
    cout << "QUERY 0 499999999\n" << flush;
    cin >> count1;

    count1 -= n - count1;
    if (!count1) {
        cout << "YES 0";
        exit(0);
    }

    int l = 0, r = 5e8, m;
    while (l + 1 < r) {
        m = l + (r - l) / 2;

        if (l + 2 == r) {
            cout << "YES " << m;
            exit(0);
        }

        int count2;
        cout << "QUERY " << m << " " << m + 499999999 << "\n" << flush;
        cin >> count2;

        count2 -= n - count2;
        if (!count2) {
            cout << "YES " << m;
            exit(0);
        }

        if ((count1 > 0) != (count2 > 0)) r = m;
        else l = m;
    }
}
