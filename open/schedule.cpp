#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, w;
    cin >> n >> w;

    int one = 1, two = 2;
    vector<string> schedule(1);
    while (schedule[0].size() < n) {
        (one < two ? one : two)++;
        schedule.assign(one + two, "");
        auto s = string(one, '1') + string(two, '2');
        do
            for (int i = one + two - 1; ~i; i--) schedule[i] += s[i];
        while (next_permutation(s.begin() + 1, s.end()));
    }

    if (one + two > w) {
        cout << "infinity";
        exit(0);
    }
    cout << one + two << "\n";
    for (auto &sc : schedule) sc.resize(n);
    for (int s = schedule.size(); schedule.size() < w;) schedule.emplace_back(schedule[schedule.size() % s]);
    for (auto &sc : schedule) cout << sc << "\n";
}
