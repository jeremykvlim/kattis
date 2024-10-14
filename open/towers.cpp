#include <bits/stdc++.h>
using namespace std;

double cmp(vector<int> &t1, int t1_end, vector<int> &t2, int t2_end) {
    double exponent = 1, scale = 1;
    for (int i = t2_end - 1; ~i; i--) {
        double a = i < t2_end ? t2[i] : 1, b = i < t1_end ? t1[i] : 1, ratio = scale * log(b) / log(a) - 1;
        scale = abs(ratio) < 1e-9 ? 1 : pow(a, exponent * ratio);

        if (scale > 50) return 2;
        else if (scale < 0.05) return 0;

        exponent = pow(a, exponent);
    }

    return scale;
}

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

        stringstream ss(tower);
        while (getline(ss, base, '^')) t.emplace_back(stoi(base));
    }
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
