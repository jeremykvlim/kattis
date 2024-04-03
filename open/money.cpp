#include <bits/stdc++.h>
using namespace std;

long long dnc(int l, int r, int start, int end, vector<pair<int, int>> &producer, vector<pair<int, int>> &consumer) {
    if (l > r) return 0;
    
    int m = l + (r - l) / 2, best = start;
    auto profit = LLONG_MIN;
    for (int i = start; i <= end; i++) {
        int duration = consumer[i].second - producer[m].second, price = consumer[i].first - producer[m].first;
        auto cost = !duration || price <= 0 ? (long long) -abs(duration) * abs(price) : (long long) duration * price;
        if (cost >= profit) {
            profit = cost;
            best = i;
        }
    }

    if (l < r) profit = max({profit, dnc(l, m - 1, start, best, producer, consumer), dnc(m + 1, r, best, end, producer, consumer)});

    return profit;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, n;
    cin >> m >> n;

    vector<pair<int, int>> producer(m), consumer(n);
    for (auto &[p, d] : producer) cin >> p >> d;
    for (auto &[q, e] : consumer) cin >> q >> e;

    sort(producer.begin(), producer.end());
    vector<pair<int, int>> selection;
    for (int i = 0, j; i < m; i = j) {
        selection.emplace_back(producer[i]);
        for (j = i; j < m && producer[j].second >= producer[i].second; j++);
    }
    producer.swap(selection);
    selection.clear();

    sort(consumer.rbegin(), consumer.rend());
    for (int i = 0, j; i < n; i = j) {
        selection.emplace_back(consumer[i]);
        for (j = i; j < n && consumer[j].second <= consumer[i].second; j++);
    }
    consumer.swap(selection);
    reverse(consumer.begin(), consumer.end());

    cout << max(0LL, dnc(0, producer.size() - 1, 0, consumer.size() - 1, producer, consumer));
}
