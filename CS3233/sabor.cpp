#include <bits/stdc++.h>
using namespace std;

void estimate(int mp, string &parties, vector<int> &arguments, vector<vector<int>> &adj_list) {
    for (int v : adj_list[mp]) {
        arguments[mp] += (parties[v] == parties[mp]) ? -1 : 1;
        arguments[v] += (parties[v] == parties[mp]) ? -1 : 1;
    }

    parties[mp] ^= 'A' ^ 'B';
    for (int i : adj_list[mp])
        if (arguments[i] > 2) estimate(i, parties, arguments, adj_list);
}

int main () {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;

    string parties(n, 'A');
    vector<int> arguments(n);
    vector<vector<int>> adj_list(n);

    for (int i = 0; i < 5; i++) {
        int p;
        cin >> p;

        while (p--) {
            int k, l;
            cin >> k >> l;
            
            adj_list[k - 1].emplace_back(l - 1);
            adj_list[l - 1].emplace_back(k - 1);
            arguments[k - 1]++;
            arguments[l - 1]++;
        }
    }

    for (int i = 0; i < n; i++)
        if (arguments[i] > 2) estimate(i, parties, arguments, adj_list);

    cout << parties;
}
