#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    if (n == 1 || n % 3 == 2) {
        cout << 0;
        exit(0);
    }

    int m = 4 * ((n - 2) / 3 + !!(n % 3)) + 8;
    vector<int> gaps(m, 3);
    int i = (n - 2) / 3 + !!(n % 3);
    gaps[0] = 0;
    gaps[1] = gaps[i + 2] = gaps[i + 3] = gaps[2 * i + 4] = gaps[2 * i + 5] = gaps[3 * i + 6] = gaps[3 * i + 7] = n % 3 ? 1 : 2;

    int cell = 1;
    cout << m << "\n";
    for (int g : gaps) cout << (cell += g) << " ";
}
