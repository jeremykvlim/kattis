#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<pair<int, int>> activity(n);
    for (auto &[s, f] : activity) cin >> s >> f;
    sort(activity.begin(), activity.end());

    multiset<int> classrooms;
    int count = 0;
    for (auto [s, f] : activity)
        if (classrooms.size() < k || (*classrooms.begin()) < s) {
            if (classrooms.size() == k) classrooms.erase(classrooms.begin());
            classrooms.emplace(f);
            count++;
        } else if ((*classrooms.rbegin()) > f) {
            classrooms.erase(--classrooms.end());
            classrooms.emplace(f);
        }

    cout << count;
}
