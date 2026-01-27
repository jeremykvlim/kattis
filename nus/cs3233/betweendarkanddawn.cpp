#include <bits/stdc++.h>
using namespace std;

struct ImplicitTreap {
    static inline mt19937_64 rng{random_device{}()};

    struct TreapNode {
        array<int, 3> family;
        unsigned long long prio;
        bool flip;
        char c, c_l, c_r;
        int base, len, pref_len, suff_len, count;
        array<int, 2> pos_l, pos_r;

        TreapNode(char c = '#', int l = 0) : family{0, 0, 0}, prio(rng()), flip(false), c(c), c_l(c), c_r(c), base(l), len(l), pref_len(l), suff_len(l), count(l - 1),
                                             pos_l{INT_MAX, INT_MAX}, pos_r{-1, -1} {
            if (l >= 2) {
                pos_l[c != 'C'] = 0;
                pos_r[c != 'C'] = l - 2;
            }
        }
    };

    vector<TreapNode> T;
    stack<int> recycled;
    int root;

    ImplicitTreap(int n, const vector<pair<char, int>> &runs) : T(n + 1), root(0) {
        for (int i = 1; i <= n; i++) T[i] = {runs[i - 1].first, runs[i - 1].second};

        deque<int> st{0};
        for (int i = 1; i <= n + 1; i++) {
            int j = 0;
            for (; st.size() > 1 && (i == n + 1 || T[st.back()].prio >= T[i].prio);) {
                j = st.back();
                st.pop_back();
            }

            if (i == n + 1) root = j;
            else {
                if (st.back()) {
                    T[i].family[2] = st.back();
                    T[st.back()].family[1] = i;
                }
                if (j) {
                    T[j].family[2] = i;
                    T[i].family[0] = j;
                }
                st.emplace_back(i);
            }
        }

        auto dfs = [&](auto &&self, int v) -> void {
            if (!v) return;
            auto [l, r, p] = T[v].family;
            self(self, l);
            self(self, r);
            pull(v);
        };
        dfs(dfs, root);
    }

    int node(char c, int l) {
        int i;
        if (!recycled.empty()) {
            i = recycled.top();
            recycled.pop();
        } else {
            T.emplace_back();
            i = T.size() - 1;
        }
        T[i] = {c, l};
        return i;
    }

    int base(int i) const {
        return !i ? 0 : T[i].base;
    }

    int len(int i) const {
        return !i ? 0 : T[i].len;
    }

    int pref_len(int i) const {
        return !i ? 0 : T[i].pref_len;
    }

    int suff_len(int i) const {
        return !i ? 0 : T[i].suff_len;
    }

    int count(int i) const {
        return !i ? 0 : T[i].count;
    }

    void pull(int i) {
        if (!i) return;
        auto [l, r, p] = T[i].family;

        T[i].len = len(l) + len(r) + base(i);
        T[i].count = count(l) + count(r) + base(i) - 1;
        T[i].c_l = l ? T[l].c_l : T[i].c;
        T[i].c_r = r ? T[r].c_r : T[i].c;
        T[i].pref_len = l ? pref_len(l) : base(i);
        T[i].suff_len = r ? suff_len(r) : base(i);
        T[i].pos_l = {INT_MAX, INT_MAX};
        T[i].pos_r = {-1, -1};
        for (int k = 0; k < 2; k++) {
            if (l) {
                T[i].pos_l[k] = min(T[i].pos_l[k], T[l].pos_l[k]);
                T[i].pos_r[k] = max(T[i].pos_r[k], T[l].pos_r[k]);
            }

            if ((T[i].c != 'C') == k && base(i) >= 2) {
                T[i].pos_l[k] = min(T[i].pos_l[k], len(l));
                T[i].pos_r[k] = max(T[i].pos_r[k], len(l) + base(i) - 2);
            }

            if (r) {
                if (T[r].pos_l[k] != INT_MAX) T[i].pos_l[k] = min(T[i].pos_l[k], len(l) + base(i) + T[r].pos_l[k]);
                if (T[r].pos_r[k] != -1) T[i].pos_r[k] = max(T[i].pos_r[k], len(l) + base(i) + T[r].pos_r[k]);
            }
        }
    }

    void flip(int i) {
        if (!i) return;
        auto &[l, r, p] = T[i].family;
        swap(l, r);
        T[i].flip = !T[i].flip;
        swap(T[i].c_l, T[i].c_r);
        swap(T[i].pref_len, T[i].suff_len);
        for (int k = 0; k < 2; k++) {
            int temp1 = T[i].pos_l[k], temp2 = T[i].pos_r[k];
            T[i].pos_l[k] = temp2 == -1 ? INT_MAX : len(i) - 2 - temp2;
            T[i].pos_r[k] = temp1 == INT_MAX ? -1 : len(i) - 2 - temp1;
        }
    }

