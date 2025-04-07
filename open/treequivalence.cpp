#include <bits/stdc++.h>
using namespace std;

bool isanagram(string s1, string s2) {
    if (s1.size() != s2.size()) return false;
    sort(s1.begin(), s1.end());
    sort(s2.begin(), s2.end());
    return s1 == s2;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        string s1, s2;
        cin >> s1 >> s2;

        vector<vector<int>> adj_list1, adj_list2;
        auto decompose = [&](string s, vector<vector<int>> &adj_list) {
            stack<int> st;
            string labels;
            for (char c : s)
                if (c == ',' || c == ')') {
                    int u = st.top();
                    st.pop();
                    
                    int v = st.top();
                    adj_list[u].emplace_back(v);
                    adj_list[v].emplace_back(u);
                } else if (isalpha(c)) {
                    adj_list.emplace_back();
                    st.emplace(labels.size());
                    labels += c;
                }
            return labels;
        };
        auto labels1 = decompose(s1, adj_list1), labels2 = decompose(s2, adj_list2);

        if (!isanagram(labels1, labels2)) {
            cout << "different\n";
            continue;
        }

        if (labels1.size() == 1) {
            cout << "same\n";
            continue;
        }

        auto dfs = [&](auto &&self, int v1, int v2, int prev1 = -1, int prev2 = -1) -> bool {
            if (adj_list1[v1].size() != adj_list2[v2].size()) return false;

            int n = adj_list1[v1].size();
            for (int k = 0; k < n; k++) {
                for (int i = 0, j = k; i < n; i++, ++j %= n) {
                    int u1 = adj_list1[v1][i], u2 = adj_list2[v2][j];
                    if ((u1 == prev1) ^ (u2 == prev2) || adj_list1[u1].size() != adj_list2[u2].size() || labels1[u1] != labels2[u2]) goto next;
                }

                for (int i = 0, j = k; i < n; i++, ++j %= n) {
                    int u1 = adj_list1[v1][i], u2 = adj_list2[v2][j];
                    if (u1 != prev1)
                        if (!self(self, u1, u2, v1, v2)) goto next;
                }
                return true;

                next:;
            }
            return false;
        };

        for (int i = 0; i < labels1.length(); i++)
            if (labels1[0] == labels2[i] && adj_list1[0].size() == adj_list2[i].size() && dfs(dfs, 0, i)) {
                cout << "same\n";
                goto done;
            }

        cout << "different\n";
        done:;
    }
}
