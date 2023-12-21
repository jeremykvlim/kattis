#include "bits/stdc++.h"
using namespace std;

long long swap_digits(long long curr, int pos, int remaining, bool repeat, vector<int> &digits, vector<vector<unordered_set<long long>>> &visited) {
    if (pos < 0) {
        if (repeat || !(remaining & 1)) return curr;
        else return curr - digits[0] - 10 * digits[1] + 10 * digits[0] + digits[1];
    }

    if (!remaining) return curr;
    if (visited[pos][remaining - 1].count(curr)) return -1;
    visited[pos][remaining - 1].insert(curr);
    int p = pos;
    vector<int> swap_indices;
    for (int i = pos - 1; i >= 0; i--)
        if (digits[p] < digits[i]) {
            p = i;
            swap_indices = {p};
        } else if (digits[p] == digits[i]) swap_indices.emplace_back(i);

    long long largest, swapped;
    if (p == pos) largest = swap_digits(curr, pos - 1, remaining, repeat, digits, visited);
    for (int i : swap_indices) {
        swapped = curr - llround(pow(10, pos)) * digits[pos] - llround(pow(10, i)) * digits[i] + llround(pow(10, pos)) * digits[i] + llround(pow(10, i)) * digits[pos];
        swap(digits[pos], digits[i]);
        largest = max(largest, swap_digits(swapped, pos - 1, remaining - 1, repeat, digits, visited));
        swap(digits[pos], digits[i]);
    }

    return largest;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    int k;
    cin >> n >> k;

    int x = (int) log10((double) n) + 1;
    vector<int> digits(x);
    vector<vector<unordered_set<long long>>> visited(x, vector<unordered_set<long long>>(k));

    int position = 0, mask = 0;
    bool repeat = false;
    auto num = n;
    while (num) {
        digits[position++] = num % 10;
        if (mask & (1 << (num % 10))) repeat = true;
        mask |= (1 << (num % 10));
        num /= 10;
    }

    cout << swap_digits(n, position - 1, k, repeat, digits, visited);
}
