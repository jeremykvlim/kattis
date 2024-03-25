#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string s;
    cin >> s;

    int north = 0, south = 1, temp;
    for (char c : s) {
        if (c == 'N') north++;
        else if (c == 'S') south++;
        else {
            north++;
            south++;
        }

        temp = south;
        south = min(south, north + 1);
        north = min(north, temp + 1);
    }

    cout << north;
}
