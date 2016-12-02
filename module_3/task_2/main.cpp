/*
 * Дано число V < 10^6 и последовательность целых чисел из [-2^31...2^31] длиной N.
 * Требуется построить бинарное дерево, заданное наивным порядком вставки.
 * Т.е., при добавлении очередного числа K в дерево с корнем root, если root->Key <= K, то
 * узел K добавляется в правое поддерево root; иначе в левое поддерево root.
 *
 * Выведите элементы в порядке level-order (по слоям, "в ширину")
 */

#include <iostream>
#include <vector>
#include <queue>
#include <fstream>

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

std::vector<int> getBFSPrintout(BinaryNode* root) {
    auto result = std::vector<int>();
    std::queue<BinaryNode*> q;
    q.push(root);

    while (!q.empty()) {
        auto node = q.front();
        result.push_back(node->key_);
        q.pop();

        if (node->left != nullptr) {
            q.push(node->left);
        }
        if (node->right != nullptr) {
            q.push(node->right);
        }
    }

    return result;
}

void readInput(BinaryNode* root, std::istream& is, size_t item_num) {
    int buffer;
    is >> root->key_;

    for (size_t i = 0; i != item_num - 1; ++i) {
        is >> buffer;
        root->add(buffer);
    }
}

int main() {
    auto root = new BinaryNode(0);
    size_t item_num;
    std::fstream ifs("/home/artem/ClionProjects/algorithm-problems/module_3/task_2/input.txt");

    //std::istream& is = ifs;
    std::istream& is = std::cin;

    is >> item_num;

    readInput(root, is, item_num);

    for (auto i: getBFSPrintout(root)) {
        std::cout << i << ' ';
    }

    delete root;
    return 0;
}