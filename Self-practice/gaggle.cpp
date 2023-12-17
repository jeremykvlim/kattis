#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> mentors(n);
    for (int &a : mentors) cin >> a;

    vector<bool> assigned(n, false);
    vector<int> end(n);
    iota(end.begin(), end.end(), 0);
    stack<int> free;
    for (int i = n - 1; i >= 0; --i) free.emplace(i);

    for (int i = 0; i < n; i++) {
        if (end[i] != mentors[i] - 1) free.emplace(mentors[i] - 1);
        int curr = -1;
        while (assigned[free.top()] || (i < n - 1 && end[i] == free.top())) {
            if (!assigned[free.top()]) curr = free.top();
            free.pop();
        }
        int j = free.top();
        free.pop();

        if (curr != -1) free.emplace(curr);
        swap(end[end[i]], end[end[j]]);
        mentors[i] = j + 1;
        assigned[j] = true;
    }

    for (int &a : mentors) cout << a << " ";
}
