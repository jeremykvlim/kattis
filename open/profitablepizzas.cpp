#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> delivs(n);
    for (auto &[t, d] : delivs) cin >> t >> d;
    sort(delivs.begin(), delivs.end(), greater<>());

    auto money = 0LL;
    multiset<int> payments;
    int curr;

    auto deliver = [&](int t) {
        while (!payments.empty() && curr > t) {
            money += *payments.rbegin();
            payments.erase(prev(payments.end()));
            curr--;
        }
    };

    for (auto [t, d] : delivs) {
        deliver(t);
        payments.emplace(d);
        curr = t;
    }
    deliver(0);

    cout << money;
}
