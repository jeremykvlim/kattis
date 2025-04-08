#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    vector<vector<int>> count(9, vector<int>(9, 0));
    for (int i = 0; i < s.length() - 1; i++) count[s[i] - '1'][s[i + 1] - '1']++;

    vector<int> row(9);
    iota(row.begin(), row.end(), 0);

    int time = INT_MAX;
    do {
        int curr = row[s[0] - '1'];
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++) curr += abs(row[i] - row[j]) * count[i][j];
        time = min(time, curr);
    } while (next_permutation(row.begin(), row.end()));

    time += s.size();
    cout << time;
}
