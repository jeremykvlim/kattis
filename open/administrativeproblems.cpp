#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--) {
        int n, m;
        cin >> n >> m;

        vector<string> N(n + 1);
        vector<int> p(n + 1), q(n + 1), k(n + 1);
        unordered_map<string, int> car;
        for (int i = 1; i <= n; i++) {
            cin >> N[i] >> p[i] >> q[i] >> k[i];

            car[N[i]] = i;
        }

        map<string, int> spy;
        vector<int> pick(m, 0), cost(m, 0);
        vector<bool> inconsistent(m, false);
        while (m--) {
            int t;
            string S;
            char e;
            cin >> t >> S >> e;

            if (!spy.count(S)) spy[S] = spy.size();
            int i = spy[S];
            if (e == 'p') {
                string C;
                cin >> C;

                if (pick[i]) inconsistent[i] = true;
                pick[i] = car[C];
                cost[i] += q[pick[i]];
            } else if (e == 'r') {
                int d;
                cin >> d;

                if (!pick[i]) inconsistent[i] = true;
                cost[i] += d * k[pick[i]];
                pick[i] = 0;
            } else {
                int s;
                cin >> s;

                if (!pick[i]) inconsistent[i] = true;
                if (s) cost[i] += (p[pick[i]] * s - 1) / 100 + 1;
            }
        }
        for (auto [name, i] : spy) {
            cout << name << " ";

            if (inconsistent[i] | pick[i]) cout << "INCONSISTENT\n";
            else cout << cost[i] << "\n";
        }
    }
}
