#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<int> costs(n);
        int total = 0;
        for (int &c : costs) {
            double spent;
            cin >> spent;

            c = round(100 * spent);
            total += c;
        }

        sort(costs.rbegin(), costs.rend());
        int remaining = total % n;
        total /= n;
        
        int money = 0;
        for (int i = 0; i < n; i++) money += i < remaining ? abs(costs[i] - total - 1) : abs(costs[i] - total);
        cout << "$" << fixed << setprecision(2) << money / 200. << "\n";
    }
}
