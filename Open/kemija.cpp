#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> first(n), second(n);
    for (auto &num : second) cin >> num;

    auto construct = [&](int start) -> int {
        int curr = start, smallest = 0, num = 0;
        do {
            first[curr] = num;
            smallest = min(smallest, first[curr]);
            curr = (curr + 1) % n;

            num -= second[curr];
            curr = (curr + 1) % n;

            num += second[curr];
            curr = (curr + 1) % n;
        } while (curr != start);

        return smallest;
    };

    auto update = [&](int start, int num) {
        int curr = start;
        do {
            first[curr] += num;
            curr = (curr + 3) % n;
        } while (curr != start);
    };

    if (n % 3) {
        construct(0);
        update(0, (second[1] - first[0] - first[1] - first[2]) / 3);
    } else {
        update(0, 1 - construct(0));
        update(1, 1 - construct(1));
        construct(2);
        update(2, second[1] - first[0] - first[1]);
    }

    for (auto &num : first) cout << num << "\n";
}
