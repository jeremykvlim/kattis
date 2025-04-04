#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int L;
    while (cin >> L && L) {
        string s, t;
        cin >> s >> t;

        auto ctoi = [](char c) {return c == 'S';};
        array<vector<int>, 2> pref_s{vector<int>(L + 1, 0), vector<int>(L + 1, 0)}, pref_t{vector<int>(L + 1, 0), vector<int>(L + 1, 0)}, indices;
        for (int i = 0; i < L; i++) {
            for (int b = 0; b < 2; b++) {
                pref_s[b][i + 1] += pref_s[b][i];
                pref_t[b][i + 1] += pref_t[b][i];
            }
            pref_s[ctoi(s[i])][i + 1]++;
            pref_t[ctoi(t[i])][i + 1]++;
            indices[ctoi(t[i])].emplace_back(i);
        }

        vector<int> prev(L, 0), curr(L, 0);
        for (int i = 0; i < L; i++) {
            fill(curr.begin(), curr.end(), L + 1);
            for (int l = 0; l < L; l++)
                for (int j = 0; j < 2; j++) {
                    int k = prev[(l + j) % L] + i + 1;
                    if (k >= L) {
                        curr[l] = min(curr[l], prev[(l + j) % L]);
                        continue;
                    }

                    array<int, 2> count;
                    int r = (l + i) % L;
                    for (int b = 0; b < 2; b++) {
                        count[b] = pref_s[b][r + 1] - pref_s[b][l];
                        if (l > r) count[b] += pref_s[b][L];
                    }

                    if (j) r = l;
                    int b = ctoi(s[r]);
                    curr[l] = min(curr[l], max(prev[(l + j) % L], count[b] <= pref_t[b][k] ? 0 : indices[b][count[b] - 1] - i));
                }
            prev = curr;
        }
        cout << *min_element(prev.begin(), prev.end()) << "\n";
    }
}
