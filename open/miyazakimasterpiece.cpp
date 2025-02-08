#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, l;
    string s;
    cin >> n >> m >> l >> s;

    if (l < 3) {
        cout << m;
        exit(0);
    }

    array<int, 3> blocks{l / 3, l / 3, l - l / 3 - l / 3}, start{0, l / 3, l / 3 + l / 3};
    vector<unordered_map<string, vector<int>>> indices(3);
    for (int i = 0; i <= n - l; i++)
        for (int b = 0; b < 3; b++) indices[b][s.substr(i + start[b], blocks[b])].emplace_back(i);

    int count = 0;
    while (m--) {
        string f;
        cin >> f;

        for (int b = 0; b < 3; b++) {
            auto ss = f.substr(start[b], blocks[b]);
            if (!indices[b].count(ss)) continue;

            for (int i : indices[b][ss]) {
                auto check = [&]() {
                    int mismatches = 0;
                    for (int j = 0; j < f.size(); j++)
                        if (s[i + j] != f[j]) {
                            mismatches++;
                            if (mismatches > 2) return false;
                        }

                    return true;
                };

                if (check()) {
                    count++;
                    goto next;
                }
            }
        }

        next:;
    }
    cout << count;
}
