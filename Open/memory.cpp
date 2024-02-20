#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9 + 9;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string n;
    cin >> n;

    auto curr = 0LL, prev = 1LL;
    for (int i = n.size() - 1, csz = 0; ~i; i--)
        if (n[i] == '0') csz++;
        else {
            auto total = (curr + prev) % MODULO;
            curr = (csz * prev + (csz + 1) * curr) % MODULO;
            prev = total;
            csz = 0;
        }

    cout << (curr + prev) % MODULO;
}
