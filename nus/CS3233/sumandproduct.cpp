#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> v{0}, numbers(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        cin >> numbers[i];
        
        if (numbers[i] > 1) v.emplace_back(i);
    }
    v.emplace_back(n + 1);

    int ways = 0;
    for (int i = 1; i < v.size() - 1; i++) {
        int sum = numbers[v[i]];
        long long product = numbers[v[i]];

        for (int j = i + 1; j < v.size() - 1; j++) {
            sum += numbers[v[j]] + v[j] - v[i] + i - j;
            product *= numbers[v[j]];

            if (product > INT_MAX) break;

            int l = v[i] - v[i - 1] - 1, r = v[j + 1] - v[j] - 1;
            if (sum == product) ways++;
            else if (sum < product && product <= sum + l + r) ways += sum + l + r + 1 - product - max(0LL, sum + l - product) - max(0LL, sum + r - product);
            sum -= v[j] - v[i] + i - j;
        }
    }

    cout << ways;
}
