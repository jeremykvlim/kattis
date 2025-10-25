#include <bits/stdc++.h>
using namespace std;

double log_binomial_coefficient(long long n, long long k) {
    if (k < 0 || k > n) return numeric_limits<double>::quiet_NaN();
    return lgamma(n + 1) - lgamma(k + 1) - lgamma(n - k + 1);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int P, t;
    double l;
    cin >> P >> t >> l;

    int last = -1;
    vector<int> indices(P);
    for (int &i : indices) {
        cin >> i;

        last = max(last, i);
    }

    int sigma = sqrt(t), _5_sigma = 5 * sigma;
    double lg_sample_space = t * log(2);
    vector<double> B(_5_sigma / 2 + 1);
    for (int k = t / 2; k <= min(t - t / 2, _5_sigma / 2) + t / 2; k++) B[k - t / 2] = exp(log_binomial_coefficient(t, k) - lg_sample_space);

    vector<double> fries(last + 2 * _5_sigma + 1, 0);
    if (P <= 500) {
        for (int i : indices)
            for (int j = -_5_sigma + ((-_5_sigma + t) & 1); j <= _5_sigma; j += 2) fries[i + j + _5_sigma] += B[(abs(j) + 1) / 2];

        int x = 0;
        for (auto f : fries) x += (f >= l);
        cout << x;
        exit(0);
    }

    vector<pair<double, int>> gaps;
    auto lowest = 1.25;
    for (int k = 0; k <= _5_sigma / 2; k++)
        if (lowest > B[k] / 0.8) {
            lowest = B[k];
            gaps.emplace_back(B[k], k);
        }

    for (int i : indices) {
        double prev = 0;
        int j = 2 * gaps[0].second;

        reverse(gaps.begin(), gaps.end());
        for (auto [p, offset] : gaps) {
            fries[i - j + _5_sigma] += p - prev;
            prev = p;
            j = 2 * offset;
        }
        reverse(gaps.begin(), gaps.end());
        for (auto [p, offset] : gaps) {
            fries[i + 2 * (offset - 1) + _5_sigma] += p - prev;
            prev = p;
            j = 2 * offset;
        }
        fries[i + j + _5_sigma] -= prev;
    }

    int x = 0;
    double even = 0, odd = 0;
    for (int i = 0; i < last + 2 * _5_sigma; i += 2) {
        x += (even >= l) + (odd >= l);
        even += fries[i];
        odd += fries[i + 1];
    }
    cout << x;
}
