#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    while (cin >> n >> m && n && m) {
        int l, k;
        cin >> l >> k;
        
        vector<int> x(k);
        for (int &xi : x) cin >> xi;
        
        auto calc = [&](int d, int rows) -> int {
            if (!rows) return 0;
            
            int same1 = 0;
            vector<int> v;
            for (int xi : x)
                if (xi == d) same1++;
                else if (xi < d) v.emplace_back(xi);
            if (same1 >= rows) return rows;
            
            sort(v.begin(), v.end());
            int same2 = 0;
            for (int i = 0, j = v.size() - 1; i < j;) {
                int s = v[i] + v[j];
                if (s == d) {
                    same2++;
                    i++;
                    j--;
                } else if (s < d) i++;
                else j--;
            }
            if (same1 + same2 < rows) return 1e9;
            return same1 + 2 * (rows - same1);
        };
        int planks = 1e9;
        if (!((m * 100) % l)) planks = min(planks, calc(n, (m * 100) / l));
        if (!((n * 100) % l)) planks = min(planks, calc(m, (n * 100) / l));
        
        if (planks == 1e9) cout << "The Yule Ball can’t happen, Draco’s father will hear about this\n";
        else cout << planks << "\n";
    }
}
