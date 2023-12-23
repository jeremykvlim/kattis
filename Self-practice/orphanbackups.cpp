#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    map<string, bool> images;
    set<string> files;
    string s;
    while (getline(cin, s), !s.empty()) images[s] = true;
    while (cin >> s >> ws) {
        auto name = s.substr(0, s.rfind('_', s.rfind('_') - 1));
        if (images.count(name)) images[name] = false;
        else files.emplace(s);
    }

    if (files.empty() && all_of(images.begin(), images.end(), [&](auto p) {return !p.second;})) {
        cout << "No mismatches.\n";
        exit(0);
    }

    for (auto &f : files) cout << "F " << f << "\n";
    for (auto &[i, j] : images)
        if (j) cout << "I " << i << "\n";
}
