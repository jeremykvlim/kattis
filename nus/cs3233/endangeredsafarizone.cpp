#include <bits/stdc++.h>
using namespace std;

struct QueryDecomposition {
    int size;
    vector<array<int, 4>> queries;

    QueryDecomposition(int n, const vector<array<int, 4>> &queries) : size(cbrt(n) * cbrt(n)), queries(queries) {}

    vector<int> mo(vector<int> a, const vector<array<int, 3>> &updates, int k) {
        vector<int> answers(queries.size()), freq(a.size() + 1, 0);
        sort(queries.begin(), queries.end(), [&](auto q1, auto q2) { return make_tuple(q1[0] / size, q1[1] / size, q1[2]) < make_tuple(q2[0] / size, q2[1] / size, q2[2]); });

        int L = 0, R = -1, T = 0, ans = 0;

        auto update = [&](int i) {
            auto [A, B, prev] = updates[i];
            if (L <= A && A <= R) {
                int p = a[A];
                if (freq[p] == 1) ans--;
                freq[p]--;
                if (freq[p] == k) ans++;
                p = B;
                if (!freq[p]) ans++;
                freq[p]++;
                if (freq[p] == k + 1) ans--;
            }
            a[A] = B;
        };

        auto revert = [&](int i) {
            auto [A, B, prev] = updates[i];
            if (L <= A && A <= R) {
                int p = a[A];
                if (freq[p] == 1) ans--;
                freq[p]--;
                if (freq[p] == k) ans++;
                p = prev;
                if (!freq[p]) ans++;
                freq[p]++;
                if (freq[p] == k + 1) ans--;
            }
            a[A] = prev;
        };

        auto add = [&](int i) {
            int p = a[i];
            if (!freq[p]) ans++;
            freq[p]++;
            if (freq[p] == k + 1) ans--;
        };

        auto remove = [&](int i) {
            int p = a[i];
            if (freq[p] == k + 1) ans++;
            freq[p]--;
            if (!freq[p]) ans--;
        };

        for (auto [l, r, t, i] : queries) {
            while (T < t) update(T++);
            while (T > t) revert(--T);
            while (L > l) add(--L);
            while (R < r) add(++R);
            while (L < l) remove(L++);
            while (R > r) remove(R--);
            answers[i] = ans;
        }

        return answers;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<int> p(n);
    for (int &pi : p) {
        cin >> pi;
        pi--;
    }

    auto temp = p;
    vector<array<int, 4>> queries;
    vector<array<int, 3>> updates;
    while (m--) {
        int q, a, b;
        cin >> q >> a >> b;
        a--;
        b--;

        if (q == 1) {
            updates.push_back({a, b, temp[a]});
            temp[a] = b;
        } else queries.push_back({a, b, (int) updates.size(), (int) queries.size()});
    }

    QueryDecomposition qd(n, queries);
    for (int count : qd.mo(p, updates, k)) cout << count << "\n";
}