    void push(int i) {
        if (!i) return;
        if (T[i].flip) {
            auto [l, r, p] = T[i].family;
            if (l) flip(l);
            if (r) flip(r);
            T[i].flip = false;
        }
    }

    void attach(int i, int c, int j) {
        T[i].family[c] = j;
        if (j) T[j].family[2] = i;
        pull(i);
    }

    pair<int, int> split(int i, int k) {
        if (!i) return {0, 0};
        push(i);
        auto [l, r, p] = T[i].family;
        int sl = len(l);
        if (k < sl) {
            auto [ll, lr] = split(l, k);
            attach(i, 0, lr);
            if (ll) T[ll].family[2] = 0;
            T[i].family[2] = 0;
            return {ll, i};
        } else if (k > sl + base(i)) {
            auto [rl, rr] = split(r, k - sl - base(i));
            attach(i, 1, rl);
            if (rr) T[rr].family[2] = 0;
            T[i].family[2] = 0;
            return {i, rr};
        } else if (k == sl) {
            attach(i, 0, 0);
            if (l) T[l].family[2] = 0;
            T[i].family[2] = 0;
            return {l, i};
        } else if (k == sl + base(i)) {
            attach(i, 1, 0);
            if (r) T[r].family[2] = 0;
            T[i].family[2] = 0;
            return {i, r};
        }

        T[i].base -= k - sl;
        if (l) T[l].family[2] = 0;
        T[i].family[0] = T[i].family[2] = 0;
        pull(i);
        return {insert(l, len(l), node(T[i].c, k - sl)), i};
    }

    int meld(int i, int j) {
        if (!i || !j) {
            int k = i ^ j;
            if (k) T[k].family[2] = 0;
            return k;
        }
        if (T[i].prio < T[j].prio) {
            push(i);
            attach(i, 1, meld(T[i].family[1], j));
            T[i].family[2] = 0;
            return i;
        } else {
            push(j);
            attach(j, 0, meld(i, T[j].family[0]));
            T[j].family[2] = 0;
            return j;
        }
    }

    int node_root(int i) {
        for (; T[i].family[2]; i = T[i].family[2]);
        return i;
    }

    int rank(int i) {
        auto propagate = [&](auto &&self, int v) {
            if (!v) return;
            self(self, T[v].family[2]);
            push(v);
        };
        propagate(propagate, i);
        int r = len(T[i].family[0]);
        while (T[i].family[2]) {
            int p = T[i].family[2];
            if (T[p].family[1] == i) r += len(T[p].family[0]) + base(p);
            i = p;
        }
        return r;
    }

    int insert(int i, int pos, int m) {
        auto [l, r] = split(i, pos);
        return meld(meld(l, m), r);
    }

    int erase(int i) {
        if (!i) return 0;
        auto [l, r] = split(node_root(i), rank(i));
        auto [rl, rr] = split(r, base(i));
        destroy(rl);
        return root = meld(l, rr);
    }

    void destroy(int i) {
        if (!i) return;
        push(i);
        auto [l, r, p] = T[i].family;
        destroy(l);
        destroy(r);
        T[i] = {};
        recycled.emplace(i);
    }

    vector<int> query() {
        vector<int> moves;
        while (count(root)) {
            int pos = T[root].pos_l[T[root].c_r == 'C'];
            moves.emplace_back(pos == INT_MAX ? 1 : pos + 1);
            auto [l, r] = split(root, moves.back());
            if (l) flip(l);
            if (T[l].c_l == T[r].c_r) {
                auto [ll, lr] = split(l, pref_len(l));
                auto [rl, rr] = split(r, len(r) - suff_len(r));
                T[rr].base += base(ll);
                pull(rr);
                erase(ll);
                r = meld(rl, rr);
                l = lr;
            }
            root = meld(r, l);
        }
        return moves;
    }

    auto & operator[](int i) {
        return T[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    string s;
    cin >> n >> s;

    int C = 0, L = 0;
    for (char c : s) (c == 'C' ? C : L)++;

    if ((!(n & 1) && C != L) || (n & 1 && abs(C - L) > 1)) {
        cout << "IMPOSSIBLE";
        exit(0);
    }

    vector<pair<char, int>> runs;
    for (int l = 0, r = 1; l < n; l = r++) {
        for (; r < n && s[l] == s[r]; r++);
        runs.emplace_back(s[l], r - l);
    }

    ImplicitTreap treap(runs.size(), runs);
    auto moves = treap.query();
    cout << moves.size() << "\n";
    for (int len : moves) cout << len << " ";
}
