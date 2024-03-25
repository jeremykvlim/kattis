#include <bits/stdc++.h>
using namespace std;

bool valid(int p1, int p2, vector<bool> visited, vector<vector<int>> between) {
    if (p1 > p2) swap(p1, p2);
    return !between[p1][p2] || visited[between[p1][p2]];
}

void unlock(int curr, int prev, vector<bool> visited, vector<vector<int>> &between, vector<pair<int, int>> &pos, vector<string> &patterns, string p = "") {
    if (all_of(visited.begin(), visited.end(), [&](auto b) {return b;})) {
        patterns.emplace_back(p);
        return;
    }

    pair<int, int> prev_dir{pos[curr].first - pos[prev].first, pos[curr].second - pos[prev].second}, next_dir;
    for (int next = 1; next <= 9; next++)
        if (!visited[next] && valid(curr, next, visited, between)) {
            next_dir = {pos[next].first - pos[curr].first, pos[next].second - pos[curr].second};
            int cross = prev_dir.first * next_dir.second - prev_dir.second * next_dir.first;
            p += (cross < 0) ? 'R' : (cross > 0) ? 'L' : (prev_dir == next_dir) ? 'S' : 'A';

            visited[next] = true;
            unlock(next, curr, visited, between, pos, patterns, p);
            visited[next] = false;
            p.pop_back();
        }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    vector<vector<int>> between(10, vector<int>(10, 0));
    between[1][3] = 2;
    between[1][7] = 4;
    between[1][9] = between[2][8] = between[3][7] = between[4][6] = 5;
    between[3][9] = 6;
    between[7][9] = 8;

    vector<pair<int, int>> pos(10);
    for (int i = 1; i <= 9; i++) pos[i] = {1 + (i - 1) / 3, 1 + (i - 1) % 3};

    vector<string> patterns;
    for (int i = 1; i <= 9; i++)
        for (int j = 1; j <= 9; j++) {
            vector<bool> visited(10, false);
            visited[0] = true;
            if (i != j && valid(i, j, visited, between)) {
                visited[i] = visited[j] = true;
                unlock(i, j, visited, between, pos, patterns);
            }
        }

    cout << count_if(patterns.begin(), patterns.end(), [&](auto p) {return equal(s.begin(), s.end(), p.begin(), [](char c1, char s1) {return c1 == '?' || c1 == s1;});});
}
