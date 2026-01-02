#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> indices(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        int pi;
        cin >> pi;

        indices[pi] = i;
    }

    int size = ceil(sqrt(n)), blocks = (n + size - 1) / size;
    vector<int> b_l(blocks), b_r(blocks);
    for (int b = 0; b < blocks; b++) {
        b_l[b] = b * size + 1;
        b_r[b] = min(n, (b + 1) * size);
    }

    vector<int> a(n + 1, 0);
    for (int i = 1; i <= n; i++) a[i] = -i;

    vector<int> base(blocks, 0);
    vector<vector<int>> freq(blocks, vector<int>(2 * size + 1, 0));
    for (int b = 0; b < blocks; b++) {
        base[b] = a[b_l[b]];
        for (int i = b_l[b]; i <= b_r[b]; i++) freq[b][a[i] - base[b] + size]++;
    }

    auto scary = 0LL;
    vector<int> diff(blocks, 0);
    for (int pi = n; pi; pi--) {
        int i = indices[pi], b = (i - 1) / size, temp = a[i];
        if (i == b_l[b]) {
            base[b] += 2;
            for (int j = i; j <= b_r[b]; j++) {
                if (a[j] == temp) scary++;
                a[j] += 2;
            }
        } else
            for (int j = i; j <= b_r[b]; j++) {
                if (a[j] == temp) scary++;

                int k = a[j] - base[b];
                freq[b][k + size]--;
                a[j] += 2;
                freq[b][k + 2 + size]++;
            }

        for (int j = b + 1, sum = 0; j < blocks; j++) {
            sum += diff[j];
            int k = temp - sum - base[j] + size;
            if (0 <= k && k < 2 * size + 1) scary += freq[j][k];
        }

        if (b + 1 < blocks) diff[b + 1] += 2;
    }
    cout << scary;
}
