#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long k;
    cin >> n >> k;

    unordered_set<string> leave;
    priority_queue<pair<long long, string>, vector<pair<long long, string>>, greater<>> pq;
    while (n--) {
        int q, t;
        cin >> q >> t;

        switch (q) {
            case 1: {
                string m;
                int s;
                cin >> m >> s;
                
                pq.emplace(-s + k * t, m);
                break;
            }

            case 2: {
                if (pq.empty()) cout << "doctor takes a break\n";
                
                while (!pq.empty()) {
                    auto [v, m] = pq.top();
                    pq.pop();
                    
                    if (!leave.count(m)) {
                        cout << m << "\n";
                        break;
                    }
                    
                    leave.erase(m);
                }
                break;
            }

            case 3: {
                string m;
                cin >> m;
                
                leave.emplace(m);
                break;
            }
        }
    }
}
