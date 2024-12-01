#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string word;
    vector<string> dict;
    while (getline(cin, word) && !word.empty()) dict.emplace_back(word);
    sort(dict.begin(), dict.end());
    dict.erase(unique(dict.begin(), dict.end()), dict.end());

    int size = dict.size();
    auto longest = max_element(dict.begin(), dict.end(), [&](auto s1, auto s2) {return s1.size() < s2.size();})->size();
    vector<vector<int>> adj_list(size);
    vector<unordered_map<string, vector<int>>> indices(longest);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < dict[i].size(); j++) {
            auto s = dict[i];
            s.erase(s.begin() + j);

            if (indices[j].count(s))
                for (int k : indices[j][s]) {
                    adj_list[i].emplace_back(k);
                    adj_list[k].emplace_back(i);
                }

            indices[j][s].emplace_back(i);
        }

    bool blank = false;
    string start, end;
    while (cin >> start >> end) {
        if (blank) cout << "\n";
        blank = true;

        int l = lower_bound(dict.begin(), dict.end(), start) - dict.begin(),
            r = lower_bound(dict.begin(), dict.end(), end) - dict.begin();

        vector<int> dist(size, -1), prev(size, -1);
        dist[l] = 0;
        queue<int> q;
        q.emplace(l);
        while (!q.empty() && dist[r] == -1) {
            int v = q.front();
            q.pop();

            for (int u : adj_list[v])
                if (dist[u] == -1) {
                    dist[u] = dist[v] + 1;
                    prev[u] = v;
                    q.emplace(u);
                }
        }

        if (dist[r] == -1) {
            cout << "No solution.\n";
            continue;
        }

        auto dfs = [&](auto &&self, int v) -> void {
            if (prev[v] != -1) self(self, prev[v]);
            cout << dict[v] << "\n";
        };

        dfs(dfs, r);
    }
}
