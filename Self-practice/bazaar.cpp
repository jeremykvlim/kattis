#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> A(n / 2), B(n / 2);
    for (int &a : A) cin >> a;
    for (int &b : B) cin >> b;
    sort(A.begin(), A.end());
    sort(B.begin(), B.end());

    int pieces = 0;
    while (n--) {
        pieces = (lower_bound(B.begin(), B.end(), A.back()) - B.begin()) - A.back() - pieces;
        A.pop_back();
        A.swap(B);
    }

    cout << pieces;
}
