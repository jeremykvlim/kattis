#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    string s;
    cin >> n >> s;
    n -= 3;
    s += '.';

    vector<int> b(n + 1);
    auto count = [&](int l, int r) {
        for (int j = l; j <= r; j++) {
            b[j] = 0;
            for (int k = 0; k < 4; k++) b[j] += s[j + k] == 'b';
        }
    };
    count(0, n);

    int extras = 0, target = 4;
    map<int, set<int>, greater<>> indices;
    map<int, int> lengths;
    auto remove = [&](int i, int len) {
        indices[len].erase(i);
        if (indices[len].empty()) indices.erase(len);
        lengths.erase(i);
        extras -= len > 4;
    };

    auto add = [&](int l, int r) {
        int len = 0;
        queue<pair<int, int>> q;
        for (int i = l; i <= r; i++)
            if (i < r && b[i] == target) len++;
            else if (len) {
                int j = i - len;
                indices[len].emplace(j);
                lengths[j] = len;
                extras += len > 4;
                q.emplace(j, len);
                len = 0;
            }
        if (q.empty()) return;

        auto [i, len1] = q.front();
        q.pop();

        while (!q.empty()) {
            auto [j, len2] = q.front();
            q.pop();

            if (i + len1 >= j - 2) {
                remove(i, len1);
                remove(j, len2);
                len1 = j - i + len2;
                indices[len1].emplace(i);
                lengths[i] = len1;
                extras += len1 > 4;
            } else tie(i, len1) = tie(j, len2);
        }
    };

    auto update = [&](int i) {
        for (int j = i; j < i + 4; j++) s[j] = '.';
        count(max(0, i - 3), min(n, i + 3));

        auto [j, len] = *prev(lengths.upper_bound(i));
        remove(j, len);
        add(j, j + len);
    };

    for (; target; target--) {
        add(0, n);

        while (!lengths.empty()) {
            int i;
            cin >> i;

            update(i - 1);
            i = *indices.begin()->second.begin();
            if (extras == 1 && lengths.size() & 1) i += 3;

            cout << i + 1 << "\n" << flush;
            update(i);
        }
    }
}
