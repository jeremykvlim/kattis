#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int r, s, n;
    cin >> r >> s >> n;

    int sum_a = 0, sum_b = 0;
    vector<int> a(r + 1, 0), b(s + 1, 0), c(n);
    for (int i = 0; i < r; i++) {
        int ai;
        cin >> ai;

        a[i + 1] = a[i] + ai;
        sum_a += ai;
    }

    for (int i = 0; i < s; i++) {
        int bi;
        cin >> bi;

        b[i + 1] = b[i] + bi;
        sum_b += bi;
    }

    for (int &ci : c) cin >> ci;

    vector<bool> capacities(2e6 + 1, false);
    capacities[0] = true;
    for (int i = 1; i <= 2e6; i++) capacities[i] = (i >= sum_a && capacities[i - sum_a]) || (i >= sum_b && capacities[i - sum_b]);

    vector<pair<int, int>> p;
    for (int i = 0; i < r; i++)
        for (int j = 0; j < s; j++) p.emplace_back(i, j);

    while (n--) {
        vector<pair<int, int>> temp;
        for (int k = 0; k < r; k++)
            for (int l = 0; l < s; l++)
                for (auto [i, j] : p) {
                    int A = a[i] - a[k], B = b[j] - b[l];
                    if (i < k) A += sum_a;
                    if (j < l) B += sum_b;

                    if (c[n] >= A + B && capacities[c[n] - A - B]) {
                        temp.emplace_back(k, l);
                        break;
                    }
                }

        p = temp;
    }
    
    if (!p.empty() && p[0] == make_pair(0, 0)) {
        cout << "Yes";
        exit(0);
    }
    
    cout << "No";
}
