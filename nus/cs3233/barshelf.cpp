#include <bits/stdc++.h>
using namespace std;

void update(int i, long long value, vector<long long> &fenwick) {
    for (; i < fenwick.size(); i += i & -i) fenwick[i] += value;
}

long long pref_sum(int i, vector<long long> &fenwick) {
    auto sum = 0LL;
    for (; i; i &= (i - 1)) sum += fenwick[i];
    return sum;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    map<int, int> indices;
    vector<int> h(n);
    for (int &hi : h) {
        cin >> hi;

        indices[hi];
    }

    int unique = 0;
    for (auto &[hi, i] : indices) i = unique++;
    indices[INT_MAX] = unique;

    vector<long long> fenwick1(unique + 1), fenwick2(unique + 1);
    auto messy = 0LL;
    for (int i = n - 1; ~i; i--) {
        int j = indices[h[i]], k = indices.upper_bound(h[i] / 2)->second;

        auto ps1 = pref_sum(k, fenwick1), ps2 = pref_sum(k, fenwick2);
        update(j + 1, 1, fenwick1);
        update(j + 1, ps1, fenwick2);
        messy += ps2;
    }

    cout << messy;
}
