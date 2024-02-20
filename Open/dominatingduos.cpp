#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    stack<int> heights;
    int count = 0;
    while (n--) {
        int d;
        cin >> d;

        auto prev = heights.size();
        while (!heights.empty() && heights.top() < d) heights.pop();
        count += min(prev, 1 + prev - heights.size());
        heights.emplace(d);
    }

    cout << count;
}
