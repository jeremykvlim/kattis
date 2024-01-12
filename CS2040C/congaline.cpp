#include<bits/stdc++.h>
using namespace std;

struct Person {
    string name;
    Person *next, *prev, *partner;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    Person *front = nullptr, *back = nullptr;
    while (n--) {
        auto *p1 = new Person(), *p2 = new Person();
        cin >> p1->name >> p2->name;

        p1->partner = p2;
        p2->partner = p1;

        if (!front) {
            front = p1;
            back = p2;
        } else {
            back->next = p1;
            p1->prev = back;
            back = p2;
        }

        p1->next = p2;
        p2->prev = p1;
    }

    front->prev = back;
    back->next = front;

    auto *mic = front;

    string instructions;
    cin >> instructions;

    for (char c : instructions) {
        if (c == 'P') cout << mic->partner->name << "\n";
        else if (c == 'F') mic = mic->prev;
        else if (c == 'B') mic = mic->next;

        else if (c == 'R') {
            auto *prev = mic->prev, *next = mic->next;

            if (mic == front) {
                front = next;
                back = mic;
            } else if (mic != back) {
                prev->next = next;
                next->prev = prev;

                back->next = mic;
                mic->prev = back;
                front->prev = mic;
                mic->next = front;

                back = mic;
            }

            mic = next;

        } else {
            auto *prev = mic->prev, *next = mic->next, *partner = mic->partner, *partner_next = partner->next;

            if (prev != partner) {
                prev->next = next;
                next->prev = prev;

                partner->next = mic;
                mic->prev = partner;
                partner_next->prev = mic;
                mic->next = partner_next;

                front = (mic == front) ? next : front;
                back = (mic == back) ? prev : (partner == back) ? mic : back;
            } else if (partner == back) {
                front = next;
                back = mic;
            }

            mic = next;
        }
    }

    cout << "\n";

    auto *person = front;
    do {
        cout << person->name << "\n";
        person = person->next;
    } while (person != front);
}
