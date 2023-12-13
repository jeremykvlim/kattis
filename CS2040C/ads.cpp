#include <bits/stdc++.h>
using namespace std;

void remove(int r, int c, vector<string> &page, vector<vector<bool>> &visited) {
    int h = r, w = c;
    while (h < page.size() - 1 && page[h + 1][c] == '+') h++;
    while (w < page[0].size() - 1 && page[r][w + 1] == '+') w++;

    for (int i = r + 1; i < h; i++)
        for (int j = c + 1; j < w; j++)
            if (page[i][j] == '+') remove(i, j, page, visited);

    for (int i = r + 1; i < h; i++)
        for (int j = c + 1; j < w; j++) {
            visited[i][j] = true;
            if (isalpha(page[i][j]) || strchr("+?!,. ", page[i][j])) continue;
            for (int k = r; k <= h; k++) {
                fill(page[k].begin() + c, page[k].begin() + w + 1, ' ');
                fill(visited[k].begin() + c, visited[k].begin() + w + 1, true);
            }
            break;
        }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int h, w;
    cin >> h >> w;
    cin.ignore();

    vector<string> page(h);
    for (auto &l: page) getline(cin, l);

    vector<vector<bool>> visited(h, vector<bool>(w, false));
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
            if (!visited[i][j] && page[i][j] == '+') remove(i, j, page, visited);

    for (auto &l: page) cout << l << "\n";
}
