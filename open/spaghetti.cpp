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

            auto label = s.substr(0, 5);
            if (!label.empty() && label != "     ") program[stoi(label)] = total;

            s = s.substr(6, s.size() - 6);
            s.erase(remove_if(s.begin(), s.end(), [](char c) {return c == ' ';}), s.end());
            
            int i = s.size() - 1;
            while (i && isdigit(s[i])) i--;
            int lab = (isdigit(s[++i]) ? stoi(s.substr(i)) : 0);

            if (i == 4 && s.substr(0, 4) == "goto") {
                s.resize(4);
                match.emplace_back(lab);
            } else if (s.substr(0, 3) == "if(" && s.substr(i - 5, 5) == ")goto") {
                s.resize(i);
                match.emplace_back(lab);
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
    auto dfs = [&](auto &&self, int a) -> bool {
        if (cycle[a]) return true;
        if (lines[a] != "goto") return false;

        cycle[a] = true;
        if (self(self, match[a])) return true;
        return cycle[a] = false;
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

    if (equiv(equiv, 0, second + 1)) cout << "The programs are equivalent.";
    else cout << "The programs are not equivalent.";
}
