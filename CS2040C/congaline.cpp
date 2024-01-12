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

    Person *head = nullptr, *tail = nullptr;

    for (int i = 0; i < n; i++) {
        auto *p1 = new Person(), *p2 = new Person();
        cin >> p1->name >> p2->name;

        p1->partner = p2;
        p2->partner = p1;

        if (!head) {
            head = p1;
            tail = p2;
        } else {
            tail->next = p1;
            p1->prev = tail;
            tail = p2;
        }

        p1->next = p2;
        p2->prev = p1;
    }

    head->prev = tail;
    tail->next = head;

    auto *mic = head;

    string instructions;
    cin >> instructions;

    for (char c : instructions) {
        if (c == 'P') cout << mic->partner->name << "\n";
        else if (c == 'F') mic = mic->prev;
        else if (c == 'B') mic = mic->next;

        else if (c == 'R') {
            auto *prev = mic->prev, *next = mic->next;

            if (mic == head) {
                head = next;
                tail = mic;
            } else if (mic != tail) {
                prev->next = next;
                next->prev = prev;

                tail->next = mic;
                mic->prev = tail;
                head->prev = mic;
                mic->next = head;

                tail = mic;
            }

            mic = next;

        } else if (c == 'C') {
            auto *next = mic->next, *prev = mic->prev, *partner = mic->partner, *partnerNext = partner->next;

            if (prev != partner) {
                prev->next = next;
                next->prev = prev;

                partner->next = mic;
                mic->prev = partner;
                partnerNext->prev = mic;
                mic->next = partnerNext;

                head = (mic == head) ? next : head;
                tail = (mic == tail) ? prev : (partner == tail) ? mic : tail;
            } else if (partner == tail) {
                head = next;
                tail = mic;
            }

            mic = next;
        }
    }

    cout << "\n";

    auto *it = head;
    do {
        cout << it->name << "\n";
        it = it->next;
    } while (it != head);
}
