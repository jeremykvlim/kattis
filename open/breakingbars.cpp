#include <bits/stdc++.h>
using namespace std;

struct Hash {
    size_t operator()(const pair<vector<int>, array<int, 5>> &p) const {
        auto h = 0ULL;
        for (int e : p.first) h ^= e + 0x9e3779b9 + (h << 6) + (h >> 2);
        for (int e : p.second) h ^= e + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<vector<int>> indices(7, vector<int>(7));
    vector<int> a(22), b(22);
    int id = 0;
    for (int i = 6; i >= 1; i--)
        for (int j = i; j >= 1; j--) {
            indices[i][j] = indices[j][i] = ++id;
            a[id] = i;
            b[id] = j;
        }

    int n, t;
    cin >> n >> t;

    vector<int> count(22, 0);
    int total = 0;
    while (n--) {
        char _;
        int ai, bi;
        cin >> ai >> _ >> bi;

        count[indices[ai][bi]]++;
        total += ai * bi;
    }

    unordered_map<pair<vector<int>, array<int, 5>>, int, Hash> memo;
    int least = INT_MAX;
    auto dp = [&](auto &&self, int pos = 1, int breaks = 0, int sum = 0, int used = 0, int grandchild = 0) {
        if (least <= breaks || total - used < t * 2 || pos > id) return INT_MAX;
        if (memo.count({count, {pos, breaks, sum, used, grandchild}})) return memo[{count, {pos, breaks, sum, used, grandchild}}];

        int squares = 0;
        for (int i = pos; i <= id; i++) squares += (count[i] / 2) * a[i] * b[i];
        if (squares + sum >= t) {
            least = min(least, breaks);
            return memo[{count, {pos, breaks, sum, used, grandchild}}] = breaks;
        }

        int B = self(self, pos + 1, breaks, sum + (count[pos] / 2) * a[pos] * b[pos], used + (count[pos] % 2) * a[pos] * b[pos]);
        if (count[pos] && grandchild < 2) {
            count[pos]--;
            for (int i = 1; i < a[pos]; i++) {
                count[indices[i][b[pos]]]++;
                count[indices[a[pos] - i][b[pos]]]++;
                B = min(B, self(self, pos, breaks + 1, sum, used, grandchild + 1) + 1);
                count[indices[i][b[pos]]]--;
                count[indices[a[pos] - i][b[pos]]]--;
            }
            for (int i = 1; i < b[pos]; i++) {
                count[indices[a[pos]][i]]++;
                count[indices[a[pos]][b[pos] - i]]++;
                B = min(B, self(self, pos, breaks + 1, sum, used, grandchild + 1) + 1);
                count[indices[a[pos]][i]]--;
                count[indices[a[pos]][b[pos] - i]]--;
            }
            count[pos]++;
        }
        return memo[{count, {pos, breaks, sum, used, grandchild}}] = B;
    };
    dp(dp);
    cout << least;
}
