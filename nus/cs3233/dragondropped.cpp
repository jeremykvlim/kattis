#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    for (int i = 0; i <= 6e3; i++) {
        cout << "NEXT GABBY\n" << flush;

        int s, e;
        cin >> s >> e;
    }

    if (n <= 12e3) {
        for (int i = 1; i < 6e3; i++) cout << "NEXT GABBY\n" << flush;
        cout << "ASK GABBY\n" << flush;
        exit(0);
    }

    for (int i = 0; i < 6e3; i++) {
        cout << "NEXT SPIKE\n" << flush;

        int s, e;
        cin >> s >> e;

        if (e) {
            for (int k = 6e3 + 1; k % ((int) 6e3 - i) != n % ((int) 6e3 - i); k++) cout << "NEXT GABBY\n" << flush;
            cout << "ASK GABBY\n" << flush;
            exit(0);
        }
    }
}
