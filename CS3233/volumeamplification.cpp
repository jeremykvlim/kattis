#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, y;
        cin >> n >> y;

        vector<int> description(n);
        for (int &a : description) cin >> a;

        vector<int> volumes = {1};
        for (int i = 0; i < n; i++) {
            vector<int> amplified;
            for (auto v : volumes)
                if (v * description[i] <= y) amplified.emplace_back(v * description[i]);
            volumes.insert(volumes.end(), amplified.begin(), amplified.end());
            sort(volumes.begin(), volumes.end());
        }

        cout << volumes.back() << "\n";
    }
}
