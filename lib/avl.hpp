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
class BinarySearchTree {
private:
    std::unique_ptr<Node<T> > _root;

    int size_;

    void insert(std::unique_ptr<Node<T> > &root, const T &key);

    void remove(std::unique_ptr<Node<T> > &root, const T &key);

    static void inorder(const std::unique_ptr<Node<T> > &root);

    static Node<T> *search(const std::unique_ptr<Node<T> > &root, const T &key);

    static std::unique_ptr<Node<T> > &findNodeRef(std::unique_ptr<Node<T> > &root, const T &key);

    void inOrderIntoVector(std::unique_ptr<Node<T> > &root, std::vector<T> &_v) const;

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

    BinarySearchTree(): _root(nullptr), size_(0) {
    }
};

#endif //AVL_HPP
