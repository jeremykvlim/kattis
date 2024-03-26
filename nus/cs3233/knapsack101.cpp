#include<bits/stdc++.h>
using namespace std;

long long pow(long long base, long long exponent, long long mod = 1) {
    auto value = 1LL;
    for (; exponent; exponent >>= 1) {
        if (exponent & 1) value = (base * value) % mod;
        base = (base * base) % mod;
    }
    return value;
}

void dfs(vector<int> &w, int p, int i, int j, int depth, int d, vector<int> &indices, unordered_map<long long, vector<int>> &half, long long z = 1) {
    if (half.size() >= 50000) return;

    if (depth == d) {
        half[z] = indices;
        return;
    }

    if (i > j) return;

    dfs(w, p, i + 1, j, depth, d, indices, half, z);
    indices.emplace_back(i);

    auto zi = (z * w[i - 1]) % p;
    dfs(w, p, i + 1, j, depth + 1, d, indices, half, zi);
    indices.pop_back();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, p;
    cin >> n >> p;

    vector<int> w(2 * n);
    for (int &wi : w) cin >> wi;

    long long z;
    cin >> z;

    auto pw = 1LL;
    for (int i = 0; i < n; i++) pw = (pw * w[i]) % p;
    z = (z * pow(pw, p - 2, p)) % p;

    if (z == 1) {
        for (int i = 0; i < n; i++) cout << 1;
        for (int i = 0; i < n; i++) cout << 0;
        exit(0);
    }

    string s(2 * n, '0');
    for (int i = 1; i <= n; i++) {
        vector<int> l, r;
        unordered_map<long long, vector<int>> left, right;

        dfs(w, p, 1, n, 0, i, l, left);
        dfs(w, p, n + 1, 2 * n, 0, i, r, right);

        for (auto [zl, indices] : left) {
            auto zr = (z * zl) % p;

            if (right.count(zr)) {
                for (int j = 0; j < n; j++) s[j] = '1';
                for (int k : indices) s[k - 1] = '0';
                for (int k : right[zr]) s[k - 1] = '1';

                cout << s;
                exit(0);
            }
        }
    }
}
