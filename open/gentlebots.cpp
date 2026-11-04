#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<array<int, 6>> robots(2);
    for (auto &[x0, y0, z0, xinf, yinf, zinf] : robots) cin >> x0 >> y0 >> z0 >> xinf >> yinf >> zinf;

    auto print = [&]() {
        cout << "(" << robots[0][0] << " " << robots[0][1] << " " << robots[0][2] << ") (" << robots[1][0] << " " << robots[1][1] << " " << robots[1][2] << ")\n";
    };
    print();

    for (int i = 0; i < 3; i++)
        if (robots[0][i] == robots[1][i]) {
            robots[0][i]++;
            print();
        }

    auto move = [&](int i) {
        for (int r = 0; r < 2; r++) {
            int delta = robots[r][i] > robots[r][i + 3] ? -1 : 1;
            while (robots[r][i] != robots[r][i + 3]) {
                robots[r][i] += delta;
                print();
            }
        }
    };

    for (int i = 0; i < 3; i++)
        if (robots[0][i + 3] != robots[1][i + 3]) {
            move(i);
            break;
        }

    for (int i = 0; i < 3; i++) move(i);
}