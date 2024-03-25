#include <bits/stdc++.h>
using namespace std;

constexpr int POP = 1e7;

double probability(vector<double> &pref_log, int k, int i, bool result) {
    if (result) return log(1 - exp(probability(pref_log, k, i, false)));
    return pref_log[POP - i] - pref_log[POP - i - k] - pref_log[POP] + pref_log[POP - k];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<double> p(5e6 + 1, 0), pref_log(POP + 1, 0);
    for (int i = 1; i <= POP; i++) pref_log[i] = pref_log[i - 1] + log(i);
    int t = 50, x = 100;
    while (t--) {
        int k = POP / x;
        cout << "test " << k << "\n" << flush;

        bool result;
        cin >> result;

        double P = -1e15;
        for (int i = 100; i <= 5e6; i *= 1.1) {
            p[i] += probability(pref_log, k, i, result);
            if (P < p[i]) {
                P = p[i];
                x = i;
            }
        }
    }

    cout << "estimate " << x << "\n";
}
