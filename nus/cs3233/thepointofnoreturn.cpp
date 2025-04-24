#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;

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

    FenwickTree(int n) : BIT(n, 0) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, Q;
    cin >> n >> Q;
    cin.ignore();

    vector<string> titles(n);
    for (auto &t : titles) {
        getline(cin, t);
        for (char &c : t)
            if (isupper(c)) c = tolower(c);
    }
    auto temp = titles;
    sort(temp.begin(), temp.end());
    temp.erase(unique(temp.begin(), temp.end()), temp.end());

    vector<int> rank(n);
    for (int i = 0; i < n; i++) rank[i] = lower_bound(temp.begin(), temp.end(), titles[i]) - temp.begin() + 1;
    
    vector<int> l(n);
    FenwickTree<int> fw(n + 1);
    for (int i = n - 1; ~i; i--) {
        l[i] = rank[i] - fw.pref_sum(rank[i]);
        fw.update(rank[i], 1);
    }

    vector<long long> diff(n + 1, 0), count(n + 1, 0), pref(n + 1, 0);
    for (int i = 0; i < n; i++) {
        diff[l[i]]++;
        diff[i + 1]--;
    }
    
    for (int i = 1; i <= n; i++) {
        count[i] = count[i - 1] + diff[i];
        pref[i] = pref[i - 1] + count[i];
    }

    vector<long long> answers(Q, -1);
    vector<vector<pair<long long, int>>> fatigues(n + 1);
    for (int q = 0; q < Q; q++) {
        long long f;
        cin >> f;

        if (f > pref[n]) answers[q] = -1;
        else {
            int i = lower_bound(pref.begin() + 1, pref.end(), f) - pref.begin();
            fatigues[i].emplace_back(f - pref[i - 1], q);
        }
    }

    vector<pair<int, int>> times(n);
    for (int i = 1; i <= n; i++) times[i - 1] = {l[i - 1], i};
    sort(times.begin(), times.end());

    auto total = 0LL;
    tree<int, null_type, less_equal<>, rb_tree_tag, tree_order_statistics_node_update> ost;
    for (int i = 1, j = 0; i <= n; i++) {
        while (j < times.size() && times[j].first <= i) {
            ost.insert(times[j].second);
            j++;
        }

        for (auto [f, q] : fatigues[i]) answers[q] = total + *ost.find_by_order(ost.order_of_key(i + 1) + f - 1) - i;
        total += n - i;
    }

    for (auto a : answers)
        if (a < 0) cout << "IMPOSSIBLE\n";
        else cout << a << "\n";
}
