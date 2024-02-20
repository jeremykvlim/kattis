#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, M;
    cin >> n >> M;

    vector<pair<int, int>> books(n);
    for (auto &[d, l] : books) cin >> l >> d;
    sort(books.begin(), books.end());

    long long l = -1, r = 1e14, m;
    while (l + 1 < r) {
        m = (l + r) / 2;

        auto pages = 0LL;
        int late = 0;
        priority_queue <int> pq;
        for (auto [d, l] : books) {
            pages += l;
            pq.emplace(l);

            if (pages > m * d) {
                pages -= pq.top();
                pq.pop();
                late++;
            }
        }

        if (late <= M) r = m;
        else l = m;
    }

    cout << l + 1;
}
