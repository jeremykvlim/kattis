#include <bits/stdc++.h>
using namespace std;

int find(int p, vector<int> &sets) {
    return (sets[p] == p) ? p : (sets[p] = find(sets[p], sets));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> sets(2 * n);
    iota(sets.begin(), sets.end(), 0);
    map<string, int> compress;
    vector<pair<int, int>> unequivalent;
    while (n--) {
        string X, s, Y;
        cin >> X >> s >> Y;

        reverse(X.begin(), X.end());
        reverse(Y.begin(), Y.end());

        int x = compress.emplace(X, compress.size()).first->second, y = compress.emplace(Y, compress.size()).first->second;
        if (s == "is") sets[find(x, sets)] = find(y, sets);
        else unequivalent.emplace_back(x, y);
    }

    for (auto x = compress.begin(), y = x; x != compress.end(); x = y) {
        y = next(x);
        while (y != compress.end()) {
            int len = min({(int) x->first.size(), (int) y->first.size(), 3});

            if (x->first.substr(0, len) == y->first.substr(0, len)) {
                sets[find(x->second, sets)] = find(y->second, sets);
                y++;
            } else break;
        }
    }

    bool consistent = true;
    for (auto &[x, y] : unequivalent)
        if (find(x, sets) ==  find(y, sets)) {
            consistent = false;
            break;
        }

    cout << (consistent ? "yes" : "wait what?");
}
