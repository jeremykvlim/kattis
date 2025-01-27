#include <bits/stdc++.h>
using namespace std;

struct Hash {
    size_t operator()(pair<int, int> p) const {
        auto h = hash<int>()(p.first);
        h ^= hash<int>()(p.second) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    string s;
    cin >> n >> s;

    int berries = n / 2, pick = 4, nim_sum = 0;
    vector<unordered_map<pair<int, int>, int, Hash>> memo(pick + 1);
    vector<unordered_map<pair<int, int>, vector<int>, Hash>> moves(pick + 1);
    auto grundy = [&](auto &&self, int l, int r) {
        if (l > r) return 0;
        if (memo[pick].count({l, r})) return memo[pick][{l, r}];

        vector<pair<int, int>> possible;
        for (int i = l; i <= r; i++)
            if (count(s.begin() + i, s.begin() + min(r + 1, i + 4), 'b') == pick) possible.emplace_back(self(self, l, i - 1) ^ self(self, i + 4, r), i);
        sort(possible.begin(), possible.end());
        possible.erase(unique(possible.begin(), possible.end(), [&](auto p1, auto p2) {return p1.first == p2.first;}), possible.end());

        int mex = 0;
        for (auto [m, i] : possible) {
            if (mex != m) break;
            mex++;
        }
        for (auto [m, i] : possible) {
            if (mex < m) break;
            moves[pick][{l, r}].emplace_back(i);
        }
        if (!mex && !possible.empty()) moves[pick][{l, r}].emplace_back(possible.begin()->second);

        return memo[pick][{l, r}] = mex;
    };

    int bound = __lg(n / pick) + 1;
    vector<unordered_set<pair<int, int>, Hash>> ranges_by_bits(bound);
    auto update_by_bits = [&](int l, int r, int g, bool add) {
        for (int b = 0; b < bound && (1 << b) <= g; b++)
            if ((g >> b) & 1) add ? (void) ranges_by_bits[b].emplace(l, r) : (void) ranges_by_bits[b].erase({l, r});
    };

    set<pair<int, int>> ranges_sorted;
    unordered_set<pair<int, int>, Hash> ranges_active;
    auto update = [&](int l, int r, bool add) {
        if (l > r) return;

        int g = grundy(grundy, l, r);
        nim_sum ^= g;

        update_by_bits(l, r, g, add);
        if (add) {
            ranges_sorted.emplace(l, r);
            if (!moves[pick][{l, r}].empty()) ranges_active.emplace(l, r);
        } else {
            ranges_sorted.erase({l, r});
            ranges_active.erase({l, r});
        }
    };

    bool berry = false;
    for (int i = 0, j = -1; i <= n; i++) {
        if (!berry) j = i;
        if (i < n && s[i] == 'b') berry = true;

        if (berry && !count(s.begin() + i, s.begin() + i + 3, 'b')) {
            update(j, i - 1, true);
            berry = false;
        }
    }

    auto temp = s;
    while (berries) {
        int i;
        cin >> i;
        i--;

        int next = count(temp.begin() + i, temp.begin() + i + 4, 'b');
        berries -= next;

        if (pick != next) {
            pick = next;
            nim_sum = 0;
            ranges_active.clear();
            for (auto &us : ranges_by_bits) us.clear();
            for (auto [l, r] : ranges_sorted) update(l, r, true);
        }

        auto move = [&]() {
            auto [l, r] = *prev(ranges_sorted.upper_bound({i + 3, n + 1}));
            update(l, r, false);
            update(l, i - 1, true);
            update(i + 4, r, true);
            fill_n(temp.begin() + i, 4, '.');
        };
        move();

        if (!nim_sum) i = moves[pick][*ranges_active.begin()].back();
        else {
            auto [l, r] = *ranges_by_bits[__lg(nim_sum)].begin();
            i = moves[pick][{l, r}][grundy(grundy, l, r) ^ nim_sum];
        }

        cout << min(i + 1, n - 3) << "\n" << flush;
        move();

        berries -= pick;
    }
}
