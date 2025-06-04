#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> x(n);
    for (int &xi : x) {
        cin >> xi;
        xi--;
    }

    vector<int> f(m), degree(m, 0);
    for (int &fi : f) {
        cin >> fi;
        fi--;

        degree[fi]++;
    }

    int zeros = count_if(degree.begin(), degree.end(), [&](int d) { return !d; });
    if (zeros > 1) {
        cout << -1;
        exit(0);
    }

    if (zeros == 1) {
        int i = find_if(degree.begin(), degree.end(), [&](int d) { return !d; }) - degree.begin();

        for (int xi : x)
            if (xi != i) {
                cout << -1;
                exit(0);
            }

        int count = 0;
        vector<bool> visited(m, false);
        while (!visited[i]) {
            visited[i] = true;
            i = f[i];
            count++;
        }

        if (count != m) {
            cout << -1;
            exit(0);
        }

        while (n--) cout << m << " ";
        exit(0);
    }

    vector<vector<int>> cycles;
    vector<pair<int, int>> cycle_indices(m, {-1, -1});
    for (int i = 0; i < m; i++) {
        if (cycle_indices[i] != make_pair(-1, -1)) continue;

        int c = cycles.size();
        vector<int> cycle;
        int j = i;
        while (cycle_indices[j] == make_pair(-1, -1)) {
            cycle_indices[j] = {c, cycle.size()};
            cycle.emplace_back(j);
            j = f[j];
        }
        cycles.emplace_back(cycle);
    }

    auto step = [&](int i, int steps) {
        auto [c, j] = cycle_indices[i];

        int s = cycles[c].size(), k = (j + steps) % s;
        if (k < 0) k += (s - k - 1) / s * s;
        return cycles[c][k];
    };

    int count = 0;
    vector<int> fx_pow_i(n), fx_pows;
    vector<queue<int>> times(m);
    vector<bool> visited(m, false);
    for (int i = 0; i < n; i++) {
        fx_pow_i[i] = step(x[i], i);
        if (!visited[fx_pow_i[i]]) {
            visited[fx_pow_i[i]] = true;
            fx_pows.emplace_back(fx_pow_i[i]);
            count++;
        }
        times[fx_pow_i[i]].emplace(i + 1);
    }

    vector<long long> t0(m);
    priority_queue<pair<long long, int>> pq;
    for (int fx_pow : fx_pows) {
        int t = times[fx_pow].front();

        t0[fx_pow] = t;
        pq.emplace(t0[fx_pow], fx_pow);

        for (int fx_pow_step_n = step(fx_pow, n), temp = count; !visited[fx_pow_step_n]; fx_pow_step_n = step(fx_pow_step_n, n)) {
            visited[fx_pow_step_n] = true;
            count++;
            t0[fx_pow_step_n] = (long long) n * (count - temp) + t;
            pq.emplace(t0[fx_pow_step_n], fx_pow_step_n);
        }
    }

    if (count != m) {
        cout << -1;
        exit(0);
    }

    auto t1 = t0;
    for (int i = 0; i < n; i++) {
        while (pq.top().first != t1[pq.top().second]) pq.pop();
        auto [t, fx_pow] = pq.top();
        cout << t - i << " ";

        times[fx_pow_i[i]].pop();
        t1[fx_pow_i[i]] = !times[fx_pow_i[i]].empty() ? times[fx_pow_i[i]].front() : (long long) n + t0[step(fx_pow_i[i], -n)];
        pq.emplace(t1[fx_pow_i[i]], fx_pow_i[i]);
    }
}
