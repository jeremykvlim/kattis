#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, D, m;
    cin >> n >> D >> m;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    auto update = [&](pair<int, array<int, 7>> &s, int pos, int degree) {
        if (!degree) {
            s.first &= ~(1 << pos);
            s.second[pos] = 0;
        } else if (degree == 1) {
            s.first &= ~(1 << pos);
            if (!s.second[pos]) s.second[pos] = 1;
        } else {
            s.first |= 1 << pos;
            s.second[pos] = 0;
        }
    };

    auto encode = [&](const pair<int, array<int, 7>> &s) -> int {
        int e = 0;
        for (int pos = 0; pos < D; pos++) e |= s.second[pos] << (3 * pos);
        return e | (s.first << (3 * D));
    };

    int label = 1;
    pair<int, array<int, 7>> s{};
    vector<int> encoded, ends(8, 0);
    vector<pair<int, array<int, 7>>> states;
    auto dfs = [&](auto &&self, int pos = 0) -> void {
        if (pos == D) {
            states.emplace_back(s);
            encoded.emplace_back(encode(s));
            return;
        }

        update(s, pos, 0);
        self(self, pos + 1);
        update(s, pos, 2);
        self(self, pos + 1);
        update(s, pos, 1);
        for (int k = 1; k < label; k++)
            if (ends[k] == 1) {
                s.second[pos] = k;
                ends[k] = 2;
                self(self, pos + 1);
                ends[k] = 1;
            }
        
        if (label <= 7) {
            s.second[pos] = label;
            ends[label] = 1;
            label++;
            self(self, pos + 1);
            label--;
            ends[label] = 0;
        }
        s.second[pos] = 0;
    };
    dfs(dfs);

    int size = states.size();
    vector<int> order(size);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j) { return encoded[i] < encoded[j]; });
    sort(encoded.begin(), encoded.end());

    int nodes = 0;
    vector<int> single(size, 0), node_id(size, -1);
    vector<bool> labeled(size, false);
    for (int i = 0; i < size; i++) {
        vector<int> count(8, 0);
        for (int pos = 0; pos < D; pos++)
            if (!((states[i].first >> pos) & 1) && states[i].second[pos]) {
                count[states[i].second[pos]]++;
                labeled[i] = true;
            }

        for (int k = 1; k < 8; k++)
            if (count[k] == 1) single[i]++;

        if (single[i] <= 2 && (labeled[i] || !states[i].first)) node_id[i] = nodes++;
    }

    auto degree = [&](const pair<int, array<int, 7>> &s, int pos) -> int {
        return (s.first >> pos) & 1 ? 2 : !!s.second[pos];
    };

    vector<int> degrees(7, 0);
    vector<vector<array<int, 3>>> next(1 << D);
    vector<vector<array<int, 2>>> last(1 << D);
    for (int i = 0; i < size; i++) {
        if (node_id[i] == -1) continue;

        fill(degrees.begin(), degrees.end(), 0);
        auto add = [&](int mask = 0) {
            int gain = 0;
            array<int, 2> offsets;
            for (int pos = 0; pos < D; pos++)
                if ((mask >> pos) & 1) offsets[gain++] = pos;
            
            auto s1 = states[i];
            if (gain == 2) {
                auto [l, r] = offsets;
                if (degree(s1, l) == 1 && degree(s1, r) == 1 && s1.second[l] == s1.second[r]) return;
            }
            
            auto s2 = s1;
            if (gain == 1) {
                int l = offsets[0];
                if (!degrees[l]) {
                    update(s2, l, 1);
                    vector<bool> used(8, false);
                    for (int pos = 0; pos < D; pos++)
                        if (!((s2.first >> pos) & 1) && s2.second[pos]) used[s2.second[pos]] = true;
                    s2.second[l] = find_if(used.begin() + 1, used.end(), [&](bool u) { return !u; }) - used.begin();
                } else update(s2, l, 2);
            } else if (gain == 2) {
                auto [l, r] = offsets;
                if (!degrees[l] && !degrees[r]) {
                    update(s2, l, 1);
                    update(s2, r, 1);
                    vector<bool> used(8, false);
                    for (int pos = 0; pos < D; pos++)
                        if (!((s2.first >> pos) & 1) && s2.second[pos]) used[s2.second[pos]] = true;
                    s2.second[l] = s2.second[r] = find_if(used.begin() + 1, used.end(), [&](bool u) { return !u; }) - used.begin();
                } else if (degrees[l] == 1 && !degrees[r]) {
                    update(s2, l, 2);
                    update(s2, r, 1);
                    s2.second[r] = s1.second[l];
                } else if (!degrees[l] && degrees[r] == 1) {
                    update(s2, r, 2);
                    update(s2, l, 1);
                    s2.second[l] = s1.second[r];
                } else {
                    update(s2, l, 2);
                    update(s2, r, 2);
                    if (s1.second[l] != s1.second[r] && s1.second[r])
                        for (int pos = 0; pos < D; pos++)
                            if (!((s2.first >> pos) & 1) && s2.second[pos] == s1.second[r]) s2.second[pos] = s1.second[l];
                }
            }

            pair<int, array<int, 7>> s3;
            for (int pos = D - 1; pos; pos--) {
                int d = degree(s2, pos - 1);
                update(s3, pos, d);
                if (d == 1) s3.second[pos] = s2.second[pos - 1];
            }

            update(s3, 0, gain);
            if (gain == 1) s3.second[0] = !degrees[offsets[0]] ? s2.second[offsets[0]] : s1.second[offsets[0]];

            vector<int> seen(8, 0);
            for (int pos = 0, count = 1; pos < D; pos++)
                if (!((s3.first >> pos) & 1) && s3.second[pos]) {
                    int l = s3.second[pos];
                    if (!seen[l]) seen[l] = count++;
                    s3.second[pos] = seen[l];
                } else s3.second[pos] = 0;

            bool end = degree(s2, D - 1) == 1;
            int j = order[lower_bound(encoded.begin(), encoded.end(), encode(s3)) - encoded.begin()];
            if (single[i] + end == single[j] && node_id[j] != -1) next[mask].push_back({node_id[i], node_id[j], gain});
            else if (single[i] + end == 2 && !labeled[j]) last[mask].push_back({node_id[i], gain});
        };

        int mask = 0;
        for (int pos = 0; pos < D; pos++) {
            degrees[pos] = degree(states[i], pos);
            if (degrees[pos] < 2) mask |= 1 << pos;
        }
        add();

        for (int l = 0; l < D; l++)
            if ((mask >> l) & 1) {
                add(1 << l);
                for (int r = l + 1; r < D; r++)
                    if ((mask >> r) & 1) add((1 << l) | (1 << r));
            }
    }

    vector<vector<int>> valid(1 << D);
    for (auto m1 = 0U; m1 < 1 << D; m1++) {
        valid[m1].emplace_back(0);
        for (auto m2 = m1; m2; m2 = (m2 - 1) & m1)
            if (popcount(m2) <= 2) valid[m1].emplace_back(m2);
    }

    int len = 1;
    vector<int> memo(nodes, -3e4), temp(nodes);
    memo[node_id[order[lower_bound(encoded.begin(), encoded.end(), encode({})) - encoded.begin()]]] = 0;
    auto dp = [&](int m1 = 0) {
        fill(temp.begin(), temp.end(), -3e4);

        for (int m2 : valid[m1]) {
            for (auto [u, v, gain] : next[m2]) temp[v] = max(temp[v], memo[u] + gain);
            for (auto [v, gain] : last[m2]) len = max(len, memo[v] + gain + 1);
        }
        memo = temp;
    };
    for (int i = 0; i < n; i++) {
        int mask = 0;
        for (int pos = 1; pos <= min(D, i); pos++)
            if (abs(a[i] - a[i - pos]) <= m) mask |= 1 << (pos - 1);
        dp(mask);
    }
    while (D--) dp();
    cout << len;
}
