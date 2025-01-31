#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    auto sum = 0LL;
    vector<long long> a(n);
    for (auto &ai : a) {
        cin >> ai;

        sum += ai;
    }

    auto meet_in_the_middle = [&](int l, int r) {
        vector<long long> subsets{0};
        for (int i = l; i < r; i++) {
            int s = subsets.size();
            for (int j = 0; j < s; j++) subsets.emplace_back(a[i] + subsets[j]);
        }
        sort(subsets.begin(), subsets.end());
        return subsets;
    };
    auto subsets_l = meet_in_the_middle(0, n / 2), subsets_r = meet_in_the_middle(n / 2, n);

    auto count = [&](auto s) {
        auto c = 0LL;
        for (int i = 0, j = subsets_r.size() - 1; i < subsets_l.size(); i++) {
            while (~j && subsets_l[i] + subsets_r[j] >= s) j--;
            if (!~j) break;
            c += j + 1;
        }
        return c;
    };
    long long l = 1, r = sum + 1, m, cl = count(l);
    while (l + 1 < r) {
        m = l + (r - l) / 2;

        auto cm = count(m);
        if (cm - cl > m - l) r = m;
        else {
            l = m;
            cl = cm;
        }
    }
    cout << l;
}
