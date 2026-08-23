#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    vector<int> start(n), prev(n + 1, -1);
    vector<vector<int>> indices(n + 1);
    for (int i = 0; i < n; i++) {
        start[i] = max(i ? start[i - 1] : 0, prev[a[i]] + 1);
        prev[a[i]] = i;
        indices[a[i]].emplace_back(i);
    }

    vector<int> link(n);
    for (int l = 0, r = -1; l < n; l++) {
        for (; r + 1 < n && start[r + 1] <= l; r++);
        link[l] = r;
    }

    vector<int> size(n), len(n + 1);
    vector<array<int, 3>> queries;
    vector<vector<int>> query_indices(n);
    for (int ai = 1; ai <= n; ai++) {
        for (int i : indices[ai]) {
            int l = i, r = i;
            if (i && size[i - 1]) l = i - size[i - 1];
            if (i + 1 < n && size[i + 1]) r = i + size[i + 1];
            size[l] = size[r] = r - l + 1;

            len[i] = min(link[l], r) - l + 1;
            if (link[l] < r) {
                queries.push_back({r, i, 0});
                query_indices[link[l] + 1].emplace_back(queries.size() - 1);
            }
        }
    }

    stack<int> s;
    for (int i = 0; i < n; i++) {
        reverse(query_indices[i].begin(), query_indices[i].end());
        for (int q : query_indices[i]) s.emplace(q);

        if (!s.empty()) queries[s.top()][2] = max(queries[s.top()][2], i - start[i] + 1);
        while (!s.empty() && queries[s.top()][0] == i) {
            int q = s.top();
            s.pop();

            len[queries[q][1]] = max(len[queries[q][1]], queries[q][2]);
            if (!s.empty()) queries[s.top()][2] = max(queries[s.top()][2], queries[q][2]);
        }
    }

    vector<bool> valid(n + 1);
    for (int i = 0; i < n; i++)
        if (len[i] >= a[i]) valid[a[i]] = true;

    vector<int> ks;
    for (int k = 1; k <= n; k++)
        if (valid[k]) ks.emplace_back(k);

    cout << ks.size() << "\n";
    for (int k : ks) cout << k << " ";
}
