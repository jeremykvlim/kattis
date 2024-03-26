#include <bits/stdc++.h>
using namespace std;

bool acceptable(string &w, string &p, int diff, int digits, int changes) {
    if (diff == digits) return digits > changes;
    if (!changes || diff - digits > 2 * (changes - digits)) return true;

    for (int i = 0; i < p.size() - 1; i++) {
        int transpose = (w[i + 1] == p[i]) + (w[i] == p[i + 1]) - (w[i] == p[i]) - (w[i + 1] == p[i + 1]);

        swap(p[i], p[i + 1]);
        if (!acceptable(w, p, diff - transpose, digits, changes - 1)) return false;
        else swap(p[i], p[i + 1]);
    }

    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<string> dictionary(n);
    for (auto &w : dictionary) cin >> w;

    string p;
    while (cin >> p) {
        for (auto &w : dictionary) {
            if (w.size() != p.size()) continue;

            int diff = 0, digits = 0;
            for (int i = 0; i < p.size(); i++) {
                if (p[i] != w[i]) diff++;
                if (isdigit(p[i])) digits++;
            }

            if (!acceptable(w, p, diff, digits, 3)) goto next;
        }

        cout << p << "\n";
        next:;
    }
}
