#include <bits/stdc++.h>
using namespace std;

struct Circle {
    int x, y, r, a, b, i;

    Circle(int x = 0, int y = 0, int r = 0, int a = 0, int b = 0, int i = 0): x(x), y(y), r(r), a(a), b(b), i(i) {}

    bool operator<(const Circle &c) const {
        return r < c.r;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Circle> circles(n);
    for (int i = 0; i < n; i++) {
        cin >> circles[i].x >> circles[i].y >> circles[i].r >> circles[i].a >> circles[i].b;
        circles[i].i = i + 1;
    }
    sort(circles.begin(), circles.end());

    vector<int> energy(n, 0), best(n, 0), ancestors(n, 0), parent(n, 0), required(n, 0);
    int most = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++)
            if (pow(circles[i].x - circles[j].x, 2) + pow(circles[i].y - circles[j].y, 2) < pow(circles[j].r, 2)) {
                if (!parent[i]) parent[i] = j;

                energy[i] += (++ancestors[i] & 1 ? circles[i].a : circles[i].b);
                best[i] = max(best[i], energy[i]);
            }

        most += best[i];
        for (int j = ancestors[i], curr = energy[i]; ~j; j--) {
            if (curr == best[i]) required[i] = j;
            curr -= (j & 1 ? circles[i].a : circles[i].b);
        }
    }

    cout << most << "\n";

    vector<bool> activated(n), skip(n, false);
    while (n--) {
        fill(skip.begin(), skip.end(), false);
        int draw = -1;
        for (int i = 0; i < activated.size(); i++) {
            if (skip[i]) skip[parent[i]] = true;
            if (activated[i] || skip[i]) continue;
            if (energy[i] == best[i] && (draw == -1 || circles[i].i < circles[draw].i)) draw = i;
            if (ancestors[i] == required[i]) skip[parent[i]] = true;
        }

        activated[draw] = true;
        for (int i = 0; i < draw; i++)
            if (pow(circles[i].x - circles[draw].x, 2) + pow(circles[i].y - circles[draw].y, 2) < pow(circles[draw].r, 2)) energy[i] -= (ancestors[i]-- & 1 ? circles[i].a : circles[i].b);

        cout << circles[draw].i << (n ? " " : "");
    }
}
