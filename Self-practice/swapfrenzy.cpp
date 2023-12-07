#include "bits/stdc++.h"
using namespace std;

long long swap_digits(long long curr, int position, int remaining, bool repeat, vector<int> &digits, vector<vector<unordered_set<long long>>> &visited) {
    if (position < 0) {
        if (repeat || !(remaining & 1)) return curr;
        else return curr - digits[0] - 10 * digits[1] + 10 * digits[0] + digits[1];
    }

    if (!remaining) return curr;
    if (visited[position][remaining - 1].count(curr)) return -1;
    visited[position][remaining - 1].insert(curr);
    int p = position;
    vector<int> swap_indices;
    for (int i = position - 1; i >= 0; i--) {
        if (digits[p] < digits[i]) {
            p = i;
            swap_indices = {p};
        } else if (digits[p] == digits[i]) swap_indices.push_back(i);
    }

    long long largest, swapped;
    if (p == position) largest = swap_digits(curr, position - 1, remaining, repeat, digits, visited);
    for (int i : swap_indices) {
        swapped = curr - llround(pow(10, position)) * digits[position] - llround(pow(10, i)) * digits[i] + llround(pow(10, position)) * digits[i] + llround(pow(10, i)) * digits[position];
        swap(digits[position], digits[i]);
        largest = max(largest, swap_digits(swapped, position - 1, remaining - 1, repeat, digits, visited));
        swap(digits[position], digits[i]);
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
    auto numerator = n;
    while (numerator) {
        digits[position++] = numerator % 10;
        if (mask & (1 << (numerator % 10))) repeat = true;
        mask |= (1 << (numerator % 10));
        dividend /= 10;
    }

    cout << swap_digits(n, position - 1, k, repeat, digits, visited);
}
