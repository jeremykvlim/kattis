#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    double s, r;
    int n, z;
    cin >> s >> r >> n >> z;

    int count = 0;
    while (s >= 2 * r) {
        count += (int) (M_PI / asin(r / (s - r)));
        s -= 2 * r;
    }
    if (s >= r) count++;
    cout << min({n, count, (int) (s * s * z / (100 * r * r))});
}
