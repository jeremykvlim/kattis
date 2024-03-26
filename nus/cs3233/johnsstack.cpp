#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n;
        cin >> n;

        vector<int> sizes(n);
        for (int &s : sizes) cin >> s;

        long long steps = 0;
        for (int i = 1; i < n; i++) {
            sort(sizes.begin(), sizes.begin() + i);
            if (sizes[i - 1] <= sizes[i]) continue;

            vector<int> smaller;
            for (int j = 0; j < i && sizes[j] < sizes[i]; j++) {
                if (j && sizes[j] == sizes[j - 1]) continue;
                smaller.emplace_back(count(sizes.begin() + j, sizes.begin() + i, sizes[j]));
            }

            long long sum = 0;
            for (int s : smaller) sum += (sum + 1) * s;
            ++steps += sum;
        }
        
        cout << steps << "\n";
    }
}
