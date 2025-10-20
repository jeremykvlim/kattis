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

    int t;
    cin >> t;

    while (t--) {
        int n;
        cin >> n;

        vector<int> a(n);
        for (int &ai : a) {
            cin >> ai;
            ai--;
        }

        vector<bool> seen(n, false);
        vector<int> indices(n, -1);
        int zl = n, zr = -1;
        for (int i = 0; i < n; i++)
            if (~a[i]) {
                seen[a[i]] = true;
                indices[a[i]] = i;
            } else {
                zl = min(zl, i);
                zr = max(zr, i);
            }

        int unseen = 0;
        queue<int> q;
        vector<int> missing;
        for (int i = 0; i < n; i++)
            if (!seen[i]) {
                unseen++;
                q.emplace(i);
                missing.emplace_back(i);
            }
        for (int i = 0; i < unseen; i++) missing.emplace_back(missing[i] + n);

        FenwickTree<int> fw(n);
        vector<long long> pref(unseen + 1, 0);
        auto w = 0LL;
        for (int i = 0, count = 0; i < n; i++) {
            int j;
            if (~a[i]) j = a[i];
            else {
                j = q.front();
                q.pop();
            }
            w += i - fw.pref_sum(j);
            fw.update(j + 1, 1);

            if (!~a[i]) count++;
            else if (0 < count && count < unseen) {
                int x = lower_bound(missing.begin(), missing.end(), a[i]) - missing.begin(),
                    l = max(count, x), r = count + x;
                pref[l - count]++;
                pref[r - count]--;

                l = count;
                r = x;
                if (l < r) {
                    pref[l - count]--;
                    pref[r - count]++;
                }

                int y = lower_bound(missing.begin(), missing.end(), a[i] + n) - missing.begin();
                l = max(count + x, y);
                r = count + unseen;
                if (l < r) {
                    pref[l - count]++;
                    pref[r - count]--;
                }

                l = count + x;
                r = min(r, y);
                if (l < r) {
                    pref[l - count]--;
                    pref[r - count]++;
                }
            }
        }

        int z = zl + zr;
        vector<long long> p(n, 0);
        for (int i = 0; i < unseen; i++) {
            pref[i + 1] += pref[i];
            p[missing[i]] = pref[i] - z;
        }

        auto weight = w;
        for (int i = 0; i < n; i++) {
            w += n - 1;
            if (seen[i]) w -= 2 * indices[i];
            else w += p[i];

            weight = min(weight, w);
        }
        cout << weight << "\n";
    }
}
