#include <bits/stdc++.h>
using namespace std;

bool gale_ryser(const vector<int> &row_sums, const vector<int> &col_sums, int r = 0, int c = 0) {
    int n = row_sums.size(), m = col_sums.size();

    int active = 0;
    vector<int> indices;
    for (int i = r; i < n; i++)
        if (row_sums[i]) {
            active++;
            indices.emplace_back(i);
        }

    if (c == m) return !active;
    if (col_sums[c] > active) return false;

    sort(indices.begin(), indices.end(), [&](int i, int j) { return row_sums[i] > row_sums[j]; });
    auto temp = row_sums;
    for (int i = 0, remaining = col_sums[c]; i < active && remaining; i++, remaining--) temp[indices[i]]--;

    vector<int> count(m, 0);
    int max_sum = 0, demand = 0;
    for (int i = 0; i < n; i++) {
        max_sum = max(max_sum, temp[i]);
        if (temp[i] > 0) {
            count[temp[i]]++;
            demand++;
        }
    }

    int supply = 0;
    for (int i = c + 1; i < m; i++) {
        if (col_sums[i] > demand - supply) return false;

        int s = max_sum, remaining = col_sums[i], prev = 0;
        for (; s && remaining; s--) {
            int sub = min(remaining, count[s]);
            count[s] -= sub;
            remaining -= sub;
            if (s == 1) supply += sub;

            count[s] += prev;
            prev = sub;
        }
        count[s] += prev;
        while (max_sum && !count[max_sum]) max_sum--;
    }
    return demand == supply;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t, p;
    while (cin >> t >> p && t && p) {
        vector<int> problems(t), teams(p);
        for (int &pi : problems) cin >> pi;
        for (int &tj : teams) cin >> tj;

        if (!all_of(problems.begin(), problems.end(), [&](int pi) { return pi <= p; }) ||
            !all_of(teams.begin(), teams.end(), [&](int tj) { return tj <= t; }) ||
            !gale_ryser(teams, problems)) {
            cout << "Impossible\n\n";
            continue;
        }

        vector<string> sol(t, string(p, '#'));
        for (int i = 0; i < t; i++)
            for (int j = 0; j < p; j++)
                if (!gale_ryser(teams, problems, (j + 1) % p, i + (j == p - 1))) {
                    sol[i][j] = 'Y';
                    problems[i]--;
                    teams[j]--;
                } else sol[i][j] = 'N';

        for (auto row : sol) cout << row << "\n";
        cout << "\n";
    }
}
