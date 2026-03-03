#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<string> names(n);
    unordered_map<string, int> indices;
    for (int i = 0; i < n; i++) {
        cin >> names[i];

        indices[names[i]] = i;
    }

    vector<bool> tracked(n, false);
    vector<int> next(n, -1);
    while (k--) {
        string s, t;
        cin >> s >> t;

        int v = indices[t];
        tracked[v] = true;
        next[indices[s]] = v;
    }

    vector<bool> visited(n, false);
    vector<pair<int, int>> pairs;
    for (int i = 0; i < n; i++)
        if (!tracked[i]) {
            int j = i;
            for (; ~j && !visited[j]; j = next[j]) {
                visited[j] = true;
                if (!~next[j]) break;
            }
            pairs.emplace_back(i, j);
        }

    for (int i = 0; i < pairs.size(); i++) {
        auto [l, ml] = pairs[i];
        auto [mr, r] = pairs[(i + 1) % pairs.size()];
        next[ml] = mr;
    }

    for (int i = 0; i < n; i++) cout << names[i] << " " << names[next[i]] << "\n";
}
