#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long double A, B;
    while (cin >> A >> B) {
        int n;
        cin >> n;
        
        vector<tuple<long double, long double, long long>> intervals(n);
        for (int i = 0; i < n; i++) {
            long double a, b;
            cin >> a >> b;
            intervals[i] = make_tuple(a, b, i);
        }
        sort(intervals.begin(), intervals.end());

        long double last = A, end = LLONG_MIN;
        long long curr = -1;
        vector<long long> cover;
        
        for (auto &[a, b, i] : intervals) {
            if (a > last) {
                if (curr == -1 || end >= B) break;
                cover.emplace_back(curr);
                last = end;
                curr = -1;
                end = -LLONG_MIN;
                if (a > last) break;
            }
            
            if (b > end) {
                end = b;
                curr = i;
            }
        }
        if (curr != -1) cover.emplace_back(curr);

        if (end < B) cout << "impossible\n";
        else {
            cout << cover.size() << '\n';
            for (auto &i : cover) cout << i << '\n';
        }
    }
}
