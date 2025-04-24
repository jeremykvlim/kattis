#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> adj_masks(7, 0);
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++) {
            bool b;
            cin >> b;

            if (b) adj_masks[i] |= 1 << j;
        }

    vector<vector<pair<int, int>>> heist;
    for (int a = 0; a < 7; a++)
        for (int b = 0; b < 7; b++)
            for (int c = 0; c < 7; c++)
                for (int d = 0; d < 7; d++)
                    for (int e = 0; e < 7; e++)
                        for (int f = 0; f < 7; f++)
                            for (int g = 0; g < 7; g++) 
                                if ((adj_masks[a] >> 0) & 1 &&
                                    (adj_masks[b] >> 1) & 1 &&
                                    (adj_masks[c] >> 2) & 1 &&
                                    (adj_masks[d] >> 3) & 1 &&
                                    (adj_masks[e] >> 4) & 1 &&
                                    (adj_masks[f] >> 5) & 1 &&
                                    (adj_masks[g] >> 6) & 1) {
                                    vector<unordered_set<int>> seen(7);
                                    vector<vector<pair<int, int>>> temp(7);

                                    for (int i = 0; i < 7; i++)
                                        if (!seen[i].count(a)) {
                                            seen[i].emplace(a);
                                            temp[i].emplace_back(a, 0);
                                            break;
                                        }

                                    for (int i = 0; i < 7; i++)
                                        if (!seen[i].count(b)) {
                                            seen[i].emplace(b);
                                            temp[i].emplace_back(b, 1);
                                            break;
                                        }

                                    for (int i = 0; i < 7; i++)
                                        if (!seen[i].count(c)) {
                                            seen[i].emplace(c);
                                            temp[i].emplace_back(c, 2);
                                            break;
                                        }

                                    for (int i = 0; i < 7; i++)
                                        if (!seen[i].count(d)) {
                                            seen[i].emplace(d);
                                            temp[i].emplace_back(d, 3);
                                            break;
                                        }

                                    for (int i = 0; i < 7; i++)
                                        if (!seen[i].count(e)) {
                                            seen[i].emplace(e);
                                            temp[i].emplace_back(e, 4);
                                            break;
                                        }

                                    for (int i = 0; i < 7; i++)
                                        if (!seen[i].count(f)) {
                                            seen[i].emplace(f);
                                            temp[i].emplace_back(f, 5);
                                            break;
                                        }

                                    for (int i = 0; i < 7; i++)
                                        if (!seen[i].count(g)) {
                                            seen[i].emplace(g);
                                            temp[i].emplace_back(g, 6);
                                            break;
                                        }

                                    for (auto it = temp.begin(); it != temp.end();)
                                        if (it->empty()) it = temp.erase(it);
                                        else it++;

                                    if (heist.empty() || heist.size() > temp.size()) heist = temp;
                                }
    
    vector<string> names{"Twilight Sparkle", "Applejack", "Rarity", "Pinkie Pie", "Fluttershy", "Rainbow Dash", "Spike"};
    if (heist.empty()) cout << "IMPOSSIBLE\n";
    else {
        cout << heist.size() << "\n";
        for (auto op : heist) {
            cout << op.size() << "\n";
            for (auto [i, eij] : op) cout << names[i] << " " << eij + 1 << "\n";
        }
    }
}
