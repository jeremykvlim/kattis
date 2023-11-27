#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long k;
    cin >> n >> k;

    unordered_set<string> leave;
    priority_queue<pair<long long, string>, vector<pair<long long, string>>, greater<pair<long long, string>>> pq;
    for (int i = 0; i < n; i++) {
        int q, t;
        cin >> q >> t;

        switch (q) {
            case 1: {
                string m;
                int s;
                cin >> m >> s;
                long long v = -s + t * k;
                pq.push({v, m});
                break;
            }

            case 2: {
                if (pq.empty()) cout << "doctor takes a break\n";
                while (!pq.empty()) {
                    auto p = pq.top();
                    pq.pop();
                    if (leave.find(p.second) == leave.end()) {
                        cout << p.second << "\n";
                        break;
                    }
                    leave.erase(p.second);
                }
                break;
            }

            case 3: {
                string m;
                cin >> m;
                leave.insert(m);
                break;
            }
        }
    }
}
