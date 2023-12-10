#include <bits/stdc++.h>
using namespace std;

struct Type {
    bool x = true, plus = true, minus = true;
    int colour = -1;
    vector<Type*> neutral;

    bool bipartite() {
        for (Type *n : neutral) {
            if (n->colour == -1) {
                n->colour = colour ^ 1;
                if (!n->bipartite()) return false;
            }
            if (colour == n->colour) return false;
        }
        return true;
    }

    void unplus() {
        plus = false;
        for (Type *n : neutral)
            if (n->minus) n->unminus();
    }

    void unminus() {
        minus = false;
        for (Type *n : neutral)
            if (n->plus) n->unplus();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<Type> types(n);
    while (m--) {
        int i, j;
        char c;
        cin >> i >> j >> c;

        if (c != 'x') types[i - 1].x = types[j - 1].x = false;

        if (c == '-') types[i - 1].plus = types[j - 1].plus = false;
        else if (c == '+') types[i - 1].minus = types[j - 1].minus = false;
        else if (c == '=') {
            types[i - 1].neutral.emplace_back(&types[j - 1]);
            types[j - 1].neutral.emplace_back(&types[i - 1]);
        }
    }

    for (Type &i : types) {
        if (i.x) continue;

        if (!i.plus) i.unplus();
        if (!i.minus) i.unminus();
        if (i.colour == -1) {
            i.colour = 0;
            if (!i.bipartite()) {
                i.unplus();
                i.unminus();
            }
        }
    }

    for (Type &i : types) {
        if (i.x) cout << 'x';
        else if (!i.plus && i.minus) cout << '-';
        else if (i.plus && !i.minus) cout << '+';
        else cout << '=';
    }
}
