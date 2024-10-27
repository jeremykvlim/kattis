#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<string> lines;
    unordered_map<int, int> p1, p2;
    vector<int> match;
    int total = 0, second = 0;
    auto read = [&](unordered_map<int, int> &program, int l, int &r) {
        string s;
        while (getline(cin, s)) {
            if (s.empty()) {
                lines.emplace_back(s);
                match.emplace_back(0);
                second = total++;
                break;
            }

            if (s.substr(0, 5) != "     ") program[stoi(s.substr(0, 5))] = total;
            s = s.substr(6);
            s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());

            int i = s.size() - 1;
            while (i && isdigit(s[i])) i--;
            int label = (isdigit(s[++i]) ? stoi(s.substr(i)) : 0);

            if (i == 4 && s.substr(0, 4) == "goto") {
                s.resize(4);
                match.emplace_back(label);
            } else if (s.substr(0, 3) == "if(" && s.substr(i - 5, 5) == ")goto") {
                s.resize(i);
                match.emplace_back(label);
            } else match.emplace_back(0);

            lines.emplace_back(s);
            if (lines[total++] == "stop" && second) break;
        }

        for (int i = l; i < r; i++)
            if (match[i]) match[i] = program[match[i]];
    };
    read(p1, 0, second);
    read(p2, second, total);

    vector<bool> cycle(total, false);
    auto dfs = [&](auto &&self, int v) -> bool {
        if (cycle[v]) return true;
        if (lines[v] != "goto") return false;

        cycle[v] = true;
        if (self(self, match[v])) return true;
        return cycle[v] = false;
    };
    for (int i = 0; i < total; i++) dfs(dfs, i);

    vector<vector<bool>> visited(total, vector<bool>(total, false));
    auto equiv = [&](auto &&self, int v, int u) {
        if (visited[v][u]) return true;
        visited[v][u] = true;

        if (cycle[v] && cycle[u]) return true;
        if (cycle[v] != cycle[u]) return false;

        if (lines[v] == "goto") return self(self, match[v], u);
        if (lines[u] == "goto") return self(self, v, match[u]);

        if (lines[v] != lines[u]) return false;
        if (lines[v] == "stop") return true;

        if (!self(self, v + 1, u + 1) || match[v] && !self(self, match[v], match[u])) return false;
        return true;
    };

    cout << (equiv(equiv, 0, second + 1) ? "The programs are equivalent." : "The programs are not equivalent.");
}
