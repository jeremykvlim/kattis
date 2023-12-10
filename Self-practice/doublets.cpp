#include <bits/stdc++.h>
using namespace std;

void print(int i, vector<int> &prev, vector<string> &dict) {
    if (i != -1) {
        print(prev[i], prev, dict);
        cout << dict[i] << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<string> dict;
    string word;
    while (getline(cin, word), !word.empty()) dict.emplace_back(word);
    sort(dict.begin(), dict.end());
    dict.resize(unique(dict.begin(), dict.end()) - dict.begin());

    int size = dict.size();
    auto longest = max_element(dict.begin(), dict.end(), [&](string s1, string s2) {return s1.size() < s2.size();})->size();
    vector<vector<int>> adj_list(size);
    vector<map<string, vector<int>>> sub(longest);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < dict[i].size(); j++) {
            string s = dict[i];
            s.erase(s.begin() + j);
            if (sub[j].find(s) != sub[j].end())
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

        print(end, prev, dict);
    }
}
