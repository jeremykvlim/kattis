#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    while (cin >> q) {
        cin.ignore();
        
        string line;
        unordered_map<string, int> count;
        while (getline(cin, line) && line != "IAMIRONMAN") {
            string word;
            for (char c : line)
                if (isalpha(c)) word += tolower(c);
                else {
                    if (!word.empty()) {
                        count[word]++;
                        word.clear();
                    }
                }
            if (!word.empty()) count[word]++;
        }
        vector<string> words;
        for (auto [w, c] : count)
            if (c == q) words.emplace_back(w);
        sort(words.begin(), words.end());

        if (words.empty()) cout << "Guess there is no such thing as a Starkism.";
        else
            for (auto w : words) cout << w << "\n";
    }
}
