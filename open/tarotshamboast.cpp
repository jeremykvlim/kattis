#include <bits/stdc++.h>
using namespace std;

struct Prediction {
    string p;
    vector<int> len;

    bool operator<(const Prediction &pred) const {
        return len < pred.len;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, s;
    cin >> n >> s;

    vector<Prediction> preds(s);
    for (auto &[p, len] : preds) {
        cin >> p;

        vector<int> pref(p.size());
        for (int j = 1, k = 0; j < p.size(); j++) {
            while (k && p[j] != p[k]) k = pref[k - 1];
            if (p[j] == p[k]) k++;
            pref[j] = k;
        }

        for (int j = pref.size() - 1; j > 0; j = pref[j] - 1)
            if (2 * p.size() - pref[j] <= n) len.emplace_back(pref[j]);
    }

    sort(preds.begin(), preds.end());
    for (auto &[p, len] : preds) cout << p << "\n";
}
