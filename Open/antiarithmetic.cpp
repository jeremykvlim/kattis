#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<int> indices(n);
        cin.ignore(1);
        for (int i = 0; i < n; i++) {
            int term;
            cin >> term;
            indices[term] = i;
        }
      
        bool antiarithmetic = true;
        for (int i = 1; antiarithmetic && i < n; i++)
            for (int j = i + i; j < n; j++)
                if ((indices[j] < indices[j - i]) == (indices[j - i] < indices[j - i - i])) {
                    antiarithmetic = false;
                    break;
                }

        cout << (antiarithmetic ? "yes\n" : "no\n");
    }
}
