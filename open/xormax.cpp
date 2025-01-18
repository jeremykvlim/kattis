#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<long long> basis(60, 0);
    while (n--) {
        long long a;
        cin >> a;

        for (int i = 59; ~i; i--) 
            if ((a >> i) & 1) {
                if (!basis[i]) basis[i] = a;
                a ^= basis[i];
            }
    }

    auto sum = 0LL;
    for (int i = 59; ~i; i--)
        if (basis[i] && !((sum >> i) & 1)) sum ^= basis[i];

    cout << sum;
}
