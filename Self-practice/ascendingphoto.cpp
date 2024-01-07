#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> heights;
    while (n--) {
        int h;
        cin >> h;
        
        if (heights.empty() || h != heights.back()) heights.emplace_back(h);
    }

    map<int, int> ids;
    for (auto h : heights) ids[h] = 0;
    int id = 0;
    for (auto i : ids) ids[i.first] = id++;
    for (auto &h : heights) h = ids[h];

    vector<vector<int> > groups(id);
    for (int i = 0; i < heights.size(); i++) groups[heights[i]].emplace_back(i);

    pair<int, int> best1 = {0, id}, second1 = best1;

    for (int i = 0; i < id; i++) {
        pair<int, int> best2 = best1, second2 = second1;

        for (auto index : groups[i]) {
            if (index == heights.size() - 1 || heights[index + 1] != i + 1) continue;

            pair<int, int> cut;
            if (groups[i].size() != 1 && best1.second + 1 == index) cut = {second1.first + 1, index};
            else cut = {best1.first + 1, index};

            if (cut > best2) {
                second2 = best2;
                best2 = cut;
            } else if (cut > second2) second2 = cut;
        }

        best1 = best2;
        second1 = second2;
    }

    cout << heights.size() - 1 - best1.first;
}
