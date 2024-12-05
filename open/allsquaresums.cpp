#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;

    if (!n) {
        cout << "1\n0^2 + 0^2 = 0";
        exit(0);
    }

    vector<pair<int, int>> pairs;
    int l = 0, r = ceil(sqrtl(n));
    while (l <= r) {
        auto s = (long long) l * l + (long long) r * r;

        if (s == n) pairs.emplace_back(l++, r--);
        else if (s < n) l++;
        else r--;
    }
    
    unordered_set<string> unique;
    for (auto [x, y] : pairs) {
        unique.emplace(to_string(x) + "^2 + " + to_string(y) + "^2 = " + to_string(n));
        unique.emplace(to_string(y) + "^2 + " + to_string(x) + "^2 = " + to_string(n));

        auto pos_x_neg_y = [&]() {
            unique.emplace(to_string(x) + "^2 + (-" + to_string(y) + ")^2 = " + to_string(n));
            unique.emplace("(-" + to_string(y) + ")^2 + " + to_string(x) + "^2 = " + to_string(n));
        };

        auto pos_y_neg_x = [&]() {
            unique.emplace("(-" + to_string(x) + ")^2 + " + to_string(y) + "^2 = " + to_string(n));
            unique.emplace(to_string(y) + "^2 + (-" + to_string(x) + ")^2 = " + to_string(n));
        };

        if (!x) pos_x_neg_y();
        else if (!y) pos_y_neg_x();
        else {
            pos_x_neg_y();
            pos_y_neg_x();
            unique.emplace("(-" + to_string(x) + ")^2 + (-" + to_string(y) + ")^2 = " + to_string(n));
            unique.emplace("(-" + to_string(y) + ")^2 + (-" + to_string(x) + ")^2 = " + to_string(n));
        }
    }

    cout << unique.size() << "\n";
    for (auto s : unique) cout << s << "\n";
}
