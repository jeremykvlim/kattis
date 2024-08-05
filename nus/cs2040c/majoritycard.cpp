#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    deque<pair<int, int>> deck;
    unordered_map<int, int> freq;
    auto cmp = [](auto p1, auto p2) {return p1.first != p2.first ? p1.first > p2.first : p1.second < p2.second;};
    set<pair<int, int>, decltype(cmp)> cards(cmp);

    auto put = [&](bool top) {
        int x, y;
        cin >> x >> y;

        cards.erase({freq[y], y});
        freq[y] += x;
        cards.emplace(freq[y], y);
        top ? deck.emplace_front(x, y) : deck.emplace_back(x, y);
    };

    auto remove = [&](bool top) {
        int z;
        cin >> z;

        for (;;) {
            auto &[x, y] = top ? deck.front() : deck.back();
            if (z >= x) {
                cards.erase({freq[y], y});
                freq[y] -= x;
                cards.emplace(freq[y], y);
                z -= x;
                top ? deck.pop_front() : deck.pop_back();
            } else {
                cards.erase({freq[y], y});
                freq[y] -= z;
                cards.emplace(freq[y], y);
                x -= z;
                break;
            }
        }
    };

    while (n--) {
        string command;
        cin >> command;

        if (command == "PUT_TOP") put(true);
        else if (command == "PUT_BOTTOM") put(false);
        else if (command == "REMOVE_TOP") remove(true);
        else remove(false);

        cout << (cards.begin())->second << "\n";
    }
}
