#include <bits/stdc++.h>
using namespace std;

template <typename I>
void balanced_shuffle(I l, I r) {
    if (l == r) return;

    using T = typename iterator_traits<I>::value_type;

    unordered_map<T, int> freq;
    for (auto it = l; it < r; it++) freq[*it]++;

    priority_queue pq([](const auto &t1, const auto &t2) -> bool { return get<4>(t1) != get<4>(t2) ? get<4>(t1) > get<4>(t2) : get<0>(t1) > get<0>(t2); },
                      vector<tuple<T, int, int, double, double>>());
    
    int n = distance(l, r);
    for (auto [e, f] : freq) pq.emplace(e, f, 0, (double) n / f, 0);

    for (auto it = l; it < r; it++) {
        auto [e, f, count, step, next_pos] = pq.top();
        pq.pop();

        *it = e;

        if (f > ++count) pq.emplace(e, f, count, step, next_pos + step);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, x;
    cin >> n >> x;

    int t = 0;
    vector<int> exposed;
    while (n--) {
        int ti;
        cin >> ti;

        t = max(t, ti);
        if (ti <= x) exposed.emplace_back(x - ti + 1);
    }

    if (exposed.size() <= 2) {
        cout << t;
        exit(0);
    }

    if (exposed.size() <= 4) {
        cout << max(t, x + 1);
        exit(0);
    }

    sort(exposed.begin(), exposed.end());
    for (int i = 0; i < 4; i++) exposed.pop_back();
    balanced_shuffle(exposed.begin(), exposed.end());

    const int s = 1e6, h = 5e5;
    int sum = 0;
    bitset<s> bs;
    bs[h] = true;
    for (int e : exposed) {
        sum += e;
        bs = (bs << e) | (bs >> e);
    }

    int time = INT_MAX;
    for (int i = h; i < s; i++)
        if (bs[i]) time = min(time, max((sum + i - h) / 2, (sum - i + h) / 2) + x + 1);

    cout << max(t, time);
}
