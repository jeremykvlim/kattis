#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> costs(n);
    for (int &c : costs) cin >> c;

    int m;
    cin >> m;
    
    vector<int> orders(m);
    for (int &s : orders) cin >> s;

    vector<long long> dp(*max_element(orders.begin(), orders.end()) + 1, 0);
    dp[0] = 1;
    for (int c : costs)
        for (int i = c; i < dp.size(); i++) dp[i] += dp[i - c];

    for (int s : orders) {
        if (!dp[s]) cout << "Impossible\n";
        else if (dp[s] > 1) cout << "Ambiguous\n";
        else {
            stack<int> st;
            for (int i = n - 1; ~i; i--)
                while (s >= costs[i] && dp[s - costs[i]]) {
                    st.emplace(i + 1);
                    s -= costs[i];
                }

            while (!st.empty()) {
                cout << st.top() << " ";
                st.pop();
            }
        }
    }
}
