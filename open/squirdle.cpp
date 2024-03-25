#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string present, guess(5, '-');
    for (int i = 0; i < 5; i++) {
        string s, response;
        for (int j = 0; j < 5; j++) s += (char) ('a' + 5 * i + j);
        cout << "? " + s + "\n" << flush;
        cin >> response;

        if (response == "22222") exit(0);

        for (int j = 0; j < 5; j++)
            if (response[j] != '0') present += s[j];
    }

    if (present.size() < 5) present += 'z';

    for (int i = 0; i < 4; i++) {
        string s, response;
        for (int j = 0; j < 5; j++) s += present[i];
        cout << "? " + s + "\n" << flush;
        cin >> response;

        if (response == "22222") exit(0);

        guess[response.find('2')] = present[i];
    }

    for (char &c : guess)
        if (c == '-') c = present[4];
    cout << "? " + guess + "\n" << flush;
}
