#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> strengths(n);
    for (int s : strengths) cin >> s;

    sort(strengths.rbegin(), strengths.rend());
    int remain = 0;
    auto last = 0LL, curr = 0LL;
    for (int i = 0; i < n; i++) {
        curr += strengths[i];
        
        if (last <= curr) {
            last += curr;
            curr = 0;
            remain = i + 1;
        }
    }

    cout << remain;
}
