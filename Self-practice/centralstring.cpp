#include <bits/stdc++.h>
using namespace std;

void compare(vector<string> &strs, vector<unordered_set<int>> &diff, string &s, int i) {
    for (int j = 0; j < diff.size(); j++)
        s[i] != strs[j][i] ? (void) diff[j].emplace(i) : (void) diff[j].erase(i);
}

void backtrack(vector<string> &strs, vector<unordered_set<int>> &diff, string &s, int d, int depth) {
    int j = find_if_not(diff.begin(), diff.end(), [&](auto i) {return i.size() <= d;}) - diff.begin();

    if (j == diff.size()) {
        cout << s;
        exit(0);
    }

    if (!depth) return;

    vector<int> indices;
    for (int i : diff[j]) {
        indices.emplace_back(i);
        if (indices.size() > d) break;
    }

    for (int i : indices) {
        char temp = s[i];
        s[i] = strs[j][i];
        compare(strs, diff, s, i);
        backtrack(strs, diff, s, d, depth - 1);
        s[i] = temp;
        compare(strs, diff, s, i);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, l, d;
    cin >> n >> l >> d;

    vector<string> strs(n);
    for (auto &a : strs) cin >> a;

    vector<unordered_set<int>> diff(n);
    for (int i = 0; i < l; i++) compare(strs, diff, strs[0], i);

    backtrack(strs, diff, strs[0], d, d);
    cout << 0;
}
