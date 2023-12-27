#include <bits/stdc++.h>
using namespace std;

bool valid(vector<char> v) {
    return v.empty() || (sort(v.begin(), v.end()), v.front() == v.back());
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int k;
        cin >> k;
        
        vector<char> steps(2 * k);
        for (auto &s : steps) cin >> s;
        int s = 0;
        vector<int> points{0, 0}, remaining{k, k}, prev{-1, -1};

        for (int i = 0; i < k * 2 && points[0] + remaining[0] >= points[1] && points[1] + remaining[1] >= points[0]; i++, s++) {
            if (steps[i] == 'E') points[i & 1]++;
            remaining[i & 1]--;
            if (points[0] + remaining[0] < points[1] || points[1] + remaining[1] < points[0]) prev[i & 1] = i;
        }

        for (int c = 0; c < s; c++) {
            vector<char> a, b;
            for (int i = c; i < s; i++)
                if (prev[i & 1] != i) (i & 1 ? a : b).emplace_back(steps[i]);

            if (valid(a) && valid(b)) {
                cout << s << ' ' << c << "\n";
                break;
            }
        }
    }
}
