#include <bits/stdc++.h>
using namespace std;

struct Prediction {
    string pred;
    vector<int> common;

    bool operator<(const Prediction &p) const {
        return common < p.common;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, s;
    cin >> n >> s;

    vector<Prediction> sequences(s);
    for (auto &seq : sequences) {
        cin >> seq.pred;
        vector<int> pref(seq.pred.size());

        for (int j = 1, k = 0; j < seq.pred.size(); j++) {
            while (k && seq.pred[j] != seq.pred[k]) k = pref[k - 1];
            if (seq.pred[j] == seq.pred[k]) k++;
            pref[j] = k;
        }

        for (int j = pref.size() - 1; j; j = pref[j] - 1)
            if (2 * seq.pred.size() - pref[j] <= n) seq.common.emplace_back(pref[j]);
    }

    sort(sequences.begin(), sequences.end());
    for (auto &seq : sequences) cout << seq.pred << "\n";
}
