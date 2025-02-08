#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int biggest = 0;
    vector<vector<vector<int>>> cards(n + 1, vector<vector<int>>(6, vector<int>(6)));
    for (int i = 1; i <= n; i++)
        for (int r = 1; r <= 5; r++)
            for (int c = 1; c <= 5; c++) {
                cin >> cards[i][r][c];

                biggest = max(biggest, cards[i][r][c]);
            }

    int t = 0;
    vector<int> visited(biggest + 1, 0);
    for (int a = 1; a <= n; a++)
        for (int b = a + 1; b <= n; b++)
            for (int r1 = 1; r1 <= 5; r1++)
                for (int r2 = 1; r2 <= 5; r2++) {
                    t++;
                    for (int c = 1; c <= 5; c++) visited[cards[a][r1][c]] = t;

                    vector<int> shared;
                    for (int c = 1; c <= 5; c++) {
                        if (visited[cards[b][r2][c]] == t) shared.emplace_back(cards[b][r2][c]);
                        visited[cards[b][r2][c]] = t;
                    }

                    for (int card : shared) {
                        auto tie = [&]() {
                            for (int i = 1; i <= n; i++)
                                for (int r = 1; r <= 5; r++)
                                    for (int c = 1; c <= 5; c++) {
                                        if (cards[i][r][c] == card || visited[cards[i][r][c]] != t) break;
                                        if (c == 5) return false;
                                    }
                            return true;
                        };

                        if (tie()) {
                            cout << a << " " << b;
                            exit(0);
                        }
                    }
                }
    cout << "no ties";
}
