#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string n;
    cin >> n;

    string pfs;
    auto p = popen(("factor " + n).c_str(), "r");
    const int size = 1 << 20;
    array<char, size> buf;
    while (fgets(buf.data(), size, p)) pfs += buf.data();
    pclose(p);
    cout << pfs.substr(pfs.find(": ") + 2);
}
