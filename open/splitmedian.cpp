#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> A(n + 2), B(m + 2);
    vector<bool> seen_A(n + 2, false), seen_B(m + 2, false);
    A[0] = B[0] = INT_MIN;
    A[n + 1] = B[m + 1] = INT_MAX;
    seen_A[0] = seen_B[0] = seen_A[n + 1] = seen_B[m + 1] = true;

    auto a = [&](int i) {
        if (!seen_A[i]) {
            cout << "? A " << i << "\n" << flush;
            cin >> A[i];
            seen_A[i] = true;
        }
        return A[i];
    };

    auto b = [&](int i) {
        if (!seen_B[i]) {
            cout << "? B " << i << "\n" << flush;
            cin >> B[i];
            seen_B[i] = true;
        }
        return B[i];
    };

    int k = (n + m + 1) / 2, l = max(0, k - m), r = min(n, k) + 1, mid;
    while (l + 1 < r) {
        mid = l + (r - l) / 2;

        if (a(mid) <= b(k - mid + 1)) l = mid;
        else r = mid;
    }
    int xa = a(l), xb = b(k - l);
    cout << "! " << max(xa, xb);
}
