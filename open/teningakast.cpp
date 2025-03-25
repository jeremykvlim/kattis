#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;
    cin.ignore();

    while (q--) {
        string s;
        getline(cin, s);
        s += "+0";

        s = regex_replace(s, regex("d1-"), "-");
        s = regex_replace(s, regex("d1\\+"), "+");

        auto split = [&](const string &s, const char &c) {
            vector<string> rolls;
            istringstream iss(s);
            string roll;
            while (getline(iss, roll, c)) rolls.emplace_back(roll);
            return rolls;
        };

        unordered_map<int, int> add, sub, add_e, sub_e;
        int base = 0;
        for (auto plus : split(s, '+')) {
            auto minus = split(plus, '-');

            bool pos = true;
            if (!minus.empty() && minus[0].empty()) {
                minus.erase(minus.begin());
                pos = false;
            }

            for (auto roll : minus) {
                if (roll.empty()) continue;

                if (roll.find('d') != string::npos) {
                    int d = roll.find('d');
                    auto l = roll.substr(0, d), r = roll.substr(d + 1);

                    bool explode = false;
                    if (!r.empty() && r.back() == '!') {
                        explode = true;
                        r.pop_back();
                    }

                    int n = stoi(l), m = stoi(r);
                    if (pos) {
                        if (explode) add_e[m] += n;
                        else add[m] += n;
                    } else {
                        if (explode) sub_e[m] += n;
                        else sub[m] += n;
                    }
                } else base += pos ? stoi(roll) : -stoi(roll);

                if (pos) pos = false;
            }
        }

        long long r;
        cin >> r;
        cin.ignore();

        long long rl = base, rr = base;
        for (auto [m, n] : add) {
            rl += n;
            rr += n * m;
        }
        for (auto [m, n] : sub) {
            rl -= n * m;
            rr -= n;
        }

        vector<int> sides;
        int count_pos_e = 0, count_neg_e = 0;
        for (auto [m, n] : add_e) {
            sides.emplace_back(m);
            count_pos_e += n;
        }
        for (auto [m, n] : sub_e) {
            sides.emplace_back(m);
            count_neg_e += n;
        }
        sort(sides.begin(), sides.end());
        sides.erase(unique(sides.begin(), sides.end()), sides.end());

        if (count_pos_e) rr = r + 1;
        else
            for (auto [m, n] : sub_e) rr -= n;

        if (count_neg_e) rl = r - 1;
        else
            for (auto [m, n] : add_e) rl += n;

        if (!(rl <= r && r <= rr)) {
            cout << "Svindl\n";
            continue;
        }

        r -= base;
        if (sides.size() == 1 && sides[0] == 2) {
            r -= count_pos_e + count_neg_e;
            r = (r % 2 + 2) % 2;
            cout << (!r ? "Raunhaeft\n" : "Svindl\n");
            continue;
        }

        if (count_pos_e + count_neg_e == 1) {
            if (sides.empty()) sides.emplace_back(0);
            r = (r % sides[0] + sides[0]) % sides[0];
            cout << (r ? "Raunhaeft\n" : "Svindl\n");
            continue;
        }

        cout << "Raunhaeft\n";
    }
}
