#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t, n;
    cin >> t >> n;

    vector<bool> l(t, false), r(t, false);
    int l_count = 0, r_count = 0;
    while (n--) {
        int ti;
        char c;
        cin >> ti >> c;

        if (c == 'L') {
            l_count++;
            l[ti - 1] = true;
        } else {
            r_count++;
            r[ti - 1] = true;
        }
    }

    int l_only = 0, r_only = 0, both = 0;
    for (int i = 0; i < t; i++)
        if (l[i] && r[i]) both++;
        else if (l[i]) l_only++;
        else if (r[i]) r_only++;
        else {
            cout << -1;
            exit(0);
        }

    int l_done = l_only, r_done = r_only;
    while (both || l_done != r_done) {
        if (l_done < r_done) {
            if (++l_done > l_count) {
                cout << -1;
                exit(0);
            }
        } else {
            if (++r_done > r_count) {
                cout << -1;
                exit(0);
            }
        }

        if (both) both--;
    }

    cout << l_done + r_done;
}
