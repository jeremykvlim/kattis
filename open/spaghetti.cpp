#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto read = [&]() -> pair<vector<string>, vector<int>> {
        vector<string> lines;
        vector<int> labels;
        unordered_map<int, int> indices;
        string line;
        while (getline(cin, line) && !line.empty()) {
            if (line.substr(0, 5) != "     ") indices[stoi(line.substr(0, 5))] = lines.size();

            auto s = line.substr(6);
            s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());

            int i = s.size();
            for (; i && isdigit(s[i - 1]); i--);

            int label = i < s.size() ? stoi(s.substr(i)) : -1;
            if (s.substr(0, i) == "goto" || s.substr(0, 3) == "if(" && s.substr(i - 5, 5) == ")goto") {
                s.resize(i);
                labels.emplace_back(label);
            } else labels.emplace_back(-1);

            lines.emplace_back(s);
        }
        for (int &label : labels)
            if (~label) label = indices[label];

        return {lines, labels};
    };
    auto [lines1, labels1] = read();
    auto [lines2, labels2] = read();

    auto resolve = [&](const auto &lines, const auto &labels) {
        int n = lines.size();
        vector<int> state(n, 0), dest(n, -2);
        auto dfs = [&](auto &&self, int v) -> int {
            if (lines[v] != "goto") return dest[v] = v;
            if (state[v] == 1) return -1;
            if (state[v] == 2) return dest[v];

            state[v] = 1;
            dest[v] = self(self, labels[v]);
            state[v] = 2;
            return dest[v];
        };
        for (int i = 0; i < n; i++)
            if (dest[i] == -2) dfs(dfs, i);

        return dest;
    };
    auto dest1 = resolve(lines1, labels1), dest2 = resolve(lines2, labels2);

    vector<vector<bool>> visited(lines1.size(), vector<bool>(lines2.size(), false));
    auto equiv = [&](auto &&self, int u = 0, int v = 0) -> bool {
        u = ~u ? dest1[u] : -1;
        v = ~v ? dest2[v] : -1;

        if (!~u || !~v) return u == v;
        if (visited[u][v]) return true;
        if (lines1[u] != lines2[v]) return false;
        visited[u][v] = true;
        if (lines1[u] == "stop") return true;
        if (!self(self, u + 1, v + 1) || (~labels1[u] && !self(self, labels1[u], labels2[v]))) return false;
        return true;
    };
    cout << (equiv(equiv) ? "The programs are equivalent." : "The programs are not equivalent.");
}