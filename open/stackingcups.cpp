#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long h;
    cin >> n >> h;

    if (n == 1 && h == 1) {
        cout << 1;
        exit(0);
    }

    if (n == 1 || h < 2 * n - 1 || h > (long long) n * n || h == (long long) n * n - 2) {
        cout << "impossible";
        exit(0);
    }

    vector<int> heights;
    for (int i = n; i; i--) {
        int j = 2 * i - 1;
        if (h == j + 2) {
            for (int k = 1; k < i; k++)
                if (k != 2) heights.emplace_back(2 * k - 1);
            heights.emplace_back(3);
            heights.emplace_back(2 * i - 1);
            break;
        }

        if (h >= j) {
            h -= j;
            heights.emplace_back(j);
        }
    }

    reverse(heights.begin(), heights.end());
    vector<bool> used(2 * n, false);
    for (int height : heights) used[height] = true;
    for (int i = n; i; i--)
        if (!used[2 * i - 1]) heights.emplace_back(2 * i - 1);

    for (int height : heights) cout << height << " ";
}
