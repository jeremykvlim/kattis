#include <bits/stdc++.h>
using namespace std;

int find(int p, vector<int> &sets) {
    return (sets[p] == p) ? p : (sets[p] = find(sets[p], sets));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<int> capacity(n), links(n, 0), sets(n), size(n, 1);
    for (int &c : capacity) cin >> c;

    if (accumulate(capacity.begin(), capacity.end(), 0) < 2 * (n - 1)) {
        cout << "no";
        exit(0);
    }

    iota(sets.begin(), sets.end(), 0);

    while (m--) {
        int u, v;
        cin >> u >> v;

        int u_set = find(u, sets), v_set = find(v, sets);
        if (u_set != v_set) {
            sets[v_set] = u_set;
            capacity[u_set] += capacity[v_set];
            links[u_set] += links[v_set];
            size[u_set] += size[v_set];
        }

        capacity[u_set] -= 2;
        links[u_set]++;
    }

    vector<int> roots;
    for (int i = 0; i < n; i++)
        if (find(i, sets) == i) roots.emplace_back(i);

    if (roots.size() == 1) {
        cout << "yes";
        exit(0);
    }

    int isolated = 0, complete = 0, incomplete = 0, degree = 0;
    for (int s : roots) {
        degree += capacity[s];

        if (!capacity[s]) {
            k--;

            if (links[s] == size[s] - 1) complete += 2;
            else isolated++;

            degree += 2;
        } else if (capacity[s] == 1) {
            if (links[s] == size[s] - 1) complete++;
            else incomplete++;
        } else isolated++;
    }

    while (degree < 2 * (complete + incomplete)) {
        k--;
        degree += 2;

        if (incomplete) {
            incomplete--;
            isolated++;
        }
    }

    cout << (k < isolated + complete + incomplete - 1 ? "no" : "yes");
}
