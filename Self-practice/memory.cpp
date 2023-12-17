#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9 + 9;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string n;
    cin >> n;

    long long curr = 0, prev = 1;
    for (int i = n.size() - 1, csz = 0; i >= 0; i--)
        if (n[i] == '0') csz++;
        else {
            auto total = (curr + prev) % MODULO;
            curr = (csz * prev + (csz + 1) * curr) % MODULO;
            prev = total;
            csz = 0;
        }

    cout << (curr + prev) % MODULO;
}
