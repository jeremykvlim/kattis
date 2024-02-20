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
    vector<int> end(n), free(n);
    iota(end.begin(), end.end(), 0);
    iota(free.rbegin(), free.rend(), 0);

    for (int i = 0; i < n; i++) {
        if (end[i] != mentors[i] - 1) free.emplace_back(mentors[i] - 1);
        int curr = -1;
        while (assigned[free.back()] || (i < n - 1 && end[i] == free.back())) {
            if (!assigned[free.back()]) curr = free.back();
            free.pop_back();
        }
        int j = free.back();
        free.pop_back();
        mentors[i] = j + 1;
        assigned[j] = true;

        int temp1 = end[i], temp2 = end[j];
        end[temp1] = temp2;
        end[temp2] = temp1;

        if (curr != -1) free.emplace_back(curr);
    }

    for (int &b : mentors) cout << b << " ";
}
