#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> points = {100, 75, 60, 50, 45, 40, 36, 32, 29, 26, 24, 22, 20, 18, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    map<int, map<int, map<int, vector<int>>>> solved;
    vector<int> scores(n, 0);
    for (int i = 0; i < n; i++) {
        int s, p, f, o;
        cin >> s >> p >> f >> o;

        scores[i] += o;
        solved[-s][p][f].emplace_back(i);
    }

    int rank = 0;
    for (auto &[s, penalty] : solved)
        for (auto &[p, time] : penalty)
            for (auto &[f, contestants] : time) {
                int total = contestants.size() - 1;
                for (int i = 0; i < contestants.size() && rank < 30; i++, rank++) total += points[rank];

                total /= contestants.size();
                for (int i : contestants) scores[i] += total;
            }

    for (int s : scores) cout << s << '\n';
}
