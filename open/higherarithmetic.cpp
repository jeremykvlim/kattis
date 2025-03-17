#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    while (n--) {
        int a;
        cin >> a;

        pq.emplace(a, a);
    }

    int ones = 0;
    while (!pq.empty() && pq.top().first == 1) {
        pq.pop();
        ones++;
    }

    while (ones--)
        if (pq.size() > 1 && pq.top().first < 3) {
            auto [a, b] = pq.top();
            pq.pop();

            pq.emplace(a + 1, b);
        } else pq.emplace(1, 1);

    if (pq.size() > 1 && pq.top().first == 1) {
        pq.pop();

        auto [a, b] = pq.top();
        pq.pop();

        pq.emplace(a + 1, b);
    }

    while (!pq.empty()) {
        auto [a, b] = pq.top();
        pq.pop();

        auto s = "(" + to_string(b);
        for (int j = 0; j < a - b; j++) s += "+1";
        s += ")";
        if (!pq.empty()) s += "*";
        cout << s;
    }
}
