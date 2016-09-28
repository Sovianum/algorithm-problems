/*
 * В город N приехал цирк с командой атлетов. Они хотя удивить горожан города N - выстроить из своих тел башню максимальной высоты.
 * Башня - это цепочка атлетов, первый стоит на земле, второй стоит у него на плечах, третий стоит на плечах у второго и т.д.
 * Каждый атлет характеризуется силой s_i (kg) и массой m_i (kg). Сила - это максимальная масса, которую атлет способен держать у себя на плечах.
 * К сожалению, ни один из атлетов не умеет программировать, так как всю жизнь они занимались физической подготовкой, и у них
 * не было времени на изучение языков программирования. Помогите им, напишите программу, которая определит максимальную высоту башни, которую они
 * могут составить.
 * Известно, что если атлет тяжелее, то он и сильнее: если m_i > m_j, то s_i > s_j.
 * Атлеты равной массы могут иметь различную силу.
 */

#include <iostream>
#include <assert.h>


class Athlete {
public:
    Athlete (int mass, int strength): mass(mass), strength(strength) {};

    int mass;
    int strength;
};

class Node {
public:
    friend class List;

    Node() {
        this->prev = NULL;
        this->next = NULL;
        this->state = NULL;
    }

    ~Node() {
        delete this->state;
        if (this->next != NULL) {
            delete(this->next);
        }
    }

    Node(Athlete& element) {
        this->next = NULL;
        this->state = new Athlete(element.mass, element.strength);
    }

    Athlete& get_state() {
        return *state;
    }

    Node* get_next() {
        return this->next;
    }

    Node* get_prev() {
        return this->prev;
    }

    bool is_head() {
        return this->prev == NULL;
    }

    bool is_tail() {
        return this->next == NULL;
    }


private:
    Athlete* state;
    Node* next;
    Node* prev;
};


class List {
public:
    List() {
        this->head = NULL;
        this->tail = this->head;
    }

    ~List() {
        delete this->head;
    }

    bool is_empty() {
        return this->head == NULL;
    }

    Node* get_head() {
        return this->head;
    }

    void insert_after(Node& node, Athlete& state) {
        Node* new_node = new Node(state);
        new_node->next = node.next;
        new_node->prev = &node;

        if (node.next->prev != NULL) {
            node.next->prev = new_node;
        }
        node.next = new_node;
    }

    void push_back(Athlete& state) {
        if (this->is_empty()) {
            this->head = new Node(state);
            this->tail = this->head;
            return;
        }

        Node* new_node_ptr = new Node(state);
        this->tail->next = new_node_ptr;
        new_node_ptr->prev = this->tail;
        this->tail = new_node_ptr;


    }

    void remove_element(Node* node) {
        if (node->next != NULL) {
            node->next->prev = node->prev;
        }

        if (node->prev != NULL) {
            node->prev->next = node->next;
        }

        node->next = node->prev = NULL;
        delete node;
    }

private:
    Node* head;
    Node* tail;
};


Node* get_strongest_athlete_node(List& list) {
    assert(!list.is_empty());
    Node* node_ptr = list.get_head();
    int max_strength = -1;
    Node* strongest = NULL;

    while (!node_ptr->is_tail()) {
        if (node_ptr->get_state().strength > max_strength) {
            max_strength = node_ptr->get_state().strength;
            strongest = node_ptr;
        }

        node_ptr = node_ptr->get_next();
    }

    return strongest;
}

Node* get_next_athlete_node(List& list, int& strength_left) {
    assert(!list.is_empty());

    Node* node_ptr = list.get_head();
    Node* next_athlete_node = NULL;

    int new_strength_left = -1;

    while (!node_ptr->is_tail()) {
        Athlete athlete = node_ptr->get_state();
        if (athlete.mass <= strength_left) {      // если предыдущие атлеты выдержат нового
            int old_athlete_strength_left = strength_left - athlete.mass;   // предел сил старых атлетов с учетом добавления нового

            if (athlete.strength > new_strength_left && old_athlete_strength_left > new_strength_left) { // если добавление этого атлета приводит к более слабому ослаблению башни
                new_strength_left = old_athlete_strength_left > athlete.strength ? athlete.strength : old_athlete_strength_left;  // выбираем слабое звено
                next_athlete_node = node_ptr;
            }
        }

        node_ptr = node_ptr->get_next();
    }

    return next_athlete_node;

}

void test_list() {
    List list = List();
    Athlete athlete = Athlete(10, 10);
    list.push_back(athlete);
    Node* head = list.get_head();
    std::cout << head->get_state().mass << ' ' << head->get_state().strength;
}

int main() {
    int i = 0;
    while (std::cin >> i) {
        std::cout << i << std::endl;
    }
    std::cout << "Done";
    return 0;
}