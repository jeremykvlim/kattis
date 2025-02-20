#include <bits/stdc++.h>
using namespace std;

struct SplayTree {
    struct SplayNode {
        array<int, 3> family;
        bool flip;
        char c, c_l, c_r;
        int count, size;
        array<int, 2> pos_l, pos_r;
        SplayNode(char c = '#') : family{0, 0, 0}, flip(false), c(c), c_l(c), c_r(c), count(0), size(c != '#'), pos_l{(int) 1e9, (int) 1e9}, pos_r{-1, -1} {}
    };

    vector<SplayNode> ST;

    SplayTree(const string &s) : ST(s.size() + 1) {
        int n = s.size();
        for (int i = 0; i < n; i++) ST[i + 1] = s[i];

        for (int i = 1; i < n; i++) {
            ST[i].family[1] = i + 1;
            ST[i + 1].family[2] = i;
        }
        for (int i = n; i; i--) pull(i);
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
        SplayNode m(ST[i].c);
        auto add = [&](SplayNode nl, SplayNode nr) {
            if (!nl.size) return nr;
            if (!nr.size) return nl;

            SplayNode node;
            node.size = nl.size + nr.size;
            node.c_l = nl.c_l;
            node.c_r = nr.c_r;
            node.count = nl.count + nr.count + (nl.c_r == nr.c_l);
            for (int k = 0; k < 2; k++) {
                node.pos_l[k] = min(nl.pos_l[k], nr.pos_l[k] == 1e9 ? (int) 1e9 : nr.pos_l[k] + nl.size);
                node.pos_r[k] = max(nl.pos_r[k], nr.pos_r[k] == -1 ? -1 : nr.pos_r[k] + nl.size);
            }
            if (nl.c_r == nr.c_l) {
                int k = nl.c_r == 'L';
                node.pos_l[k] = min(node.pos_l[k], nl.size - 1);
                node.pos_r[k] = max(node.pos_r[k], nl.size - 1);
            }
            return node;
        };
        auto added = add(add(ST[l], m), ST[r]);
        tie(ST[i].size, ST[i].c_l, ST[i].c_r, ST[i].count, ST[i].pos_l, ST[i].pos_r) = tie(added.size, added.c_l, added.c_r, added.count, added.pos_l, added.pos_r);
    }

    void flip(int i) {
        if (!i) return;
        auto &[l, r, p] = ST[i].family;
        swap(l, r);
        ST[i].flip = !ST[i].flip;
        swap(ST[i].c_l, ST[i].c_r);
        auto temp = ST[i].pos_l;
        for (int k = 0; k < 2; k++) {
            if (~ST[i].pos_r[k]) ST[i].pos_l[k] = ST[i].size - 2 - ST[i].pos_r[k];
            if (temp[k] != 1e9) ST[i].pos_r[k] = ST[i].size - 2 - temp[k];
        }
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

    int subtree_maximum(int i) {
        while (ST[i].family[1]) {
            push(i);
            i = ST[i].family[1];
        }
        push(i);
        return i;
    }

    int meld(int i, int j) {
        if (!i || !j) return i ^ j;
        i = subtree_maximum(i);
        splay(i);
        ST[i].family[1] = j;
        ST[j].family[2] = i;
        pull(i);
        return i;
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

    SplayTree st(s);
    int i = 0;
    for (int j = 1; j <= n; j++)
        if (!st[j].family[2]) {
            i = j;
            break;
        }

    vector<int> moves;
    while (st[i].count) {
        char c = st[st.subtree_maximum(i)].c;
        int j = (st[i].pos_l[c == 'C'] == 1e9 ? 1 : st[i].pos_l[c == 'C'] + 1);
        moves.emplace_back(j);
        auto [l, r] = st.split(i, j);
        if (l) st.flip(l);
        i = st.meld(r, l);
    }
    cout << moves.size() << "\n";
    for (int l : moves) cout << l << " ";
}
