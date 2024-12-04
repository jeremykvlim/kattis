#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    array<int, 4> X;
    for (int &Xi : X) cin >> Xi;

    auto losing = [&]() {
        int lsb = (X[1] - X[0]) & (-(X[1] - X[0]));
        return ((X[2] - X[1] - 1) % (lsb * 2) != lsb - 1 || (X[3] - X[2] - 1) % (lsb * 2) != lsb - 1);
    };
    
    auto read = [&]() {
        char c;
        cin >> c;

        if (c == 'M') {
            int x, y;
            cin >> x >> y;

            if (X[3] == x) X[3] = y;
            else if (X[0] == x) X[0] = y;

            sort(X.begin(), X.end());
        } else exit(0);
    };

    if (losing()) cout << "1\n" << flush;
    else {
        cout << "2\n" << flush;
        read();
    }

    for (;;) {
        auto move = [&](int y, int i, int j) {
            auto winning = [&](int y, int j) {
                auto temp = X;
                X[j] = y;
                sort(X.begin(), X.end());
                bool b = losing();
                X = temp;
                return !b;
            };
            
            if (y < X[i] && winning(y, j)) {
                cout << "M " << X[j] << " " << y << "\n" << flush;

                X[j] = y;
                sort(X.begin(), X.end());
                read();
                return true;
            }
            return false;
        };

        if (!move(X[1] + ((X[3] - X[2]) & -(X[3] - X[2])), 2, 0) &&
            !move(X[2] + ((X[2] - X[1]) & -(X[2] - X[1])), 3, 0) &&
            !move(X[0] + ((X[2] - X[1]) & -(X[2] - X[1])), 1, 3) &&
            !move(X[1] + ((X[2] - X[1]) & -(X[2] - X[1])), 2, 3)) {
            cout << "D\n" << flush;
            exit(0);
        }
    }
}
