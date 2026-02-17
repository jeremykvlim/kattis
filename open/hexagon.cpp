#include <bits/stdc++.h>
using namespace std;

template <typename T>
pair<T, vector<int>> jonker_volgenant(const vector<vector<T>> &C) {
    int n = C.size(), m = C[0].size();

    vector<T> dist(m), potential(m);
    vector<int> row_match(n, -1), col_match(m, -1), cols(m), prev(m);
    iota(cols.begin(), cols.end(), 0);
    T d = 0;
    for (int i = 0, c1 = -1, temp = 0; i < n; i++) {
        for (int c = 0; c < m; c++) {
            dist[c] = C[i][c] - potential[c];
            prev[c] = i;
        }

        int s = 0, t = 0;
        for (;;) {
            if (s == t) {
                temp = s;
                d = dist[cols[t++]];
                for (int j = t; j < m; j++) {
                    c1 = cols[j];
                    if (d < dist[c1]) continue;
                    if (d > dist[c1]) {
                        d = dist[c1];
                        t = s;
                    }
                    cols[j] = exchange(cols[t++], c1);
                }

                for (int j = s; j < t; j++)
                    if (!~col_match[c1 = cols[j]]) goto done;
            }

            int c2 = cols[s++], r = col_match[c2];
            for (int j = t; j < m; j++) {
                c1 = cols[j];
                if (dist[c1] > C[r][c1] - C[r][c2] + potential[c2] - potential[c1] + d) {
                    dist[c1] = C[r][c1] - C[r][c2] + potential[c2] - potential[c1] + d;
                    prev[c1] = r;

                    if (dist[c1] == d) {
                        if (!~col_match[c1]) goto done;
                        cols[j] = exchange(cols[t++], c1);
                    }
                }
            }
        }
        done:;

        for (int j = 0; j < temp; j++) potential[cols[j]] += dist[cols[j]] - d;
        for (int r = -1; r != i;) {
            r = col_match[c1] = prev[c1];
            swap(c1, row_match[r]);
        }
    }

    T cost = 0;
    for (int i = 0; i < n; i++) cost += C[i][row_match[i]];
    return {cost, row_match};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    cin.ignore();

    for (int X = 1; X <= n; X++) {
        string s1;
        getline(cin, s1);

        istringstream iss1(s1);
        vector<int> pos{istream_iterator<int>{iss1}, istream_iterator<int>{}};

        string s2;
        getline(cin, s2);

        istringstream iss2(s2);
        vector<int> val{istream_iterator<int>{iss2}, istream_iterator<int>{}};

        int s = pos.size(), radius = (s - 1) / 2;
        vector<array<int, 3>> coords(3 * radius * (radius + 1) + 2);
        for (int r = -radius, p = 1; r <= radius; r++)
            for (int x = max(-radius, -r - radius); x <= min(radius, -r + radius); x++) coords[p++] = {x, -x - r, r};

        vector<array<int, 3>> checkers(s);
        for (int i = 0; i < s; i++) checkers[i] = coords[pos[i]];

        vector<vector<array<int, 3>>> targets(3);
        for (int r = -radius; r <= radius; r++) {
            targets[0].push_back({r, -r, 0});
            targets[1].push_back({0, -r, r});
            targets[2].push_back({r, 0, -r});
        }

        int score = 1e9;
        for (int k = 0; k < 3; k++) {
            auto dist = [&](auto &a1, auto &a2) {
                return max({abs(a1[0] - a2[0]), abs(a1[1] - a2[1]), abs(a1[2] - a2[2])});
            };

            vector<vector<int>> C(s, vector<int>(s));
            for (int i = 0; i < s; i++)
                for (int j = 0; j < s; j++) C[i][j] = val[i] * dist(checkers[i], targets[k][j]);

            score = min(score, jonker_volgenant(C).first);
        }

        cout << "Case #" << X << ": " << score << "\n";
    }
}
