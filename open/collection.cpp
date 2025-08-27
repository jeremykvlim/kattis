#include <bits/stdc++.h>
using namespace std;

struct SplayTree {
    struct SplayNode {
        array<int, 3> family;
        int size, key, path_min, path_max;

        SplayNode(int key = 0) : family{0, 0, 0}, size(1), key(key), path_min(key), path_max(key) {}
    };

    vector<SplayNode> ST;

    SplayTree(int n) : ST(n + 1) {
        iota(ST.begin(), ST.end(), 0);
    }

    auto & operator[](int i) {
        return ST[i];
    }

    int size(int i) {
        return !i ? 0 : ST[i].size;
    }

    int key(int i) {
        return !i ? 0 : ST[i].key;
    }

    int path_min(int i) {
        return !i ? INT_MAX : ST[i].path_min;
    }

    int path_max(int i) {
        return !i ? INT_MIN : ST[i].path_max;
    }

    void pull(int i) {
        if (!i) return;
        auto [l, r, p] = ST[i].family;
        ST[i].size = size(l) + size(r) + 1;
        ST[i].path_min = min({path_min(l), path_min(r), key(i)});
        ST[i].path_max = max({path_max(l), path_max(r), key(i)});
    }

    void push(int i) {
        if (!i) return;
    }

    void splay(int i) {
        auto root = [&](int i) {
            auto [l, r, p] = ST[ST[i].family[2]].family;
            return !i || l != i && r != i;
        };

        auto child = [&](int i, int parent) { return ST[parent].family[1] == i; };

        auto rotate = [&](int i) {
            int j = ST[i].family[2], k = ST[j].family[2];
            if (!root(j)) ST[k].family[child(j, k)] = i;

            int c = child(i, j), s = ST[j].family[c] = ST[i].family[c ^ 1];
            if (s) ST[s].family[2] = j;

            ST[i].family[c ^ 1] = j;
            ST[i].family[2] = k;
            ST[j].family[2] = i;
            pull(j);
        };

        auto propagate = [&](auto &&self, int i) -> void {
            if (!root(i)) self(self, ST[i].family[2]);
            push(i);
        };

        propagate(propagate, i);
        while (!root(i)) {
            int j = ST[i].family[2], k = ST[j].family[2];
            if (!root(j)) rotate(child(i, j) != child(j, k) ? i : j);
            rotate(i);
        }
        pull(i);
    }

    pair<int, int> split(int i, int k) {
        if (!i) return {0, 0};
        for (;;) {
            push(i);
            auto [l, r, p] = ST[i].family;
            int sl = size(l);
            if (k <= sl) {
                if (!l) {
                    splay(i);
                    return {0, i};
                }
                i = l;
            } else if (k == sl + 1) {
                splay(i);
                r = ST[i].family[1];
                ST[i].family[1] = 0;
                pull(i);
                if (r) ST[r].family[2] = 0;
                return {i, r};
            } else {
                k -= sl + 1;
                if (!r) {
                    splay(i);
                    return {i, 0};
                }
                i = r;
            }
        }
    }

    int subtree_max(int i) {
        while (ST[i].family[1]) {
            push(i);
            i = ST[i].family[1];
        }
        push(i);
        return i;
    }

    int meld(int i, int j) {
        if (!i || !j) return i ^ j;
        i = subtree_max(i);
        splay(i);
        ST[i].family[1] = j;
        ST[j].family[2] = i;
        pull(i);
        return i;
    }

    int kth(int i, int k) {
        if (!i) return 0;
        for (;;) {
            push(i);
            auto [l, r, p] = ST[i].family;
            int sl = size(l);
            if (k <= sl) i = l;
            else if (k == sl + 1) {
                splay(i);
                return i;
            } else {
                k -= sl + 1;
                i = r;
            }
        }
    }

    pair<int, int> split_by_key(int i, int key) {
        if (!i) return {0, 0};
        int j = i, p = 0;
        while (j) {
            push(j);
            if (key <= ST[j].key) {
                p = j;
                j = ST[j].family[0];
            } else j = ST[j].family[1];
        }

        if (p) {
            splay(p);
            int l = ST[p].family[0];
            if (l) ST[l].family[2] = 0;
            ST[p].family[0] = 0;
            pull(p);
            return {l, p};
        } else {
            i = subtree_max(i);
            splay(i);
            int r = ST[i].family[1];
            if (r) ST[r].family[2] = 0;
            ST[i].family[1] = 0;
            pull(i);
            return {i, 0};
        }
    }

    int meld_by_key(int i, int j) {
        if (!i || !j) return i ^ j;
        if (path_max(i) < path_min(j)) return meld(i, j);
        if (path_max(j) < path_min(i)) return meld(j, i);

        push(i);
        auto [l, r, p] = ST[i].family;
        if (l) ST[l].family[2] = 0;
        if (r) ST[r].family[2] = 0;

        auto [ll, rr] = split_by_key(j, ST[i].key);

        if (!l || !ll) ll ^= l;
        else if (path_max(l) < path_min(ll)) ll = meld(l, ll);
        else if (path_max(ll) < path_min(l)) ll = meld(ll, l);
        else ll = meld_by_key(l, ll);

        if (ll) ST[ll].family[2] = i;
        ST[i].family[0] = ll;

        if (!r || !rr) rr ^= r;
        else if (path_max(r) < path_min(rr)) rr = meld(r, rr);
        else if (path_max(rr) < path_min(r)) rr = meld(rr, r);
        else rr = meld_by_key(r, rr);

        if (rr) ST[rr].family[2] = i;
        ST[i].family[1] = rr;

        pull(i);
        return i;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    stack<int> s;
    SplayTree st(n);
    vector<int> root(n + 1);
    iota(root.begin(), root.end(), 0);
    while (q--) {
        int t, i, j;
        cin >> t >> i >> j;

        if (t == 1) {
            int idx = st.kth(root[i], j);
            cout << st[idx].key << "\n";
            root[i] = idx;
        } else if (t == 2) {
            root[i] = st.meld_by_key(root[i], root[j]);
            s.emplace(j);
        } else {
            auto [l, r] = st.split(root[i], j);
            root[i] = l;
            root[s.top()] = r;
            s.pop();
        }
    }
}
