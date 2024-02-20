#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n);
    for (auto &ai : a) {
        cin >> ai;
        ai--;
    }

    vector<long long> b(n);
    priority_queue<pair<long long, int>> antivirus;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> virus;
    for (int i = 0; i < n; i++) {
        cin >> b[i];

        if (b[i] > 0) antivirus.emplace(b[i], -i);
        else if (b[i] < 0) virus.emplace(b[i], -i);
    }

    vector<int> count(n, 1);
    auto day = 0LL;
    while (!virus.empty()) {
        if (virus.top().first != b[-virus.top().second]) {
            virus.pop();
            continue;
        }

        if (antivirus.empty()) {
            cout << "never";
            exit(0);
        }

        auto [bi, i] = antivirus.top();
        antivirus.pop();
        if (!b[-i]) continue;

        int ai = a[-i];
        if (!b[ai] && !ai && a[ai] == -i) {
            cout << "never";
            exit(0);
        }

        if (!b[ai] && ai && ai != a[0]) {
            a[-i] = a[ai];
            count[-i] += count[ai];
            ai = a[-i];
        }

        day += count[-i];
        b[ai] += b[-i];
        b[-i] = 0;

        if (b[ai] > 0) antivirus.emplace(b[ai], -ai);
        else if (b[ai] < 0) virus.emplace(b[ai], -ai);
    }

    cout << day;
}
