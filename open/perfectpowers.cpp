#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long x;
    while (cin >> x && x) {
        int p = 1;
        for (auto b = 2LL; b * b <= abs(x); b++) {
            auto a = b * b;
            p = 2;
            while (a < abs(x)) {
                a *= b;
                p++;
            }

            if (a == abs(x) && (x > 0 || p & 1)) break;
            else p = 1;
        }

        cout << p << "\n";
    }
}
