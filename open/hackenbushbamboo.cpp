#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    
    int grundy = 0;
    while (n--) {
        int h;
        cin >> h;
        
        grundy ^= h;
    }
    cout << "*" << grundy;
}