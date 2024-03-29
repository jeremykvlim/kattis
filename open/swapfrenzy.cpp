#include "bits/stdc++.h"
using namespace std;

long long largest(long long curr, int pos, int k, bool repeat, vector<int> &digits, vector<vector<unordered_set<long long>>> &visited) {
    if (pos < 0) {
        if (!repeat && (k & 1)) curr -= digits[0] + 10 * digits[1] - 10 * digits[0] - digits[1];
        return curr;
    }

    if (!k) return curr;
    if (visited[pos][k - 1].count(curr)) return -1;

    visited[pos][k - 1].emplace(curr);
    int p = pos;
    vector<int> indices;
    for (int i = pos - 1; ~i; i--)
        if (digits[p] < digits[i]) {
            p = i;
            indices = {p};
        } else if (digits[p] == digits[i]) indices.emplace_back(i);

    auto l = (p == pos) ? largest(curr, pos - 1, k, repeat, digits, visited) : -1LL;
    for (int i : indices) {
        auto next = curr - llround(pow(10, pos)) * digits[pos] - llround(pow(10, i)) * digits[i] + llround(pow(10, pos)) * digits[i] + llround(pow(10, i)) * digits[pos];
        swap(digits[pos], digits[i]);

        l = max(l, largest(next, pos - 1, k - 1, repeat, digits, visited));
        swap(digits[pos], digits[i]);
    }

    return l;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    int k;
    cin >> n >> k;

    int rank = log10(n) + 1;
    vector<int> digits(rank);
    vector<vector<unordered_set<long long>>> visited(rank, vector<unordered_set<long long>>(k));
    vector<bool> seen(10, false);
    int i = 0;
    bool repeat = false;
    for (auto temp = n; temp; temp /= 10) {
        digits[i++] = temp % 10;
        repeat |= seen[temp % 10];
        seen[temp % 10] = true;
    }

    cout << largest(n, rank - 1, k, repeat, digits, visited);
}
