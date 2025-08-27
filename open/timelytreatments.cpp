#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    
    vector<pair<int, int>> patients(n);
    for (auto &[d, t] : patients) cin >> t >> d;
    sort(patients.begin(), patients.end());
    
    int total = 0;
    priority_queue<int> pq;
    for (auto [d, t] : patients) {
        total += t;
        pq.emplace(t);
        
        if (total > d) {
            total -= pq.top();
            pq.pop();
        }
    }
    cout << pq.size();
}
