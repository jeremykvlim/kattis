#include <bits/stdc++.h>
using namespace std;

struct Prediction {
    string s;
    int i;
    vector<int> common;

    bool operator<(const Prediction &s) const {
        return common == s.common ? i < s.i : common < s.common;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, s;
    cin >> n >> s;
    vector<Prediction> sequences(s);
    for (int i = 0; i < s; ++i) {
        cin >> sequences[i].s;
        sequences[i].i = i;
        vector<int> prefix(sequences[i].s.size());
        for (int j = 1, k = 0; j < sequences[i].s.size(); j++) {
            while (k && sequences[i].s[j] != sequences[i].s[k]) k = prefix[k - 1];
            if (sequences[i].s[j] == sequences[i].s[k]) k++;
            prefix[j] = k;
        }
        for (int pos = prefix.size() - 1; pos > 0; pos = prefix[pos] - 1)
            if (2 * sequences[i].s.size() - prefix[pos] <= n) sequences[i].common.push_back(prefix[pos]);
    }
    sort(sequences.begin(), sequences.end());
    for (auto &prediction : sequences) cout << prediction.s << '\n';
}
