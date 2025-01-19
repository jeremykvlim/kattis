#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> candidates;
    for (int x = 100; x <= 5e6; x *= 1.01) candidates.emplace_back(x);

    int n = candidates.size(), pop = 1e7;
    vector<double> distribution(n, 1. / n), log_fact(pop + 1, 0);
    for (int i = 1; i <= pop; i++) log_fact[i] = log_fact[i - 1] + log(i);

    for (int _ = 0; _ < 50; _++) {
        vector<double> prob(n);
        auto pr = [&](int k) {
            double prob_positive = 0;
            for (int i = 0; i < n; i++) {
                if (pop - candidates[i] >= k) prob[i] = 1 - exp(log_fact[pop - candidates[i]] - log_fact[pop - candidates[i] - k] - log_fact[pop] + log_fact[pop - k]);
                else prob[i] = 1;
                
                prob_positive += prob[i] * distribution[i];
            }
            return prob_positive;
        };
        int l = 0, r = pop;
        while (l + 1 < r) {
            int m = l + (r - l) / 2;

            if (pr(m) >= 0.5) r = m;
            else l = m;
        }
        cout << "test " << l << "\n" << flush;

        bool y;
        cin >> y;

        auto prob_positive = pr(l);
        for (int i = 0; i < n; i++) distribution[i] *= (y ? prob[i] / prob_positive : (1 - prob[i]) / (1 - prob_positive));
    }
    cout << "estimate " << candidates[max_element(distribution.begin(), distribution.end()) - distribution.begin()] << "\n" << flush;
}
