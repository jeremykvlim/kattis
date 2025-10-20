#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    vector<T> BIT;

    void update(int i, T v) {
        for (; i && i < BIT.size(); i += i & -i) BIT[i] += v;
    }

    T pref_sum(int i) {
        T sum = 0;
        for (; i; i &= i - 1) sum += BIT[i];
        return sum;
    }

    T range_sum_query(int l, int r) {
        if (l >= r) return 0;
        return pref_sum(r) - pref_sum(l);
    }

    FenwickTree(int n) : BIT(n, 0) {}
};

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

    vector<vector<int>> indices(n);
    for (int i = 0; i < k; i++) indices[inspections[i]].emplace_back(i + 1);
    for (int i = 0; i < n; i++) indices[i].emplace_back(i + 1e7);

    FenwickTree<int> fw(k + 1);
    vector<bool> attend(k + 1, false);
    auto process = [&](int i, int value) {
        if (i <= k) {
            attend[i] = value == 1;
            fw.update(i, value);
        }
    };
    for (auto i : indices) process(i[0], 1);

    curr = 0;
    for (int i = 0; i < k; i++) {
        while (!attend[curr]) curr++;

        int next = *upper_bound(indices[inspections[curr - 1]].begin(), indices[inspections[curr - 1]].end(), curr);
        process(curr, -1);
        cout << fw.pref_sum(min(next, k)) + 1 << " ";
        process(next, 1);
    }
}
