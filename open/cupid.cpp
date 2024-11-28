#include <bits/stdc++.h>
using namespace std;

struct QueryDecomposition {
    int size;
    vector<array<int, 3>> queries;

    QueryDecomposition(int n, int q) : size(ceil(sqrt(n))), queries(q) {
        read();
    }

    void read() {
        for (int j = 0; j < queries.size(); j++) {
            auto &[l, r, i] = queries[j];

            cin >> l >> r;
            i = j;
        }
    }

    vector<int> mo(vector<int> a, vector<int> b, int k) {
        vector<int> answers(queries.size());
        sort(queries.begin(), queries.end(), [&](auto q1, auto q2) {return make_pair(q1[0] / size, q1[1]) < make_pair(q2[0] / size, q2[1]);});

        int L = 0, R = -1, ans = 0;

        vector<int> c(k, 0), d(k, 0);
        auto add = [&](int i) {
            ans -= min(c[a[i]], d[a[i]]);
            c[a[i]]++;
            ans += min(c[a[i]], d[a[i]]);
            ans -= min(c[b[i]], d[b[i]]);
            d[b[i]]++;
            ans += min(c[b[i]], d[b[i]]);
        };

        auto remove = [&](int i) {
            ans -= min(c[a[i]], d[a[i]]);
            c[a[i]]--;
            ans += min(c[a[i]], d[a[i]]);
            ans -= min(c[b[i]], d[b[i]]);
            d[b[i]]--;
            ans += min(c[b[i]], d[b[i]]);
        };

        for (auto [l, r, i] : queries) {
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

    vector<int> a(n), b(n);
    for (int &ai : a) cin >> ai;
    for (int &bi : b) cin >> bi;

    QueryDecomposition qd(n, m);
    for (int couples : qd.mo(a, b, k)) cout << couples << "\n";
}
