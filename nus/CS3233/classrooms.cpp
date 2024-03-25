#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, k;
    multiset<int> classrooms;
    cin >> n >> k;

    vector<pair<int, int>> activity(n);
    for (auto &a : activity) cin >> a.first >> a.second;
    sort(activity.begin(), activity.end());

    int most = 0;
    for (auto &a : activity)
        if (classrooms.size() < k || (*classrooms.begin()) < a.first) {
            if (classrooms.size() == k) classrooms.erase(classrooms.begin());
            classrooms.insert(a.second);
            most++;
        } else if ((*classrooms.rbegin()) > a.second) {
            classrooms.erase(--classrooms.end());
            classrooms.insert(a.second);
        }

    cout << most;
}
