#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    double A, B;
    while (cin >> A >> B) {
        int n;
        cin >> n;

        vector<tuple<double, double, int>> intervals(n);
        for (int i = 0; i < n; i++) {
            long double a, b;
            cin >> a >> b;

            intervals[i] = make_tuple(a, b, i);
        }
        sort(intervals.begin(), intervals.end());

        double last = A, end = LLONG_MIN;
        int curr = -1;
        vector<int> cover;
        for (auto [a, b, i] : intervals) {
            if (a > last) {
                if (curr == -1 || end >= B) break;
                
                cover.emplace_back(curr);
                last = end;
                end = LLONG_MIN;
                curr = -1;
            
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
            cout << cover.size() << "\n";
            for (int i : cover) cout << i << " ";
            cout << "\n";
        }
    }
}
