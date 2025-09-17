#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto imparrotator = [&]() {
        cout << "Im-parrot-ator!";
        exit(0);
    };
    
    string s;
    unordered_set<string> seen;
    bool cat = false, night = false, walnuts = false, asked = false;
    while (getline(cin, s) && s != "all the birds") {
        if (s == "Kermit" || s == "Kermit Kermit" || s == "Kermit Kermit Kermit") continue;
        else if (s == "awwww kitty") cat = true;
        else if (s == "night night") {
            if (night) imparrotator();
            night = true;
        } else if (s == "mmmmmm") {
            if (!night) imparrotator();
            walnuts = true;
        } else if (s == "what are you doing") {
            if (asked || night) imparrotator();
            asked = true;
        } else if (s.size() > 3 && !s.rfind("Hi ", 0)) {
            if (s.find(' ', 3) != string::npos) imparrotator();
            else {
                auto name = s.substr(3);
                if (!isupper(name[0]) || any_of(name.begin() + 1, name.end(), [&](char c) { return !islower(c); })) imparrotator();
                else {
                    if (name == "Daniel") imparrotator();
                    else if (name == "Roger") seen.emplace("Daniel");
                    else seen.emplace(name);
                }
            }
        } else imparrotator();
    }
    
    if (!night) imparrotator();
    else {
        cout << "Could be Kermit\n";

        vector<string> day;
        if (walnuts) day.emplace_back("He had walnuts");
        if (asked) day.emplace_back("His human worked from home");
        if (cat) day.emplace_back("There was a cat or a small dog");
        for (auto &name : seen) day.emplace_back("He saw " + name);
        sort(day.begin(), day.end());
        for (auto &line : day) cout << line << "\n";
    }
}
