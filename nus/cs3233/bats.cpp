#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, s;
    string steps;
    cin >> m >> s >> steps;

    if (m < s) {
        cout << steps;
        exit(0);
    }

    int x = 0, bound = 0, capacity = 0;
    vector<int> blocks;
    for (int i = m - 1; ~i; i--)
        if (steps[i] == '+') {
            bound++;
            capacity++;
        } else {
            if (i == m - 1 || steps[i + 1] == '+') blocks.emplace_back(capacity);
            bound /= 2;
            if (capacity) capacity = (capacity - 1) / 2;
            x++;
        }

    if (x + bit_width((unsigned) bound + 1) < s) {
        cout << steps;
        exit(0);
    }

    bool first = true;
    int block_plus, block_x;
    auto process_block = [&](int l = 0) {
        block_plus = first;
        block_x = 0;
        int i = l;
        for (; i < m; i++) {
            char c = steps[i];
            if (c == '+') block_plus++;
            else if (!block_plus) block_x++;
            else break;
        }
        first = false;
        return i - l;
    };
    int r = process_block();

    int b = bit_width((unsigned) capacity + 2) - 1, mask = (1 << b) - 1;
    if (block_plus > mask) {
        block_plus = mask;
        for (int i = 0; i <= r - block_plus; i++) steps[i] = 'o';
    }

    int remaining = mask - block_plus, replace_x = max(0, x - s + b);
    reverse(blocks.begin(), blocks.end());
    for (int c : blocks) {
        int len = process_block(r), replace = min(min(block_x, replace_x), max(0, block_x + (int) bit_width((unsigned) remaining) - (int) bit_width((unsigned) c)));
        block_x -= replace;
        replace_x -= replace;

        if (block_x >= 20 || remaining > c) {
            replace = min(replace_x, block_x);
            block_x -= replace;
            replace_x -= replace;
        } else {
            auto inflate = [&]() -> int {
                if (!block_x) return remaining;
                if (block_x >= 20) return INT_MAX;
                return min((((long long) remaining + 1) << block_x) - 1, (long long) INT_MAX);
            };

            int temp = inflate();
            if (replace_x && temp > c) {
                block_x--;
                replace_x--;
                temp = inflate();
            }
            remaining = temp;
            if (block_plus > remaining) block_plus = remaining;
            remaining -= block_plus;
        }

        for (int i = min(block_x, m - r); i < len - min(block_plus, len); i++) steps[i + r] = 'o';
        r += len;
    }
    cout << steps;
}
