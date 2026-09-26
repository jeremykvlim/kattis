#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;
    int n = s.size();

    deque<int> left, right;
    for (int i = 1; i <= n; i++)
        if (s[i - 1] == '0') right.emplace_back(i);

    for (int i = 1; i <= n; i++) {
        if (s[i - 1] == '2') {
            while (!right.empty() && right.front() < i) {
                left.emplace_back(right.front());
                right.pop_front();
            }

            if (!right.empty() && right.front() == i) {
                right.pop_front();
                continue;
            }

            int l = 0, r = n + 1;
            if (!left.empty()) {
                l = left.back();
                left.pop_back();
            }

            if (!right.empty()) {
                r = right.front();
                right.pop_front();
            }

            int j = l + r - i;
            if (i >= j) left.emplace_back(j);
            else right.emplace_front(j);
        }
    }

    fill(s.begin(), s.end(), '1');
    for (int i : left) s[i - 1] = '0';
    for (int i : right) s[i - 1] = '0';
    cout << s;
}
