#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<bool> valid(n + 2, false);
    vector<long long> squares(n + 2);
    for (auto i = 0LL; i <= n + 1; i++) squares[i] = i * i;

    int bound = ceil(sqrt(n)) + 1;
    for (int a = 0; a <= bound; a++)
        for (int b = a; b <= bound; b++) {
            int sum = squares[a] + squares[b];
            if (sum > n + 1) break;
            valid[sum] = true;
        }

    int count = 0;
    for (int c = 0; c <= n; c++)
        if (valid[c] && valid[c + 1]) {
            auto C = (long long) c * (c + 1);
            bound = ceil(sqrtl(C / 2));
            for (int a = 0; a <= bound; a++) {
                auto B = C - squares[a];
                int b = ceil(sqrtl(B));
                if (squares[b] == B && a <= b && b <= c) count++;
            }
        }

    cout << count;
}
