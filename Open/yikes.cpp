#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    while (n--) {
        double m, b, d, t;
        cin >> m >> b >> d >> t;

        auto center = make_pair(d - t * b, -5.5), dxdy = make_pair(-b, m);
        auto normal = hypot(dxdy.first, dxdy.second);

        auto front = make_pair(center.first + dxdy.second / normal * 0.5, center.second - dxdy.first / normal * 0.5), back = make_pair(center.first - dxdy.second / normal * 0.5, center.second + dxdy.first / normal * 0.5);
        auto front_time = front.first + dxdy.first * (-front.second / dxdy.second), back_time = back.first + dxdy.first * (-back.second / dxdy.second);

        if (back_time > 0) cout << "Max beats the first bicycle\n";
        else if (front_time - 2 < -40) cout << "Max crosses safely after bicycle 10\n";
        else {
            auto front_segment = (int) (front_time - 1e-9) / -2, back_segment = (int) (back_time - 1e-9) / -2;
            
            if (front_segment == back_segment && front_segment % 2) cout << "Max crosses safely after bicycle " << front_segment / 2 + 1 << "\n";
            else if (front_segment < 0) cout << "Collision with bicycle 1\n";
            else cout << "Collision with bicycle " << (front_segment / 2 + 1 + front_segment % 2) << "\n";
        }
    }
}
