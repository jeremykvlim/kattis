#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int XOR = 0;
    vector<int> a;
    for (int i = 0; i < 15; i++) {
        string s(n, '0');
        for (int j = 0; j < n; j++) s[j] += (j >> i) & 1;
        cout << "? " << s << "\n" << flush;

        int L;
        cin >> L;

        if (!(L & 1)) continue;

        XOR |= 1 << i;
        if (a.empty())
            for (int j = 0; j < n; j++)
                if (s[j] == '0') a.emplace_back(j);
    }

    int l = -1, r = a.size() - 1, m;
    while (l + 1 < r) {
        m = l + (r - l) / 2;

        string s(n, '0');
        for (int k = l + 1; k <= m; k++) s[a[k]]++;
        cout << "? " << s << "\n" << flush;

        int L;
        cin >> L;

        if (L & 1) r = m;
        else l = m;
    }
    cout << "! " << a[r] << " " << (a[r] ^ XOR) << flush;
}
