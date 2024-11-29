#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    
    while (t--) {
        int a, b;
        cin >> a >> b;
        
        int count = 0;
        vector<int> seen(1e6 + 1, -1);
        for (int i = a; i <= b; i++) {
            int p10 = 10;
            while (p10 <= i) p10 *= 10;
            p10 /= 10;
            
            int pow = log10(p10) + 1, j = i;
            while (pow--) {
                j = (j % 10) * p10 + (j / 10);
                if (i < j && j <= b && seen[j] != i) {
                    seen[j] = i;
                    count++;
                }
            }
        }
        cout << count << "\n";
    }
}
