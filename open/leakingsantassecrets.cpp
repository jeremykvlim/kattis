#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    mt19937_64 rng(random_device{}());
    vector<int> q(n);
    iota(q.begin(), q.end(), 0);
    auto query = [&]() {
        for (int i = 0; i < n; i++) cout << q[i] + 1 << " ";
        cout << "\n" << flush;

        int delay;
        cin >> delay;

        if (!delay) exit(0);
        return delay;
    };

    int delay;
    for (;;) {
        shuffle(q.begin(), q.end(), rng);
        delay = query();
        if (delay < n) break;
    }

    int x, y;
    for (;;) {
        x = rng() % n;
        y = rng() % n;
        if (x == y) continue;
        swap(q[x], q[y]);
        int d = query();
        swap(q[x], q[y]);
        if (d == delay + 1) break;
    }

    vector<int> order{y};
    for (int i = 0; i < n; i++) {
        if (i == x || i == y) continue;

        auto before = [&](int a, int b) {
            swap(q[a], q[x]);
            swap(q[a], q[b]);
            int d = query();
            swap(q[a], q[b]);
            swap(q[a], q[x]);
            return d == delay;
        };

        int l = -1, r = order.size(), m;
        while (l + 1 < r) {
            m = l + (r - l) / 2;

            if (before(order[m], i)) r = m;
            else l = m;
        }

        if (!r && before(i, order[0])) {
            swap(q[i], q[x]);
            delay--;
            r = order.size();
        }
        order.insert(order.begin() + r, i);
    }
    order.insert(order.begin(), x);

    vector<int> temp(n);
    for (int i = 0; i < n; i++) temp[q[order[i]]] = order[(i + n - 1) % n];
    q = temp;
    query();
}
