#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, n;
    cin >> m >> n;

    vector<pair<int, int>> producer(m), consumer(n);
    for (auto &[p, d] : producer) cin >> p >> d;
    for (auto &[q, e] : consumer) cin >> q >> e;

    sort(producer.begin(), producer.end());
    vector<pair<int, int>> choose;
    for (int i = 0, j; i < m; i = j) {
        choose.emplace_back(producer[i]);
        for (j = i; j < m && producer[j].second >= producer[i].second; j++);
    }
    producer = choose;
    choose.clear();

    sort(consumer.rbegin(), consumer.rend());
    for (int i = 0, j; i < n; i = j) {
        choose.emplace_back(consumer[i]);
        for (j = i; j < n && consumer[j].second <= consumer[i].second; j++);
    }
    consumer = choose;
    reverse(consumer.begin(), consumer.end());

    auto dnc = [&](auto &&self, int pl, int pr, int cl, int cr) -> long long {
        if (pl >= pr) return 0;

        auto profit = LLONG_MIN;
        int pm = pl + (pr - pl) / 2, cm = cl;
        for (int i = cl; i <= cr; i++) {
            auto [p, d] = producer[pm];
            auto [q, e] = consumer[i];
            int days = e - d, price = q - p;
            auto cost = !days || price <= 0 ? (long long) -abs(days) * abs(price) : (long long) days * price;
            if (cost >= profit) {
                profit = cost;
                cm = i;
            }
        }

        if (pl + 1 < pr) profit = max({profit, self(self, pl, pm, cl, cm), self(self, pm, pr, cm, cr)});
        return profit;
    };
    cout << max(0LL, dnc(dnc, 0, producer.size(), 0, consumer.size()));
}
