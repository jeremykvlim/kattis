#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n) {
        vector<int> numbers(n);
        for (int &v : numbers) cin >> v;
        sort(numbers.rbegin(), numbers.rend());

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                if (i != j && gcd(numbers[i], numbers[j]) > 1) goto next;
            
            cout << numbers[i] << "\n";
            break;
            next:;
        }
    }
}
