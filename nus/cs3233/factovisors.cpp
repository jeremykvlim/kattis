#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    while (cin >> n >> m) {
        if (!m) {
            cout << "0 does not divide " << n << "!\n";
            continue;
        }

        auto count = [&](int n, int d) {
            int count = 0;
            while (n) {
                n /= d;
                count += n;
            }
            return count;
        };

        bool divides = true;
        int temp = m;
        for (auto i = 2LL; i * i <= temp; i++) {
            if (!(temp % i)) {
                int pow = 0;
                while (!(temp % i)) {
                    temp /= i;
                    pow++;
                }
                if (pow > count(n, i)) {
                    divides = false;
                    break;
                }
            }
        }
        if (temp > 1) divides = count(n, temp);
        cout << m << (divides ? " divides " : " does not divide ") << n << "!\n";
    }
}
