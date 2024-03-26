#include <bits/stdc++.h>
using namespace std;

int dfs(int person, int committees, int &curr, vector<int> &hostile, vector<int> &members) {
    if (curr <= committees) return curr;
    if (person == hostile.size()) {
        curr = min(curr, committees);
        return committees;
    }

    int least = hostile.size();
    for (int i = 0; i <= committees; i++)
        if (!(hostile[person] & members[i])) {
            members[i] |= 1 << person;
            least = min(least, dfs(person + 1, committees + (i == committees), curr, hostile, members));
            members[i] ^= 1 << person;
        }

    return least;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    while (cin >> n >> m && n || m) {
        unordered_map<string, int> compress;
        vector<int> hostile(n), members(n);
        while (m--) {
            string a, b;
            cin >> a >> b;
            int p1 = compress.count(a) ? compress[a] : compress[a] = compress.size(), p2 = compress.count(b) ? compress[b] : compress[b] = compress.size();
            hostile[p1] |= 1 << p2;
            hostile[p2] |= 1 << p1;
        }

        int curr = n;
        cout << dfs(0, 0, curr, hostile, members) << "\n";
    }
}
