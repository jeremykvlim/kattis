#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> p(n);
    iota(p.begin(), p.end(), 1);
    shuffle(p.begin(), p.end(), mt19937(random_device{}()));

    int i = p[0], a_max = 0;
    vector<int> b(n + 1);
    unordered_map<int, int> seen;
    for (int k = 1; k < n; k++) {
        int j = p[k];
        cout << "? " << i << " " << j << "\n" << flush;

        int a;
        cin >> a;

        if (seen.count(a)) {
            b[i] = a;
            b[seen[a]] = 0;
            i = seen[a];
            seen.clear();
            cout << "? " << i << " " << j << "\n" << flush;

            cin >> a;
            a_max = a;
        }

        a_max = max(a_max, a);
        b[p[k]] = a;
        seen[a] = p[k];
    }
    b[i] = a_max;
    cout << "! ";
    for (int k = 1; k <= n; k++) cout << b[k] << " ";
    cout << "\n" << flush;
}
