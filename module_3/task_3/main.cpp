/*
 * Дано число N < 10^6 и последовательность пар целых чисел из [-2^31...2^31] длиной N.
 * Построить декартово дерево из N узлов, характеризующееся парами чисел {X_i, Y_i}.
 * Каждая пара чисел {X_i, Y_i} определяет ключ X_i и приоритет Y_i в декартовом дереве.
 * Добавление узла в декартово дерево выйолняйте второй версией алгоритма,
 * рассказанного на лекции:
 *      При добавлении узла (x, y) выполняйте спуск по ключу до узла P с меньшим
 *      приоритетом. Затем разбейте найденного поддерево по ключу x так, чтобы в первом
 *      поддереве все ключи меньше x, а во втором больше или равны x.
 *      Получившиеся два дерева сделайте дочерними для нового узла (x, y). Новый узел
 *      вставьте на место узла P.
 *
 * Построить также наивное дерево поиска по ключам X_i методом из задачи 2.
 *
 * Вычислить количество излов в самом широком слое декартова дерева и
 * количество узлов в самом широком слое наивного дерева поиска.
 * Вывести их разницу. Разница может быть отрицательна.
 */

#include <iostream>
#include <assert.h>

class BinaryNode {
public:
    BinaryNode(int key=0, BinaryNode* parent= nullptr): left{nullptr}, right{nullptr}, parent{parent}, key_{key} {};

    ~BinaryNode() {
        delete(left);
        delete(right);
    }

    void add(int key) {
        BinaryNode*& addition_point = getNext(key);
        if (addition_point == nullptr) {
            addition_point = new BinaryNode(key, this);
        } else {
            addition_point->add(key);
        }
    }

    BinaryNode*& getNext(int key) {
        if (key_ <= key) {
            return right;
        } else {
            return left;
        }
    }

    int key_;
    BinaryNode* left;
    BinaryNode* right;
    BinaryNode* parent;
};

class SearchTree {
public:
    SearchTree(): root(nullptr) {};
    ~SearchTree() {
        delete root;
    }

    void add(int key) {
        if (root == nullptr) {
            root = new BinaryNode();
            root->key_ = key;
        } else {
            root->add(key);
        }
    }

    BinaryNode* find(int key) {
        assert(root != nullptr);
        auto node = root;

        while (node != nullptr) {
            if (node->key_ == key) {
                return node;
            } else if (node->key_ > key) {
                node = node->left;
            } else {
                node = node->right;
            }
        }
        return nullptr;
    }

    BinaryNode* findMinimum() {
        assert(root != nullptr);
        auto node = root;

        while (node->left != nullptr) {
            node = node->left;
        }

        return node;
    }

    void erase(int key) {
        if (root == nullptr) {
            return;
        }

        auto node = root;
        while (node != nullptr) {
            if (root->key_ > key) {
                node = node->left;
            } else if (node->key_ < key) {
                node = node->right;
            }

            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                node = nullptr;
            } else if (node->left == nullptr) {
                auto temp = node->right;
                delete node;
                node = temp;
            } else if (node->left != nullptr) {
                auto temp = node->left;
                delete node;
                node = temp;
            } else {
                // TODO реализовать третий случай
            }
        }
    }

private:

    BinaryNode* root;
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}