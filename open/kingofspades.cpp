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

    vector<array<int, 3>> pile1(n), pile2(m);
    auto read = [&](vector<array<int, 3>> &pile) {
        for (auto &[suit, rank, points] : pile) {
            string card;
            cin >> card;

            suit = suits[card[1]];
            rank = ranks[card[0]];
            points = values[card[0]];
        }
    };
    read(pile1);
    read(pile2);

    auto process = [&](const auto &pile) {
        int s = pile.size();

        vector<pair<unsigned __int128, int>> masks;
        for (int i = 0; i < s; i++) {
            vector<int> freq(18, 0);
            int score = 0;
            for (int j = i; j - i < 100 && j < s; j++) {
                auto [suit, rank, points] = pile[j];
                if (!suit && rank == 13) break;

                freq[14 + suit]++;
                freq[rank]++;
                score += points;

                unsigned __int128 mask = 0;
                for (int k = 1; k <= 17; k++) mask = (mask << 7) | freq[k];
                masks.emplace_back(mask, score);
            }
        }
        sort(masks.begin(), masks.end());
        masks.erase(unique(masks.begin(), masks.end(), [](auto p1, auto p2) {return p1.first == p1.second;}), masks.end());
        return masks;
    };
    auto l = process(pile1), r = process(pile2);

    int score = 0;
    for (int i = 0, j = 0; i < l.size() && j < r.size();)
        if (l[i].first == r[j].first) {
            score = max(score, 2 * l[i].second);
            i++;
            j++;
        } else l[i].first < r[j].first ? i++ : j++;
    cout << score;
}
