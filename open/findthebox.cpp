#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int h, w;
    cin >> h >> w;
    cout << "? " << string(h - 1, 'v') << string(w - 1, '>') << "\n" << flush;

    int r, c;
    cin >> r >> c;

    if (r + 1 != h) {
        cout << "! " << r + 1 << " 0" << flush;
        exit(0);
    }

    if (c + 1 != w) {
        cout << "! " << h - 1 << " " << c + 1 << flush;
        exit(0);
    }

    cout << "? " << string(h - 2, 'v');
    for (int i = 0; i < h - 1; i++) cout << string(w - 1, '>') << "v>^" << string(w - 1, '<') << "^";
    cout << string(w - 1, '>') << "\n" << flush;

    cin >> r >> c;
    cout << "! " << r << " " << c + 1 << flush;
}
