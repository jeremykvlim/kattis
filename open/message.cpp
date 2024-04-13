#include <bits/stdc++.h>
using namespace std;

long long mul(long long x, long long y, long long mod) {
    auto product = x * y - mod * (long long) (1.L / mod * x * y);
    return product + mod * (product < 0) - mod * (product >= mod);
}

long long pow(long long base, long long exponent, long long mod) {
    auto value = 1LL;
    for (; exponent; exponent >>= 1) {
        if (exponent & 1) value = mul(value, base, mod);
        base = mul(base, base, mod);
    }

    return value;
}

vector<vector<long long>> I(int size) {
    vector<vector<long long>> I(size, vector<long long>(size, 0));
    for (int i = 0; i < size; i++) I[i][i] = 1;

    return I;
}

vector<vector<long long>> matmul(vector<vector<long long>> &a, vector<vector<long long>> &b, long long mod) {
    int r1 = a.size(), r2 = b.size(), c2 = b[0].size();

    vector<vector<long long>> c(r1, vector<long long>(c2, 0));
    for (int i = 0; i < r1; i++)
        for (int j = 0; j < c2; j++)
            for (int k = 0; k < r2; k++) c[i][j] = (c[i][j] + mul(a[i][k], b[k][j], mod)) % mod;

    return c;
}

vector<vector<long long>> matpow(vector<vector<long long>> a, long long n, long long mod) {
    int size = a.size();
    auto b = I(size);

    for (; n; n >>= 1) {
        if (n & 1) b = matmul(a, b, mod);
        a = matmul(a, a, mod);
    }

    return b;
}

vector<int> kmp(string s) {
    vector<int> pi(s.size());
    for (int i = 1; i < s.size(); i++) {
        int j = pi[i - 1];
        while (j && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }

    return pi;
}

vector<vector<int>> automaton(string s, vector<int> pi) {
    vector<vector<int>> fsm(s.size(), vector<int>(26));
    for (int i = 0; i < s.size(); i++)
        for (int c = 0; c < 26; c++)
            fsm[i][c] = (i && 'a' + c != s[i]) ? fsm[pi[i - 1]][c] : i + ('a' + c == s[i]);

    return fsm;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        long long n, m;
        string s;
        cin >> n >> m >> s;

        int size = s.size();
        auto pi = kmp(s);
        auto fsm = automaton(s, pi);

        vector<vector<long long>> count(size, vector<long long>(size, 0));
        for (int i = 0; i < size; i++)
            for (int c = 0; c < 26; c++)
                if (fsm[i][c] < size) count[i][fsm[i][c]]++;

        count = matpow(count, n, m);
        auto r = 0LL;
        for (int i = 0; i < size; i++) {
            r += count[0][i];
            if (r >= m) r -= m;
        }

        r = (pow(26, n, m) - r) % m;
        if (r < 0) r += m;

        cout << r << "\n";
    }
}
