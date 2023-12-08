#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    tree<string, null_type, less<string>, rb_tree_tag, tree_order_statistics_node_update>  male, female;
    int command;
    while (cin >> command && command) {
        switch (command) {
            case 1: {
                string name;
                int gender;
                cin >> name >> gender;
                gender == 1 ? male.insert(name) : female.insert(name);
                break;
            }
            case 2: {
                string name;
                cin >> name;
                auto it = male.lower_bound(name);
                if (*it == name) male.erase(male.lower_bound(name));
                else female.erase(female.lower_bound(name));
                break;
            }
            case 3: {
                string start, end;
                int gender, count;
                cin >> start >> end >> gender;
                count = (gender == 0 || gender == 1 ? male.order_of_key(end) - male.order_of_key(start) : 0) +
                        (gender == 0 || gender == 2 ? female.order_of_key(end) - female.order_of_key(start) : 0);
                cout << count << "\n";
                break;
            }
        }
    }
}
