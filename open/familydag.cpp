#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;
    for (;;) {
        unordered_set<string> seen;
        unordered_map<string, vector<string>> adj_list;
        while (getline(cin, line) && line != "done") {
            istringstream iss(line);
            string parent, _, child;
            iss >> parent >> _ >> child;

            adj_list[child].emplace_back(parent);
            seen.emplace(parent);
            seen.emplace(child);
        }
        if (seen.empty()) break;

        vector<string> people{seen.begin(), seen.end()};
        sort(people.begin(), people.end());

        vector<pair<string,string>> flagged;
        for (auto &person : people) {
            string first = person;
            bool p = false, h = false;
            unordered_set<string> visited;
            auto dfs = [&](auto &&self, const auto &v) -> void {
                for (const auto &u : adj_list[v]) {
                    if (u == first) {
                        p = true;
                        continue;
                    }

                    if (visited.count(u)) {
                        h = true;
                        continue;
                    }
                    visited.emplace(u);
                    self(self, u);
                }
            };
            dfs(dfs, person);
            if (p) flagged.emplace_back(person, "paradox");
            else if (h) flagged.emplace_back(person, "hillbilly");
        }

        for (auto [name, type] : flagged) cout << name << " " << type << "\n";
        cout << "\n";
    }
}
