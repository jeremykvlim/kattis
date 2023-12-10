#include <bits/stdc++.h>
using namespace std;

void update(int i, int val, int inspections, vector<int> &fenwick, vector<bool> &continuous) {
    if (i >= inspections) return;
    continuous[i] = val != -1;
    while (i < fenwick.size()) {
        fenwick[i] += val;
        i = i | (i + 1);
    }
}

int pref(int i, int inspections, vector<int> &fenwick) {
    if (i >= inspections) i = inspections - 1;
    int sum = 0;
    while (i != -1) {
        sum += fenwick[i];
        i = (i & (i + 1)) - 1;
    }
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
        else while (!inspected[a - 1]) {
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

    for (int i = 0; i < n; i++) update(index[i][0], 1, k, fenwick, attend);

    curr = 0;
    for (int i = 0; i != k; i++) {
        while (!attend[curr]) curr++;
        int next = *upper_bound(index[inspections[curr]].begin(), index[inspections[curr]].end(), curr);
        update(curr, -1, k, fenwick, attend);
        cout << pref(next, k, fenwick) + 1 << " ";
        update(next, 1, k, fenwick, attend);
    }
}
