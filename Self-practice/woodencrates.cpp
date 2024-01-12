#include <bits/stdc++.h>
using namespace std;

vector<long long> divisors(long long n) {
    vector<long long> d;
    for (auto i = 1LL; i <= sqrt(n); i++)
        if (!(n % i)) {
            d.emplace_back(i);
            if (n / i != i) d.emplace_back(n / i);
        }

    return d;
}

long long rearrange(long long d, long long total, vector<int> crates) {
    auto actions = 0LL, average = total / d;
    for (int c : crates) actions += abs(c - average);
    actions += average * (d - crates.size());

    auto last = d - 1;
    if (d == crates.size()) {
        while (last >= 0 && crates[last] == average) last--;
        if (last == -1) return 0;
    }

    vector<long long> diff(min((long long) crates.size(), last), -average);
    for (int i = 0; i < diff.size(); i++) {
        diff[i] += crates[i];
        if (i) diff[i] += diff[i - 1];
    }

    if (crates.size() < last) actions += (diff.back() - average) * ((diff.back() - average) / average + 1) - last + crates.size();

    for (auto &di : diff) {
        actions += abs(di);
        if (di > 1) {
            actions += di - 1;
            di = 1;
        } else if (!di) {
            actions++;
            di = 1;
        } else if (di < 0) {
            actions -= di;
            di = 0;
        }
    }

    long long remaining1 = count(diff.begin(), diff.end(), 0), remaining2 = last - remaining1;
    remaining1 = min(remaining1, remaining2);
    for (auto di : diff) {
        remaining2 += !di - (di == 1);
        remaining1 = min(remaining1, remaining2);
    }

    actions += remaining1;
    return actions;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> crates(n);
    for (auto &c : crates) cin >> c;

    auto total = accumulate(crates.begin(), crates.end(), 0LL), actions = LLONG_MAX;
    auto div = divisors(total);
    for (auto d : div)
        if (d >= n) actions = min(actions, rearrange(d, total, crates));

    cout << actions;
}
