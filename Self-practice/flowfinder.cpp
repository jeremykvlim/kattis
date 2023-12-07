#include <bits/stdc++.h>
using namespace std;

bool update(int curr, long long diff, vector<bool> &pending, vector<vector<int>> &children, vector<long long> &min_flow) {
    pending[curr] = false;
    min_flow[curr] += diff;
    if (children[curr].size() >= 2 && pending[children[curr][1]]) return false;
    else if (!children[curr].empty()) return update(children[curr].front(), diff, pending, children, min_flow);
    else return true;
}

bool dfs(int curr, vector<bool> &pending, vector<vector<int>> &children, vector<long long> &min_flow, vector<long long> &flow) {
    if (children[curr].empty()) {
        pending[curr] = !flow[curr];
        min_flow[curr] = max(flow[curr], 1LL);
        return true;
    }

    for (auto c : children[curr]) {
        if (!dfs(c, pending, children, min_flow, flow)) return false;
        min_flow[curr] += min_flow[c];
    }
    sort(children[curr].begin(), children[curr].end(), [&](int c1, int c2) {return pending[c1] > pending[c2];});

    if (flow[curr]) {
        pending[curr] = false;
        if (min_flow[curr] > flow[curr]) return false;
        auto diff = flow[curr] - min_flow[curr];
        min_flow[curr] += diff;
        if (diff)
            if (children[curr].size() >= 2 && pending[children[curr][1]] || !pending[children[curr].front()] ||
                !update(children[curr].front(), diff, pending, children, min_flow)) return false;
    } else pending[curr] = pending[children[curr].front()];

    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> children(n + 1);
    for (int i = 2; i <= n; i++) {
        int p;
        cin >> p;
        children[p].emplace_back(i);
    }

    vector<long long> flow(n + 1), min_flow(n + 1, 0);
    for (int i = 1; i <= n; i++) cin >> flow[i];

    vector<bool> pending(n + 1, true);
    if (dfs(1, pending, children, min_flow, flow) && !pending[1])
        for (int i = 1; i <= n; i++) cout << min_flow[i] << '\n';
    else cout << "impossible\n";
}
