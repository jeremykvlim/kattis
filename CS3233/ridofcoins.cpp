#include <iostream>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int p, n1, n5, n10, n25;
    cin >> p >> n1 >> n5 >> n10 >> n25;

    int most = -1, max1 = min(n1, p);
    for (int i = 0; i < 25 && i <= max1; i++) {
        int used1 = p - (max1 - i), max5 = min(n5, used1 / 5);
        for (int j = 0; j < 10 && j <= max5; j++) {
            int used5 = used1 - 5 * (max5 - j), max10 = min(n10, used5 / 10);
            for (int k = 0; k < 5 && k <= max10; k++) {
                int used10 = used5 - 10 * (max10 - k);
                if (!(used10 % 25) && used10 / 25 <= n25) most = max(most, max1 - i + max5 - j + max10 - k + used10 / 25);
            }
        }
    }

    cout << (most == -1 ? "Impossible" : to_string(most));
}
