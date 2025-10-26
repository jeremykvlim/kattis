#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    unordered_map<char, int> suits{{'S', 0}, {'H', 1}, {'D', 2}, {'C', 3}},
                             ranks{{'A', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9}, {'T', 10}, {'J', 11}, {'Q', 12}, {'K', 13}},
                             values{{'A', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9}, {'T', 10}, {'J', 10}, {'Q', 10}, {'K', 10}};

    vector<array<int, 4>> pile1(n), pile2(m);
    auto read = [&](vector<array<int, 4>> &pile) {
        for (auto &[suit, rank, points, king_spades] : pile) {
            string card;
            cin >> card;

            suit = suits[card[1]];
            rank = ranks[card[0]];
            points = values[card[0]];
            king_spades = card[0] == 'K' && card[1] == 'S';
        }
    };
    read(pile1);
    read(pile2);

    int base = 101;
    vector<__int128> p101(17, 1);
    for (int i = 1; i < 17; i++) p101[i] = p101[i - 1] * base;

    vector<__int128> pref1(n + 1, 0), pref2(m + 1, 0);
    vector<int> pref1_king_spades(n + 1, 0), pref2_king_spades(m + 1, 0), pref2_points(m + 1);
    for (int i = 0; i < n; i++) {
        pref1[i + 1] = pref1[i] + p101[pile1[i][1] - 1] + p101[13 + pile1[i][0]];
        pref1_king_spades[i + 1] = pref1_king_spades[i] + pile1[i][3];
    }
    for (int i = 0; i < m; i++) {
        pref2[i + 1] = pref2[i] + p101[pile2[i][1] - 1] + p101[13 + pile2[i][0]];
        pref2_king_spades[i + 1] = pref2_king_spades[i] + pile2[i][3];
        pref2_points[i + 1] = pref2_points[i] + pile2[i][2];
    }

    int score = 0;
    unordered_set<__int128> seen;
    for (int k = min(100, min(n, m)); k; k--) {
        if (score >= 20 * k) break;

        seen.clear();
        for (int i = 0; i + k <= n; i++)
            if (pref1_king_spades[i + k] == pref1_king_spades[i]) seen.emplace(pref1[i + k] - pref1[i]);
        if (seen.empty()) continue;

        int s = 0;
        for (int i = 0; i + k <= m; i++)
            if (pref2_king_spades[i + k] == pref2_king_spades[i])
                if (seen.count(pref2[i + k] - pref2[i])) {
                    s = max(s, pref2_points[i + k] - pref2_points[i]);
                    if (s == 10 * k) break;
                }

        if (s) {
            score = max(score, s * 2);
            if (score == 20 * k) break;
        }
    }
    cout << score;
}
