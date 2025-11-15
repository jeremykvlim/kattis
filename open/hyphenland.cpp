#include <bits/stdc++.h>
using namespace std;
 
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int m;
    cin >> m;
 
    vector<pair<string, int>> names;
    auto parse = [&]() {
        string s;
        cin >> s;
 
        string t;
        for (char c : s)
            if (c == '-') {
                names.emplace_back(t, t.size());
                t.clear();
            } else t += c;
 
        if (!t.empty()) names.emplace_back(t, t.size());
    };
    parse();
 
    while (m--) {
        int g, n;
        cin >> g >> n;
 
        while (n--) {
            parse();
 
            int l = 0;
            for (auto [part, len] : names) l += len;
 
            for (; l > g; l--) {
                int longest = 0, j = -1;
                for (int i = 0; i < names.size(); i++)
                    if (longest < names[i].second) {
                        longest = names[i].second;
                        j = i;
                    }
                if (longest < 2) break;
                names[j].first.pop_back();
                names[j].second--;
            }
        }
    }
 
    for (int i = 0; i < names.size(); i++) {
        cout << names[i].first;
        if (i + 1 < names.size()) cout << "-";
    }
}
 
