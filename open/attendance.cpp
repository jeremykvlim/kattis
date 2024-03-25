#include <bits/stdc++.h>
using namespace std;

void update(int i, int value, vector<int> &fenwick, int inspections, vector<bool> &attend) {
    if (i >= inspections) return;

    attend[i] = value != -1;
    for (; i < fenwick.size(); i = i | (i + 1)) fenwick[i] += value;
}

int pref_sum(int i, vector<int> &fenwick, int inspections) {
    if (i >= inspections) i = inspections - 1;

    int sum = 0;
    for (; ~i; i = (i & (i + 1)) - 1) sum += fenwick[i];
    return sum;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> students(m), order(n);
    for (int &a : students) cin >> a;
    for (int &b : order) cin >> b;

    vector<bool> inspected(n);
    vector<int> inspections;
    int curr = 0;
    for (int a : students)
        if (inspected[a - 1]) inspections.emplace_back(a - 1);
        else
            while (!inspected[a - 1]) {
                inspected[order[curr] - 1] = true;
                inspections.emplace_back(order[curr++] - 1);
            }

    int k = inspections.size();
    cout << k << "\n";

    vector<vector<int>> index(n);
    for (int i = 0; i < k; i++) index[inspections[i]].emplace_back(i);
    for (int i = 0; i < n; i++) index[i].emplace_back(INT_MAX - i);

    vector<int> fenwick(k);
    vector<bool> attend(k, false);

    for (int i = 0; i < n; i++) update(index[i][0], 1, fenwick, k, attend);

    curr = 0;
    for (int i = 0; i < k; i++) {
        while (!attend[curr]) curr++;

        int next = *upper_bound(index[inspections[curr]].begin(), index[inspections[curr]].end(), curr);
        update(curr, -1, fenwick, k, attend);
        cout << pref_sum(next, fenwick, k) + 1 << " ";
        update(next, 1, fenwick, k, attend);
    }
}
