#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<bool> visited(1 << k);
    queue<int> q;
    while (n--) {
        string s;
        cin >> s;
        
        int v = (int) bitset<20>(s).to_ulong();

        if (!visited[v]) {
            q.emplace(v);
            visited[v] = true;
        }
    }

    int tira;
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (int i = 0; i < k; i++) {
            int u = v ^ (1 << i);
            if (!visited[u]) {
                visited[u] = true;
                q.emplace(u);
                tira = u;
            }
        }
    }

    cout << bitset<20>(tira).to_string().substr(20 - k);
}
