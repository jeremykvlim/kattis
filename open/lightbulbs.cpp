#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    
    auto print = [&](auto lamps) {
        vector<string> grid(n, string(n, '0'));
        for (auto [r, c] : lamps) grid[r][c] = '1';
        for (auto row : grid) cout << row << "\n" << flush;
    };

    mt19937 rng(random_device{}());
    vector<unsigned long long> states{0};
    vector<pair<int, int>> unknowns, h_lamps, v_lamps;
    vector<vector<bool>> lit_squares(n, vector<bool>(n));
    vector<vector<int>> dir(n, vector<int>(n, -1)), indices(n, vector<int>(n, -1));
    while (h_lamps.size() < n && v_lamps.size() < n) {
        while (states.size() <= 256 && unknowns.size() < min(64, (int) ((n - h_lamps.size()) * (n - v_lamps.size())))) {
            int r = -1, c = -1;
            vector<pair<int, int>> valid;
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    if (!lit_squares[i][j] && !~indices[i][j]) valid.emplace_back(i, j);

            if (valid.empty()) break;
            tie(r, c) = valid[rng() % valid.size()];

            if (!~r && !~c) continue;
            for (int i = states.size() - 1; ~i; i--) states.emplace_back(states[i] | (1ULL << unknowns.size()));
            indices[r][c] = unknowns.size();
            unknowns.emplace_back(r, c);
        }

        auto lit = [&](auto lamps, auto s) -> int {
            vector<bool> row(n, false), col(n, false);
            for (auto [r, c] : lamps) ((~dir[r][c] ? dir[r][c] : (s >> indices[r][c]) & 1) ? col[c] : row[r]) = true;
            return n * n - count(row.begin(), row.end(), false) * count(col.begin(), col.end(), false);
        };

        vector<pair<int, int>> lamps;
        double H = -1;
        for (int _ = 0; _ < 100; _++) {
            int p = rng() % 100 + 1;
            vector<pair<int, int>> temp;
            auto add = [&](auto l) {
                for (auto [r, c] : l)
                    if (rng() % 100 < p) temp.emplace_back(r, c);
            };
            add(h_lamps);
            add(v_lamps);
            add(unknowns);

            unordered_map<int, int> freq;
            for (auto s : states) freq[lit(temp, s)]++;

            double sum = 0;
            for (auto [_, f] : freq) {
                auto p_i = (double) f / states.size();
                sum += p_i * p_i;
            }

            auto collision_entropy = -log2(sum);
            if (H < collision_entropy) {
                H = collision_entropy;
                lamps = temp;
            }
        }

        cout << "?\n";
        print(lamps);

        int l;
        cin >> l;

        vector<unsigned long long> valid;
        for (auto s : states)
            if (lit(lamps, s) == l) valid.emplace_back(s);
        states = valid;

        auto h = 0ULL, v = 0ULL;
        for (auto s : states) {
            h |= s ^ ((1ULL << unknowns.size()) - 1);
            v |= s;
        }

        vector<pair<int, int>> temp;
        for (int i = 0; i < unknowns.size(); i++) {
            auto [r, c] = unknowns[i];
            if (((h ^ v) >> i) & 1) {
                if (!lit_squares[r][c]) {
                    bool d = (v >> i) & 1;

                    dir[r][c] = d;
                    if (!d) {
                        for (int j = 0; j < n; j++) lit_squares[r][j] = true;
                        h_lamps.emplace_back(r, c);
                    } else {
                        for (int j = 0; j < n; j++) lit_squares[j][c] = true;
                        v_lamps.emplace_back(r, c);
                    }
                }
                indices[r][c] = -1;
            } else {
                indices[r][c] = temp.size();
                temp.emplace_back(r, c);
            }
        }

        for (auto &curr : states) {
            auto next = 0ULL;
            for (int i = 0; i < unknowns.size(); i++)
                if ((curr >> i) & 1) {
                    auto [r, c] = unknowns[i];
                    if (~indices[r][c]) next |= 1ULL << indices[r][c];
                }
            curr = next;
        }

        sort(states.begin(), states.end());
        states.erase(unique(states.begin(), states.end()), states.end());
        unknowns = temp;
    }

    cout << "!\n";
    print(h_lamps.size() == n ? h_lamps : v_lamps);
}
