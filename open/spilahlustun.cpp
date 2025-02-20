#include <bits/stdc++.h>
using namespace std;

struct SplayTree {
    struct SplayNode {
        array<int, 3> family;
        bool flip, dir;
        int id, size;

        SplayNode(int i = 0) : family{0, 0, 0}, flip(false), dir(false), id(i), size(1) {}
    };

    vector<SplayNode> ST;

    SplayTree(int n) : ST(n + 1) {
        iota(ST.begin(), ST.end(), 0);
        for (int i = 1; i < n; i++) {
            ST[i].family[1] = i + 1;
            ST[i + 1].family[2] = i;
            pull(i);
        }
        pull(n);
    }

    auto & operator[](int i) {
        return ST[i];
    }

    int size(int i) {
        return !i ? 0 : ST[i].size;
    }

    void pull(int i) {
        if (!i) return;
        auto [l, r, p] = ST[i].family;
        ST[i].size = size(l) + size(r) + 1;
    }

    void flip(int i) {
        if (!i) return;
        auto &[l, r, p] = ST[i].family;
        swap(l, r);
        ST[i].flip = !ST[i].flip;
        ST[i].dir = !ST[i].dir;
    }

    void push(int i) {
        if (!i) return;
        if (ST[i].flip) {
            auto [l, r, p] = ST[i].family;
            if (l) flip(l);
            if (r) flip(r);
            ST[i].flip = false;
        }
    }

    void splay(int i) {
        auto root = [&](int i) {
            auto [l, r, p] = ST[ST[i].family[2]].family;
            return !i || l != i && r != i;
        };

        auto child = [&](int i, int parent) {return ST[parent].family[1] == i;};

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

    int meld(int i, int j) {
        if (!i || !j) return i ^ j;
        while (ST[i].family[1]) {
            push(i);
            i = ST[i].family[1];
        }
        push(i);
        splay(i);
        ST[i].family[1] = j;
        ST[j].family[2] = i;
        pull(i);
        return i;
    }

    vector<int> in_order_traversal(int i) {
        vector<int> order;
        auto dfs = [&](auto &&self, int i) {
            if (!i) return;
            push(i);
            auto [l, r, p] = ST[i].family;
            self(self, l);
            order.emplace_back(ST[i].dir ? -ST[i].id : ST[i].id);
            self(self, r);
        };
        dfs(dfs, i);
        return order;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    SplayTree st(n);
    int top = 1;
    while (q--) {
        int i, j;
        cin >> i >> j;

        auto [l, ml] = st.split(top, i - 1);
        auto [mr, r] = st.split(ml, j - i + 1);
        if (mr) st.flip(mr);
        top = st.meld(st.meld(l, mr), r);
    }
    for (int k : st.in_order_traversal(top)) cout << k << " ";
}
