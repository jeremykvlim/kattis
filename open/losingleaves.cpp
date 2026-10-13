#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> p(n), height(n, 1);
    for (int i = 1; i < n; i++) cin >> p[i];
    for (int i = n - 1; i; i--) height[p[i]] = max(height[p[i]], height[i] + 1);

    vector<int> count(n + 1, 0);
    for (int h : height) {
        count[h]++;
        if (h > 1) count[h - 1]--;
    }

    int nodes = 0;
    for (int h = n, remaining = n - k; h && remaining > 0; h--) {
        int c = min(count[h], (remaining + h - 1) / h);
        nodes += c;
        remaining -= c * h;
    }
    cout << nodes;
}
