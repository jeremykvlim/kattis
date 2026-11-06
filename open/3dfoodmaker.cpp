#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int x = 1;
    auto parse = [&](auto &&self) {
        string l, op, r, _;
        cin >> l;
        if (l != "(") return l;

        l = self(self);
        cin >> op;
        r = self(self);
        cin >> _;

        auto bowl = "bowl_" + to_string(x++);
        cout << "LOAD " << l << "\n" << op << " " << r << "\n" << "STORE " << bowl << "\n";
        return bowl;
    };
    auto all = parse(parse);
    cout << "BAKE " << all;
}
