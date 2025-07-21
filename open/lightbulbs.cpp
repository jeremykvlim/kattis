#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    auto print = [&](auto lamps) {
        vector<string> grid(n, string(n, '0'));
        for (auto [r, c] : lamps) grid[r][c]++;
        for (auto row : grid) cout << row << "\n" << flush;
    };

    mt19937 rng(random_device{}());
    int lamps_h = 0, lamps_v = 0;
    vector<bool> h(n, false), v(n, false);
    vector<unsigned __int128> states{0};
    vector<pair<int, int>> unknowns;
    vector<vector<bool>> visited(n, vector<bool>(n, false));
    vector<vector<int>> dir(n, vector<int>(n, -1));

    auto OR = [&](unsigned __int128 &mask, int shift) {
        mask |= ((unsigned __int128) 1) << shift;
    };

    while (lamps_h < n && lamps_v < n) {
        while (states.size() < 128) {
            vector<pair<int, int>> valid;
            for (int r = 0; r < n; r++)
                for (int c = 0; c < n; c++)
                    if (!h[r] && !v[c] && !visited[r][c]) valid.emplace_back(r, c);

            if (valid.empty()) break;
            auto [r, c] = valid[rng() % valid.size()];
            visited[r][c] = true;
            unknowns.emplace_back(r, c);
            int s = states.size();
            states.resize(2 * s);
            for (int i = 0; i < s; i++) states[i + s] = states[i] | ((unsigned __int128) 1) << (unknowns.size() - 1);
        }

        auto lit = [&](int count_h, int count_v) -> int {
            return (count_h + count_v) * n - count_h * count_v;
        };

        int most = 1e3;
        vector<vector<bool>> chosen, sample(n, vector<bool>(n));
        for (int _ = 0; _ < 20; _++) {
            unsigned __int128 base_h = 0, base_v = 0;
            for (int r = 0; r < n; r++)
                for (int c = 0; c < n; c++) {
                    sample[r][c] = false;
                    if (~dir[r][c] || visited[r][c]) {
                        sample[r][c] = rng() & 1;
                        if (~dir[r][c] && sample[r][c]) {
                            if (!dir[r][c]) OR(base_h, r);
                            else OR(base_v, c);
                        }
                    }
                }

            int f = 0;
            unordered_map<int, int> freq;
            for (const auto &s : states) {
                auto lit_h = base_h, lit_v = base_v;
                for (int i = 0; i < unknowns.size(); i++) {
                    auto [r, c] = unknowns[i];
                    if (sample[r][c]) {
                        if (!((s >> i) & 1)) OR(lit_h, r);
                        else OR(lit_v, c);
                    }
                }
                f = max(f, ++freq[lit(popcount(lit_h), popcount(lit_v))]);
            }

            if (most > f) {
                most = f;
                chosen = sample;
            }
        }

        vector<pair<int, int>> lamps;
        for (int r = 0; r < n; r++)
            for (int c = 0; c < n; c++)
                if (chosen[r][c]) lamps.emplace_back(r, c);

        cout << "?\n";
        print(lamps);

        int l;
        cin >> l;

        unsigned __int128 base_h = 0, base_v = 0;
        for (int r = 0; r < n; r++)
            for (int c = 0; c < n; c++)
                if (~dir[r][c] && chosen[r][c]) {
                    if (!dir[r][c]) OR(base_h, r);
                    else OR(base_v, c);
                }

        vector<unsigned __int128> temp;
        for (const auto &s : states) {
            auto lit_h = base_h, lit_v = base_v;
            for (int i = 0; i < unknowns.size(); i++) {
                auto [r, c] = unknowns[i];
                if (chosen[r][c]) {
                    if (!((s >> i) & 1)) OR(lit_h, r);
                    else OR(lit_v, c);
                }
            }
            if (lit(popcount(lit_h), popcount(lit_v)) == l) temp.emplace_back(s);
        }

        for (int i = unknowns.size() - 1; ~i; i--) {
            bool d = (temp[0] >> i) & 1;
            if (!all_of(next(temp.begin()), temp.end(), [&] (auto state) { return ((state >> i) & 1) == d; })) continue;

            auto [r, c] = unknowns[i];
            dir[r][c] = d;
            visited[r][c] = false;
            if (!d) {
                lamps_h += !h[r];
                h[r] = true;
            } else {
                lamps_v += !v[c];
                v[c] = true;
            }
            for (auto &s : temp) s = (s & ((((unsigned __int128) 1) << i) - 1)) | ((s >> (i + 1)) << i);
            unknowns.erase(unknowns.begin() + i);
        }
        states = temp;
    }

    vector<pair<int, int>> lamps;
    if (lamps_h == n) {
        for (int r = 0; r < n; r++)
            for (int c = 0; c < n; c++)
                if (!dir[r][c]) {
                    lamps.emplace_back(r, c);
                    break;
                }
    } else {
        for (int c = 0; c < n; c++)
            for (int r = 0; r < n; r++)
                if (dir[r][c] > 0) {
                    lamps.emplace_back(r, c);
                    break;
                }
    }

    cout << "!\n";
    print(lamps);
}
