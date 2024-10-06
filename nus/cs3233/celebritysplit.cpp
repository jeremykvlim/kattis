#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<int> mansions(n);
        for (int &v : mansions) cin >> v;
        sort(mansions.rbegin(), mansions.rend());

        vector<int> suff(n + 1, 0);
        for (int i = n - 1; ~i; i--) suff[i] = suff[i + 1] + mansions[i];

        int value = INT_MAX;
        auto dfs = [&](auto &&self, int depth = 0, int jack = 0, int jill = 0, int sold = 0) -> void {
            if (sold >= value || abs(jack - jill) > suff[depth]) return;
            if (jack == jill) value = min(value, sold + suff[depth]);
            if (depth == n) return;

            self(self, depth + 1, jack + mansions[depth], jill, sold);
            if (jack != jill && jack) self(self, depth + 1, jack, jill + mansions[depth], sold);
            self(self, depth + 1, jack, jill, sold + mansions[depth]);
        };
        dfs(dfs);
        cout << value << "\n";
    }
}
