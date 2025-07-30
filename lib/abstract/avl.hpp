//
// Created by frostzt on 7/13/25.
//

#ifndef AVL_HPP
#define AVL_HPP

#include <iostream>
#include <memory>
#include <vector>

template<typename T>
class Node {
public:
    T key;
    int height;
    std::unique_ptr<Node<T> > left;
    std::unique_ptr<Node<T> > right;

    explicit Node(const T &val) : key(val), height(1), left(nullptr), right(nullptr) {
    }

    static int getHeight(const std::unique_ptr<Node<T> > &node);

    static int getBalance(const std::unique_ptr<Node<T> > &node);
};

template<typename T>
class AVLTree {
private:
    std::unique_ptr<Node<T> > _root;

    int size_;

    void insert(std::unique_ptr<Node<T> > &root, const T &key);

    void remove(std::unique_ptr<Node<T> > &root, const T &key);

    static void inorder(const std::unique_ptr<Node<T> > &root);

    static Node<T> *search(const std::unique_ptr<Node<T> > &root, const T &key);

    static std::unique_ptr<Node<T> > &findNodeRef(std::unique_ptr<Node<T> > &root, const T &key);

    void inOrderIntoVector(const std::unique_ptr<Node<T> > &root, std::vector<T> &_v) const;

    static void rotateLeft(std::unique_ptr<Node<T> > &node);

    static void rotateRight(std::unique_ptr<Node<T> > &node);

    static std::unique_ptr<Node<T> > &findLeftMost(std::unique_ptr<Node<T> > &node);

    static const std::unique_ptr<Node<T> > &findLeftMost(const std::unique_ptr<Node<T> > &node);

    static std::unique_ptr<Node<T> > &findRightMost(std::unique_ptr<Node<T> > &node);

    static const std::unique_ptr<Node<T> > &findRightMost(const std::unique_ptr<Node<T> > &node);

public:
    void insert(const T &key);

    void inorder() const;

    void remove(const T &key);

    void printTree() const;

    void clear();

    std::vector<T> toSortedVector() const;

    [[nodiscard]] Node<T> *search(const T &key) const;

    AVLTree(): _root(nullptr), size_(0) {
    }
};

template<typename T>
int Node<T>::getHeight(const std::unique_ptr<Node<T> > &node) {
    if (node) {
        return node->height;
    }

    return 0;
}

template<typename T>
int Node<T>::getBalance(const std::unique_ptr<Node<T> > &node) {
    if (!node) {
        return 0;
    }

    return getHeight(node->left) - getHeight(node->right);
}

template<typename T>
void AVLTree<T>::clear() {
    _root = nullptr;
}

template<typename T>
void AVLTree<T>::rotateLeft(std::unique_ptr<Node<T> > &node) {
    const auto oldNode = node.get();

    auto newRoot = std::move(node->right);

    node->right = std::move(newRoot->left);
    newRoot->left = std::move(node);

    oldNode->height = 1 + std::max(Node<T>::getHeight(oldNode->left), Node<T>::getHeight(oldNode->right));
    newRoot->height = 1 + std::max(Node<T>::getHeight(newRoot->left), Node<T>::getHeight(newRoot->right));

    node = std::move(newRoot);
}

template<typename T>
void AVLTree<T>::rotateRight(std::unique_ptr<Node<T> > &node) {
    const auto oldNode = node.get();

    auto newRoot = std::move(node->left);

    node->left = std::move(newRoot->right);
    newRoot->right = std::move(node);

    oldNode->height = 1 + std::max(Node<T>::getHeight(oldNode->left), Node<T>::getHeight(oldNode->right));
    newRoot->height = 1 + std::max(Node<T>::getHeight(newRoot->left), Node<T>::getHeight(newRoot->right));

    node = std::move(newRoot);
}

template<typename T>
void AVLTree<T>::remove(const T &key) {
    remove(_root, key);
}

template<typename T>
void AVLTree<T>::remove(std::unique_ptr<Node<T> > &root, const T &key) {
    if (!root) return;

    if (std::less<T>{}(root->key, key)) {
        remove(root->right, key);
    } else if (std::less<T>{}(key, root->key)) {
        remove(root->left, key);
    } else {
        if (!root->right) {
            root = std::move(root->left);
        } else if (!root->left) {
            root = std::move(root->right);
        } else {
            const auto &successor = findLeftMost(root->right);
            root->key = successor->key;
            remove(root->right, successor->key);
        }

        size_--;
    }

    if (!root) return;

    // Update the height
    root->height = 1 + std::max(Node<T>::getHeight(root->left), Node<T>::getHeight(root->right));

    if (const int balance = Node<T>::getBalance(root); balance) {
        // LL
        if (balance > 1 && Node<T>::getBalance(root->left) >= 0) {
            rotateRight(root);
        }
        // LR
        else if (balance > 1 && Node<T>::getBalance(root->left) < 0) {
            rotateLeft(root->left);
            rotateRight(root);
        }
        // RR
        else if (balance < -1 && Node<T>::getBalance(root->right) <= 0) {
            rotateLeft(root);
        }
        // RL
        else if (balance < -1 && Node<T>::getBalance(root->right) > 0) {
            rotateRight(root->right);
            rotateLeft(root);
        }
    }
}


