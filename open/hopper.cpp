#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, D, m;
    cin >> n >> D >> m;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    auto update = [&](pair<int, array<int, 7>> &s, int b, int degree) {
        if (!degree) {
            s.first &= ~(1 << b);
            s.second[b] = 0;
        } else if (degree == 1) {
            s.first &= ~(1 << b);
            if (!s.second[b]) s.second[b] = 1;
        } else {
            s.first |= 1 << b;
            s.second[b] = 0;
        }
    };

    auto encode = [&](const pair<int, array<int, 7>> &s) -> int {
        int e = 0;
        for (int b = 0; b < D; b++) e |= (s.second[b] & 7) << (3 * b);
        return e | (s.first << (3 * D));
    };

    int label = 1;
    pair<int, array<int, 7>> s;
    vector<int> encoded, ends(8, 0);
    vector<pair<int, array<int, 7>>> states;
    auto dfs = [&](auto &&self, int b = 0) -> void {
        if (b == D) {
            states.emplace_back(s);
            encoded.emplace_back(encode(s));
            return;
        }

        update(s, b, 0);
        self(self, b + 1);
        update(s, b, 2);
        self(self, b + 1);
        update(s, b, 1);
        for (int k = 1; k < label; k++)
            if (ends[k] == 1) {
                s.second[b] = k;
                ends[k] = 2;
                self(self, b + 1);
                ends[k] = 1;
            }
        
        if (label <= 7) {
            s.second[b] = label;
            ends[label] = 1;
            label++;
            self(self, b + 1);
            label--;
            ends[label] = 0;
        }
        s.second[b] = 0;
    };
    dfs(dfs);

    int size = states.size();
    vector<int> order(size);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j) { return encoded[i] < encoded[j]; });
    sort(encoded.begin(), encoded.end());

    auto degree = [&](const pair<int, array<int, 7>> &s, int b) -> int {
        return (s.first >> b) & 1 ? 2 : !!s.second[b];
    };

    vector<vector<array<int, 4>>> next(size);
    for (int i = 0; i < size; i++) {
        auto add = [&](int mask = 0) {
            int gain = 0;
            array<int, 2> lengths;
            for (int b = 0; b < D; b++)
                if ((mask >> b) & 1) lengths[gain++] = b;

            auto s1 = states[i];
            if (gain == 2) {
                auto [b1, b2] = lengths;
                if (degree(s1, b1) == 1 && degree(s1, b2) == 1 && s1.second[b1] == s1.second[b2]) return;
            }

            vector<int> degrees(D);
            for (int b = 0; b < D; b++) degrees[b] = degree(s1, b);

            auto s2 = s1;
            if (gain == 1) {
                int b1 = lengths[0];
                if (!degrees[b1]) {
                    update(s2, b1, 1);
                    vector<bool> used(8, false);
                    for (int b = 0; b < D; b++)
                        if (!((s2.first >> b) & 1) && s2.second[b]) used[s2.second[b]] = true;
                    s2.second[b1] = find_if(used.begin() + 1, used.end(), [&](bool u) { return !u; }) - begin(used);
                } else update(s2, b1, 2);
            } else if (gain == 2) {
                auto [b1, b2] = lengths;
                if (!degrees[b1] && !degrees[b2]) {
                    update(s2, b1, 1);
                    update(s2, b2, 1);
                    vector<bool> used(8, false);
                    for (int b = 0; b < D; b++)
                        if (!((s2.first >> b) & 1) && s2.second[b]) used[s2.second[b]] = true;
                    s2.second[b1] = s2.second[b2] = find_if(used.begin() + 1, used.end(), [&](bool u) { return !u; }) - begin(used);
                } else if (degrees[b1] == 1 && !degrees[b2]) {
                    update(s2, b1, 2);
                    update(s2, b2, 1);
                    s2.second[b2] = s1.second[b1];
                } else if (!degrees[b1] && degrees[b2] == 1) {
                    update(s2, b2, 2);
                    update(s2, b1, 1);
                    s2.second[b1] = s1.second[b2];
                } else {
                    update(s2, b1, 2);
                    update(s2, b2, 2);
                    if (s1.second[b1] != s1.second[b2] && s1.second[b2])
                        for (int b = 0; b < D; b++)
                            if (!((s2.first >> b) & 1) && s2.second[b] == s1.second[b2]) s2.second[b] = s1.second[b1];
                }
            }

            pair<int, array<int, 7>> s3;
            for (int b = D - 1; b; b--) {
                int d = degree(s2, b - 1);
                update(s3, b, d);
                if (d == 1) s3.second[b] = s2.second[b - 1];
            }

            update(s3, 0, gain);
            if (gain == 1) s3.second[0] = !degrees[lengths[0]] ? s2.second[lengths[0]] : s1.second[lengths[0]];

            vector<int> seen(8, 0);
            for (int b = 0, count = 1; b < D; b++)
                if (!((s3.first >> b) & 1) && s3.second[b]) {
                    int l = s3.second[b];
                    if (!seen[l]) seen[l] = count++;
                    s3.second[b] = seen[l];
                } else s3.second[b] = 0;

            next[i].push_back({mask, degree(s2, D - 1) == 1, order[lower_bound(encoded.begin(), encoded.end(), encode(s3)) - encoded.begin()], gain});
        };

        int mask = 0;
        for (int b = 0; b < D; b++)
            if (degree(states[i], b) < 2) mask |= 1 << b;

        add();

        for (int b1 = 0; b1 < D; b1++)
            if ((mask >> b1) & 1) {
                add(1 << b1);
                for (int b2 = b1 + 1; b2 < D; b2++)
                    if ((mask >> b2) & 1) add((1 << b1) | (1 << b2));
            }
    }

    array<int, 3> unreachable{-1, -1, -1};
    int start = order[lower_bound(encoded.begin(), encoded.end(), encode({})) - encoded.begin()];
    vector<array<int, 3>> memo(size, array<int, 3>{-1, -1, -1}), temp(size);
    memo[start][0] = 0;
    auto dp = [&](int m1 = 0) {
        for (int i = 0; i < size; i++) temp[i] = unreachable;

        for (int i = 0; i < size; i++)
            if (memo[i] != unreachable)
                for (auto [m2, end, k, gain] : next[i])
                    if (!(m2 & ~m1))
                        for (int j = 0; j + end < 3; j++)
                            if (~memo[i][j]) temp[k][j + end] = max(temp[k][j + end], memo[i][j] + gain);
        memo = temp;
    };

    for (int i = 0; i < n; i++) {
        int mask = 0;
        for (int b = 1; b <= min(D, i); b++)
            if (abs(a[i] - a[i - b]) <= m) mask |= 1 << (b - 1);
        dp(mask);
    }
    while (D--) dp();
    cout << memo[start][2] + 1;
}
