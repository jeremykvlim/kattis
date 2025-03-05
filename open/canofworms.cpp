#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<array<int, 3>> fences(n);
    vector<int> l(n), r(n);
    for (int i = 0; i < n; i++) {
        cin >> fences[i][0] >> fences[i][1];

        fences[i][2] = l[i] = r[i] = i;
    }
    sort(fences.begin(), fences.end());

    bool change;
    do {
        change = false;
        auto update = [&](bool rev = false) {
            stack<int> s;
            for (int i = rev ? n - 1 : 0; (rev ? ~i : i < n); (rev ? i-- : i++)) {
                while (!s.empty() && abs(fences[i][0] - fences[s.top()][0]) <= fences[i][1]) {
                    int j = s.top();
                    s.pop();

                    if (l[i] > l[j]) {
                        l[i] = l[j];
                        change = true;
                    }

                    if (r[i] < r[j]) {
                        r[i] = r[j];
                        change = true;
                    }
                }
                s.emplace(i);
            }
        };
        update(false);
        update(true);
    } while (change);

    vector<int> cans(n);
    for (int i = 0; i < n; i++) cans[fences[i][2]] = r[i] - l[i] + 1;
    for (int c : cans) cout << c << " ";
}
