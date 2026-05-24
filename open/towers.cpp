#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m;
    cin >> m;
    cin.ignore();

    vector<vector<int>> towers(m);
    for (auto &t : towers) {
        string tower, base;
        cin >> tower;

        istringstream iss(tower);
        while (getline(iss, base, '^')) t.emplace_back(stoi(base));
    }

    auto cmp = [&](vector<int> &t1, int t1_end, vector<int> &t2, int t2_end) -> double {
        double exponent = 1, ratio = 1;
        for (int i = t2_end - 1; ~i; i--) {
            auto a = i < t2_end ? t2[i] : 1., b = i < t1_end ? t1[i] : 1., delta = ratio * log(b) - log(a);
            ratio = fabs(delta) <= 1e-13 * max(fabs(ratio * log(b)), fabs(log(a))) ? 1 : exp(delta * exponent);

            if (ratio > 9) return 2;
            else if (ratio * 9 < 1) return 0;

            exponent = pow(a, exponent);
        }

        return ratio;
    };

    stable_sort(towers.begin(), towers.end(), [&](auto t1, auto t2) {
        int t1_end = find(t1.begin(), t1.end(), 1) - t1.begin(), t2_end = find(t2.begin(), t2.end(), 1) - t2.begin();
        return t1_end < t2_end ? cmp(t1, t1_end, t2, t2_end) < 1 : cmp(t2, t2_end, t1, t1_end) > 1;
    });

    cout << "Case 1:\n";
    for (auto &t : towers) {
        for (int i = 0; i < t.size(); i++) cout << (i ? "^" : "") << t[i];
        cout << "\n";
    }
}
