#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c, s, f;
    string sigma;
    cin >> n >> c >> s >> f >> sigma;

    vector<int> final(f);
    for (int &fi : final) cin >> fi;
    sort(final.begin(), final.end());

    vector<vector<int>> table(n, vector<int>(c));
    for (auto &row : table)
        for (int &symb : row) cin >> symb;

    vector<vector<int>> adj_list_epsilon(n + 1);
    adj_list_epsilon[0].emplace_back(s);
    for (int fi : final) adj_list_epsilon[fi].emplace_back(0);

    auto epsilon_closure = [&](int state) {
        queue<int> q;
        for (int i = 0; i <= n; i++)
            if (state & (1 << i)) q.emplace(i);

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (int u : adj_list_epsilon[v])
                if (!(state & (1 << u))) {
                    state |= (1 << u);
                    q.emplace(u);
                }
        }
        return state;
    };

    unordered_map<int, int> indices;
    vector<int> states;
    int start = epsilon_closure(1);
    indices[start] = 1;
    states.emplace_back(start);

    vector<bool> final_states;
    auto check = [&](int state) {
        for (int i = 0; i <= n; i++)
            if (state & (1 << i))
                if (!i || binary_search(final.begin(), final.end(), i)) return true;
        return false;
    };
    final_states.emplace_back(check(start));

    vector<vector<int>> temp{vector<int>(c, 0)};
    queue<int> q;
    q.emplace(1);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        int curr = states[v - 1];

        for (int j = 0; j < c; j++) {
            int next = 0;
            for (int i = 1; i <= n; i++)
                if (curr & (1 << i)) next |= 1 << table[i - 1][j];

            next = epsilon_closure(next);
            if (!indices.count(next)) {
                int u = states.size() + 1;
                indices[next] = u;
                states.emplace_back(next);
                q.emplace(u);

                temp.emplace_back(vector<int>(c));
                final_states.emplace_back(check(next));
            }

            temp[v - 1][j] = indices[next];
        }
    }
    n = states.size();
    final.clear();
    for (int i = 0; i < n; i++)
        if (final_states[i]) final.emplace_back(i + 1);
    f = final.size();
    table = temp;

    cout << n << " " << c << " " << 1 << " " << f << "\n" << sigma << "\n";
    for (int fi : final) cout << fi << " ";
    cout << "\n";
    for (auto row : table) {
        for (int symb : row) cout << symb << " ";
        cout << "\n";
    }
}
