#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    map<int, int> optimal, usual;
    optimal[0] = usual[0] = k;
    int possible = 0, run = 0;
    while (n--) {
        int t, l;
        cin >> t >> l;
        
        if (optimal.begin()->first <= t) {
            if (!(--optimal.begin()->second)) optimal.erase(optimal.begin());
            optimal[t + l]++;
            possible++;
        }
        else {
            auto last = prev(optimal.end());
            if (last->first > t + l) {
                if (!(--last->second)) optimal.erase(last);
                optimal[t + l]++;
            }
        }

        if (usual.begin()->first <= t) {
            if (!(--usual.begin()->second)) usual.erase(usual.begin());
            usual[t + l]++;
            run++;
        }

        cout << possible - run << "\n";
    }
}
