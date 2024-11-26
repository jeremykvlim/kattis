#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    unordered_map<string, list<string>::iterator> partner;
    list<string> people;
    while (n--) {
        string p1, p2;
        cin >> p1 >> p2;

        partner[p2] = people.emplace(people.end(), p1);
        partner[p1] = people.emplace(people.end(), p2);
    }
    auto mic = people.begin();

    string q;
    cin >> q;

    for (char c : q) {
        if (c == 'P') cout << *partner[*mic] << "\n";
        else if (c == 'F') mic = mic != people.begin() ? prev(mic) : prev(people.end());
        else if (c == 'B') mic = mic != prev(people.end()) ? next(mic) : people.begin();
        else if (c == 'R') {
            auto temp = mic;
            mic = mic != prev(people.end()) ? next(mic) : people.begin();
            people.splice(people.end(), people, temp);
        } else {
            auto temp = mic;
            mic =  mic != prev(people.end()) ? next(mic) : people.begin();
            people.splice(next(partner[*temp]), people, temp);
        }
    }
    cout << "\n";

    for (auto p : people) cout << p << "\n";
}
