#include <bits/stdc++.h>
using namespace std;

int find(int p, vector<int> &sets) {
    return (sets[p] == p) ? p : (sets[p] = find(sets[p], sets));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n;
        cin >> n;

        vector<bool> visited(2 * (n + 1), false);
        vector<int> sets(2 * (n + 1));
        iota(sets.begin(), sets.end(), 0);

        bool possible = true;
        while (n--) {
            int p, q;
            cin >> p >> q;
            if (!possible) continue;

            int p_set = find(p, sets), q_set = find(q, sets);
            if (visited[p_set] && visited[q_set]) possible = false;

            if (p_set != q_set) {
                sets[q_set] = p_set;
                visited[p_set] = visited[p_set] | visited[q_set];
            } else visited[p_set] = true;
        }

        cout << (possible ? "possible\n" : "impossible\n");
    }
}
