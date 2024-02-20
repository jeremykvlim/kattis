#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> strengths(n);
    for (auto &s : strengths) cin >> s;

    sort(strengths.rbegin(), strengths.rend());
    int remain = 0;
    long long last = 0, curr = 0;
    for (int i = 0; i < n; i++) {
        curr += strengths[i];
        if (curr >= last) {
            last += curr;
            curr = 0;
            remain = i + 1;
        }
    }

    cout << remain;
}
