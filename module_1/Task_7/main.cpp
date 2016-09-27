#include <iostream>


class Athlete {
public:
    Athlete (int mass, int strength): mass(mass), strength(strength) {};

    int mass;
    int strength;
};


class Node {
public:
    Node() {
        this->next = NULL;
        this->state = NULL;
    }

    Node(Athlete& athlete) {
        this->next = NULL;
        this->state = new Athlete(athlete.mass, athlete.strength);
    }

    void set_next(Node& node) {
        *(this->next) = node;
    }

    void insert_after(Athlete element) {
        Node node = Node(element);

        if (this->next == NULL) {
            set_next(element);
        }

        if ()

    }


private:
    Athlete* state;
    Node* next;
};


int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}