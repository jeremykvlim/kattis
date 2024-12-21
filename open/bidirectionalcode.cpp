#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;

    auto reduce = [&](long long n) {
        if (n == 1) return n;

        string s = to_string(n);
        for (int i = 1; i < s.size(); i++)
            if (s[i] != '0') goto next;
        return n - 1;

        next:;
        auto m1 = s.substr(0, (s.size() + 1) / 2), m2 = s.substr(0, s.size() / 2);
        reverse(m2.begin(), m2.end());
        auto m = stoll(m1 + m2);
        if (m <= n) return m;

        m1 = to_string(stoll(m1) - 1);
        m2 = m1.substr(0, m2.size());
        reverse(m2.begin(), m2.end());
        return stoll(m1 + m2);
    };

    vector<long long> palindromes;
    while (n) {
        auto m = reduce(n);
        palindromes.emplace_back(m);
        n -= m;
    }

    cout << palindromes.size() << "\n";
    for (auto p : palindromes) cout << p << "\n";
}
