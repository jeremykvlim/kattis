#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> w(n + 1), f(n + 1);
    for (int i = 1; i <= n; i++) cin >> w[i] >> f[i];

    auto two_opt = [&](auto &order) {
        auto eval = [&]() {
            int fun = 100, pos = 0;
            for (int i : order) {
                fun -= 8 * abs(i - pos);
                for (int _ = 0; _ < w[i]; _++) fun -= fun / 20;
                fun += f[i];
                pos = i;
            }
            return fun - 8 * pos;
        };

        int points = eval();
        bool change;
        do {
            change = false;
            for (int i = 0; i < n; i++)
                for (int j = i + 1; j < n; j++) {
                    reverse(order.begin() + i, order.begin() + j + 1);
                    int p = eval();
                    if (points < p) {
                        points = p;
                        change = true;
                    } else reverse(order.begin() + i, order.begin() + j + 1);
                }
        } while (change);

        return points;
    };

    int points = 0;
    vector<int> order(n), temp;
    iota(order.begin(), order.end(), 1);

    temp = order;
    points = max(points, two_opt(temp));

    reverse(temp.begin(), temp.end());
    points = max(points, two_opt(temp));

    temp = order;
    sort(temp.begin(), temp.end(), [&](int i, int j) { return f[i] < f[j]; });
    points = max(points, two_opt(temp));

    temp = order;
    sort(temp.begin(), temp.end(), [&](int i, int j) { return w[i] > w[j]; });
    points = max(points, two_opt(temp));
    cout << points;
}
