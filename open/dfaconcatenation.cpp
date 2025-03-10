#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto read = [&]() -> tuple<int, int, int, int, string, vector<int>, vector<vector<int>>> {
        int n, c, s, f;
        string sigma;
        cin >> n >> c >> s >> f >> sigma;

        vector<int> final(f);
        for (int &fi : final) cin >> fi;

        vector<vector<int>> table(n, vector<int>(c));
        for (int i = 0; i < n; i++)
            for (int &symb : table[i]) cin >> symb;

        return {n, c, s, f, sigma, final, table};
    };
    auto [n1, c1, s1, f1, sigma1, final1, table1] = read();
    auto [n2, c2, s2, f2, sigma2, final2, table2] = read();

    int n = n1 + n2;
    vector<vector<int>> table(n, vector<int>(c1, 0));
    for (int i = 0; i < n1; i++)
        for (int j = 0; j < c1; j++) table[i][j] = table1[i][j];

    for (int i = 0; i < n2; i++)
        for (int j = 0; j < c1; j++) table[i + n1][j] = table2[i][j] + n1;

    vector<vector<int>> adj_list_epsilon(n + 1);
    for (int fi : final1) adj_list_epsilon[fi].emplace_back(s2 + n1);

    auto epsilon_closure = [&](int state) {
        queue<int> q;
        for (int i = 0; i <= n; i++)
            if ((state >> i) & 1) q.emplace(i);

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (int u : adj_list_epsilon[v])
                if (!((state >> u) & 1)) {
                    state |= (1 << u);
                    q.emplace(u);
                }
        }
        return state;
    };

    unordered_map<int, int> indices;
    vector<int> states;
    int start = epsilon_closure(1 << s1);
    indices[start] = 0;
    states.emplace_back(start);

    vector<bool> final_states;
    auto check = [&](int state) {
        for (int fi : final2)
            if ((state >> (n1 + fi)) & 1) return true;
        return false;
    };
    final_states.emplace_back(check(start));

    vector<vector<int>> temp{vector<int>(c1, 0)};
    queue<int> q;
    q.emplace(0);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        int curr = states[v];

        for (int j = 0; j < c1; j++) {
            int next = 0;
            for (int i = 1; i <= n; i++)
                if ((curr >> i) & 1) next |= 1 << table[i - 1][j];

            next = epsilon_closure(next);
            if (!indices.count(next)) {
                int u = states.size();
                indices[next] = u;
                states.emplace_back(next);
                q.emplace(u);

                temp.emplace_back(vector<int>(c1));
                final_states.emplace_back(check(next));
            }

            temp[v][j] = indices[next];
        }
    }
    n = states.size();
    vector<int> final;
    for (int i = 0; i < n; i++)
        if (final_states[i]) final.emplace_back(i + 1);
    int f = final.size();
    table = temp;

    cout << n << " " << c1 << " " << 1 << " " << f << "\n" << sigma1 << "\n";
    for (int fi : final) cout << fi << " ";
    cout << "\n";
    for (auto row : table) {
        for (int symb : row) cout << symb + 1 << " ";
        cout << "\n";
    }
}
