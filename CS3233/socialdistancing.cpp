#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> array(n);
    for (int &a : array) cin >> a;

    priority_queue<long long> pq;
    long long dist = 0;
    for (int i = 1; i < n; i++) {
        pq.emplace(array[i]);
        dist += array[i];
        if (!pq.empty() && pq.top() > array[i]) {
            dist -= pq.top() - array[i];
            pq.pop();
            pq.emplace(array[i]);
        }
    }

    cout << dist;
}
