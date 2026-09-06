#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> min_size{0, 1}, max_size{0, 1};
    while (min_size.back() <= n) {
        min_size.emplace_back(min_size.back() + min_size[min_size.size() - 2] + 1);
        max_size.emplace_back(min(n, 2 * max_size.back() + 1));
    }

    auto dfs = [&](auto &&self, int smallest, int size, int min_height, int max_height) -> int {
        if (!size) return 0;

        int left = size + 1, hll = -1, hlr = -1;
        for (int hl = max(0, min_height - 2); hl < max_height; hl++)
            for (int hr = max(0, hl - 1); hr <= min(max_height - 1, hl + 1); hr++) {
                if (max(hl, hr) + 1 < min_height) continue;

                int l = max(min_size[hl], size - 1 - max_size[hr]);
                if (l > max_size[hl] || size - 1 - l < min_size[hr]) continue;

                if (left > l) {
                    left = l;
                    hll = hlr = hl;
                } else if (left == l) hlr = hl;
            }
        cout << smallest + left << " ";
        
        int hl = self(self, smallest, left, hll, hlr), right = size - 1 - left, hrl = -1, hrr = -1;
        for (int hr = max(0, hl - 1); hr <= min(max_height - 1, hl + 1); hr++) {
            if (max(hl, hr) + 1 < min_height || (!(min_size[hr] <= right && right <= max_size[hr]))) continue;

            if (!~hrl) hrl = hr;
            hrr = hr;
        }

        int hr = self(self, smallest + left + 1, right, hrl, hrr);
        return max(hl, hr) + 1;
    };
    dfs(dfs, 1, n, 1, min_size.size() - 2);
}
