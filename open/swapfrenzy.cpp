#include "bits/stdc++.h"
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    int k;
    cin >> n >> k;

    vector<int> digits;
    vector<bool> seen(10, false);
    bool repeat = false;
    for (auto temp = n; temp; temp /= 10) {
        digits.emplace_back(temp % 10);
        repeat |= seen[temp % 10];
        seen[temp % 10] = true;
    }
    vector<vector<unordered_set<long long>>> visited(digits.size(), vector<unordered_set<long long>>(k));

    auto largest = [&](auto &&self, long long curr, int pos, int k) -> long long {
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

        auto l = p == pos ? self(self, curr, pos - 1, k) : -1LL;
        for (int i : indices) {
            auto next = curr - llround(pow(10, pos)) * digits[pos] - llround(pow(10, i)) * digits[i] + llround(pow(10, pos)) * digits[i] + llround(pow(10, i)) * digits[pos];
            swap(digits[pos], digits[i]);

            l = max(l, self(self, next, pos - 1, k - 1));
            swap(digits[pos], digits[i]);
        }

        return l;
    };

    cout << largest(largest, n, digits.size() - 1, k);
}
