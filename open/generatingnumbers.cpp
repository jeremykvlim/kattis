#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int x;
        cin >> x;

        if (x <= 10) {
            cout << x - 1 << "\n";
            continue;
        }

        int ops = 0, rank = 1;
        for (int i = 1; 10 * rank <= x; i++, rank *= 10) ops += 10 * i - 1;

        if (rank == x) {
            cout << ops << "\n";
            continue;
        }

        if (!(x % rank)) {
            x--;
            ops++;
        }

        if (x % 10) ops--;

        deque<int> digits;
        bool one = false;
        while (x) {
            ops += x % 10 + (x % 10 > 0);
            digits.emplace_front(x % 10);
            x /= 10;
            if (digits.size() > 1 && digits[0] == 1) one = true;
        }
        if (one) ops--;

        cout << ops - 1 << "\n";
    }
}
