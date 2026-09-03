#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<string> names{"1", "2", "3", "4", "5",
                         "blue", "green", "red", "white", "yellow",
                         "anna", "bernhard", "chris", "david", "ellen",
                         "danish", "finnish", "icelandic", "norwegian", "swedish",
                         "amiga", "atari", "linux", "mac", "windows",
                         "c", "c++", "java", "pascal", "perl"};

    unordered_map<string, int> indices;
    for (int i = 0; i < names.size(); i++) indices[names[i]] = i;

    int N;
    cin >> N;

    for (int n = 1; n <= N; n++) {
        int k;
        cin >> k;

        vector<array<int, 3>> facts(k);
        vector<int> degree(names.size(), 0);
        for (auto &[f, s, t] : facts) {
            string first, second, third;
            cin >> first >> second >> third;

            f = indices[first];
            s = second == "same-as" ? 0 : second == "left-of" ? -1 : second == "right-of" ? 1 : 2;
            t = indices[third];
            degree[f]++;
            degree[t]++;
        }

        auto dfs = [&]() {
            vector<int> room(names.size(), -1);
            for (int i = 0; i < 5; i++) room[i] = i;

            auto check = [&](int r1, int relationship, int r2) {
                return !~r1 || !~r2 || (relationship == 2 ? abs(r1 - r2) == 1 : r1 - r2 == relationship);
            };
            for (auto [f, s, t] : facts)
                if (!check(room[f], s, room[t])) return false;

            vector<int> used(6, 0);
            auto backtrack = [&](auto &&self) -> bool {
                int j = -1, count = 6, mask = 0;
                for (int i = 5; i < names.size(); i++)
                    if (room[i] < 0) {
                        int m = 0;
                        for (int r = 0; r < 5; r++)
                            if (!((used[i / 5] >> r) & 1)) {
                                for (auto [f, s, t] : facts)
                                    if (!check(f == i ? r : room[f], s, t == i ? r : room[t])) goto next;
                                m |= 1 << r;
                                next:;
                            }

                        int pc = popcount((unsigned) m);
                        if (!pc) return false;
                        if (!~j || count > pc || count == pc && degree[j] < degree[i]) {
                            j = i;
                            count = pc;
                            mask = m;
                        }
                    }
                if (!~j) return true;

                for (int r = 0; r < 5; r++)
                    if ((mask >> r) & 1) {
                        room[j] = r;
                        used[j / 5] |= 1 << r;
                        if (self(self)) return true;
                        used[j / 5] ^= 1 << r;
                        room[j] = -1;
                    }
                return false;
            };
            return backtrack(backtrack);
        };

        int count = 0, owner = -1;
        for (int o = 10; o < 15; o++) {
            facts.push_back({o, 0, indices["amiga"]});
            degree[o]++;
            degree[indices["amiga"]]++;
            if (dfs()) {
                count++;
                owner = o;
            }
            degree[o]--;
            degree[indices["amiga"]]--;
            facts.pop_back();
        }

        cout << "scenario #" << n << ": ";
        if (!count) cout << "impossible.\n";
        else if (count > 1) cout << "cannot identify the amiga owner.\n";
        else cout << names[owner] << " owns the amiga.\n";
    }
}