template<typename T>
void AVLTree<T>::insert(const T &key) {
    insert(_root, key);
}

template<typename T>
void AVLTree<T>::insert(std::unique_ptr<Node<T> > &root, const T &key) {
    if (root == nullptr) {
        size_++;
        root = std::make_unique<Node<T> >(key);
        return;
    }

    // Prevent duplicate insert
    if (root->key == key) {
        root->key = key;
        return;
    }

    if (std::less<T>{}(root->key, key)) {
        insert(root->right, key);
        root->height = 1 + std::max(Node<T>::getHeight(root->left), Node<T>::getHeight(root->right));
    } else {
        insert(root->left, key);
        root->height = 1 + std::max(Node<T>::getHeight(root->left), Node<T>::getHeight(root->right));
    }

    if (const int balance = Node<T>::getBalance(root); balance) {
        if (balance > 1 && std::less<T>{}(key, root->left->key)) {
            // 1. LL Case
            rotateRight(root);
        } else if (balance < -1 && std::less<T>{}(root->right->key, key)) {
            // 2. RR Case
            rotateLeft(root);
        } else if (balance > 1 && std::less<T>{}(root->left->key, key)) {
            // 3. LR Case
            rotateLeft(root->left);
            rotateRight(root);
        } else if (balance < -1 && std::less<T>{}(key, root->right->key)) {
            // 4. RL Case
            rotateRight(root->right);
            rotateLeft(root);
        }
    }
}

template<typename T>
void AVLTree<T>::inOrderIntoVector(const std::unique_ptr<Node<T> > &root, std::vector<T> &_v) const {
    if (root == nullptr) {
        return;
    }

    inOrderIntoVector(root->left, _v);
    _v.push_back(root->key);
    inOrderIntoVector(root->right, _v);
}

template<typename T>
std::vector<T> AVLTree<T>::toSortedVector() const {
    std::vector<T> data;
    data.reserve(size_);

    inOrderIntoVector(_root, data);
    return data;
}


template<typename T>
void AVLTree<T>::inorder() const {
    inorder(_root);
    std::cout << std::endl;
}

template<typename T>
void AVLTree<T>::inorder(const std::unique_ptr<Node<T> > &root) {
    if (root == nullptr) {
        return;
    }

    inorder(root->left);
    if constexpr (std::is_same_v<T, int> || std::is_same_v<T, std::string>) {
        std::cout << root->key << "(H:" << root->height
                << ", B:" << Node<T>::getBalance(root) << ") ";
    }
    inorder(root->right);
}

template<typename T>
std::unique_ptr<Node<T> > &AVLTree<T>::findLeftMost(std::unique_ptr<Node<T> > &node) {
    if (!node) {
        throw std::invalid_argument("Cannot find leftmost of a null subtree");
    }

    if (node->left) {
        return findLeftMost(node->left);
    } else {
        return node;
    }
}

template<typename T>
const std::unique_ptr<Node<T> > &AVLTree<T>::findLeftMost(const std::unique_ptr<Node<T> > &node) {
    if (!node) {
        throw std::invalid_argument("Cannot find leftmost of a null subtree");
    }

    if (node->left) {
        return findLeftMost(node->left);
    } else {
        return node;
    }
}

template<typename T>
std::unique_ptr<Node<T> > &AVLTree<T>::findRightMost(std::unique_ptr<Node<T> > &node) {
    if (!node) {
        throw std::invalid_argument("Cannot find rightmost of a null subtree");
    }

    if (node->right) {
        return findRightMost(node->right);
    } else {
        return node;
    }
}

template<typename T>
const std::unique_ptr<Node<T> > &AVLTree<T>::findRightMost(const std::unique_ptr<Node<T> > &node) {
    if (!node) {
        throw std::invalid_argument("Cannot find rightmost of a null subtree");
    }

    if (node->right) {
        return findRightMost(node->right);
    } else {
        return node;
    }
}

template<typename T>
void AVLTree<T>::printTree() const {
    printTree(_root, 0);
}

template<typename T>
std::unique_ptr<Node<T> > &AVLTree<T>::findNodeRef(std::unique_ptr<Node<T> > &root, const T &key) {
    if (root == nullptr || root->key == key) {
        return root;
    }

    if (std::less<T>{}(key, root->key)) {
        return findNodeRef(root->left, key);
    } else {
        return findNodeRef(root->right, key);
    }
}

template<typename T>
Node<T> *AVLTree<T>::search(const T &key) const {
    return search(_root, key);
}

template<typename T>
Node<T> *AVLTree<T>::search(const std::unique_ptr<Node<T> > &root, const T &key) {
    if (root == nullptr || root->key == key) {
        return root.get();
    }

    if (std::less<T>{}(key, root->key)) {
        if (auto const foundNode = search(root->left, key); foundNode != nullptr) {
            return foundNode;
        }
    } else {
        if (auto const foundNode = search(root->right, key); foundNode != nullptr) {
            return foundNode;
        }
    }

    return nullptr;
}

#endif //AVL_HPP
