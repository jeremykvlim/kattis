#include <bits/stdc++.h>
using namespace std;

void dfs(int i, vector<int> &prev, vector<string> &dict) {
    if (i != -1) {
        dfs(prev[i], prev, dict);
        cout << dict[i] << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string word;
    vector<string> dict;
    while (getline(cin, word), !word.empty()) dict.emplace_back(word);
    sort(dict.begin(), dict.end());
    dict.erase(unique(dict.begin(), dict.end()), dict.end());

    int size = dict.size();
    auto longest = max_element(dict.begin(), dict.end(), [&](auto s1, auto s2) {return s1.size() < s2.size();})->size();
    vector<vector<int>> adj_list(size);
    vector<map<string, vector<int>>> sub(longest);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < dict[i].size(); j++) {
            auto s = dict[i];
            s.erase(s.begin() + j);

            if (sub[j].count(s))
                for (int k : sub[j][s]) {
                    adj_list[i].emplace_back(k);
                    adj_list[k].emplace_back(i);
                }
            sub[j][s].emplace_back(i);
        }

    pair<string, string> p;
    bool blank = false;
    while (cin >> p.first >> p.second) {
        if (blank) cout << "\n";
        blank = true;

        int begin = lower_bound(dict.begin(), dict.end(), p.first) - dict.begin(), end = lower_bound(dict.begin(), dict.end(), p.second) - dict.begin();

        queue<int> q;
        vector<int> dist(size, -1), prev(size, -1);
        dist[begin] = 0;
        q.emplace(begin);
        while (!q.empty() && dist[end] == -1) {
            int v = q.front();
            q.pop();

            for (int u : adj_list[v])
                if (dist[u] == -1) {
                    dist[u] = dist[v] + 1;
                    prev[u] = v;
                    q.emplace(u);
                }
        }

        if (dist[end] == -1) {
            cout << "No solution.\n";
            continue;
        }

        dfs(end, prev, dict);
    }
}
