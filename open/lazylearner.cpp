#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    int n, Q;
    cin >> s >> n >> Q;

    vector<string> words(n);
    for (auto &w : words) cin >> w;
    sort(words.begin(), words.end());

    vector<int> offset(n + 1), letters;
    for (int i = 0; i < n; i++) {
        offset[i] = letters.size();
        for (char c : words[i]) letters.emplace_back(c - 'a');
        if (words[i].size() > 10) words[i].resize(10);
    }
    offset[n] = letters.size();

    int m = s.size();
    vector<array<int, 26>> next(m + 2);
    for (int c = 0; c < 26; c++) next[m + 1][c] = m + 1;
    for (int i = m; i; i--) {
        next[i] = next[i + 1];
        next[i][s[i - 1] - 'a'] = i;
    }

    auto index = [&](int l, int r) {
        return (l - 1) * (2 * m - l + 2) / 2 + r - l;
    };

    vector<vector<int>> queries(m * (m + 1) / 2);
    vector<int> K(Q), r_max(m + 1, 0);
    for (int q = 0; q < Q; q++) {
        int l, r;
        cin >> l >> r >> K[q];

        queries[index(l, r)].emplace_back(q);
        r_max[l] = max(r_max[l], r);
    }

    int size = ceil(sqrt(n)), blocks = (n + size - 1) / size;
    bitset<(int) 2e4> active;
    vector<int> count(blocks), indices(Q, -1);
    vector<vector<int>> add(m + 1);
    for (int l = 1; l <= m; l++) {
        if (!r_max[l]) continue;

        vector<int> undo;
        for (int i = 0; i < n; i++) {
            if (offset[i + 1] - offset[i] > r_max[l] - l + 1) continue;

            int r = l - 1;
            for (int j = offset[i]; j < offset[i + 1]; j++) {
                r = next[r + 1][letters[j]];
                if (r > r_max[l]) goto skip;
            }
            if (add[r].empty()) undo.emplace_back(r);
            add[r].emplace_back(i);
            skip:;
        }

        active.reset();
        fill(count.begin(), count.end(), 0);
        for (int r = l; r <= r_max[l]; r++) {
            for (int i : add[r]) {
                active[i] = true;
                count[i / size]++;
            }

            for (int q : queries[index(l, r)]) {
                int b = 0, k = K[q];
                for (; b < blocks && k > count[b]; k -= count[b++]);
                if (b != blocks)
                    for (int i = b * size; i < min(n, (b + 1) * size); i++)
                        if (active[i])
                            if (!--k) {
                                indices[q] = i;
                                break;
                            }
            }
        }

        for (int r : undo) add[r].clear();
    }

    for (int i : indices)
        if (!~i) cout << "NO SUCH WORD\n";
        else cout << words[i] << "\n";
}
