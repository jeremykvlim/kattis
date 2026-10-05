#pragma GCC optimize("Ofast,unroll-loops")
#pragma GCC target("avx2")
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> speeds(2 * n);
    for (int i = 0; i < 2 * n; i++) {
        int s;
        cin >> s;

        speeds[i] = {s, i};
    }
    sort(speeds.begin(), speeds.end());

    const int size = 128;
    int blocks = (n + size - 1) / size;

    auto full_adder = [](auto &a, auto &b, const auto &c) {
        auto temp = a ^ b;
        b = (a & b) | (c & temp);
        a = temp ^ c;
    };

    int lg = __lg(n) + 1;
    vector<int> count(lg + 1, 0);
    vector<vector<bitset<size>>> sum(lg + 1, vector<bitset<size>>(blocks)), carry(lg + 1, vector<bitset<size>>(blocks)), temps(size, vector<bitset<size>>((2 * n + size - 1) / size, 0));
    for (auto [_, i] : speeds)
        if (i >= n)
            for (int r = 0; r < size; r++, i = (i - 1) % n + n) temps[r][i / size][i % size] = temps[r][(i - n) / size][(i - n) % size] = true;
        else {
            auto temp = temps[i % size].begin() + i / size;
            for (int l = 0; l <= lg; l++) {
                if (count[l] < 2) {
                    copy_n(temp, blocks, count[l]++ ? carry[l].begin() : sum[l].begin());
                    break;
                }

                for (int b = 0; b < blocks; b++) full_adder(sum[l][b], carry[l][b], temp[b]);
                count[l] = 1;
                temp = carry[l].begin();
            }
        }

    vector<int> shifts;
    for (int i = 0; i < n; i++) {
        int wins = 0;
        for (int l = 0, b = i / size, r = i % size; l <= lg; l++) {
            if (count[l]) wins += sum[l][b][r] << l;
            if (count[l] == 2) wins += carry[l][b][r] << l;
        }
        if (wins * 2 > n) shifts.emplace_back(i);
    }

    cout << shifts.size() << "\n";
    for (int s : shifts) cout << s << " ";
}