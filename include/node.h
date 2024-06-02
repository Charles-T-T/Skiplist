#pragma once

#include <iostream>
#include <memory>
#include <vector>

// 跳表节点
template <typename K, typename V>
class Node
{
private:
    K key;
    V value;
    int nodeLevel;

public:
    Node(K k, V v, int level) // 带参数的构造函数
        : key(std::move(k)), value(std::move(v)), nodeLevel(level), forward(level + 1) {}
    Node() = delete;   // 删除默认构造函数
    ~Node() = default; // 默认析构函数

    K GetKey() const { return key; }
    V GetValue() const { return value; }
    void SetValue(V v) { value = std::move(v); } // 支持移动语义

    std::vector<std::shared_ptr<Node<K, V>>> forward; // 用于实现搜索跳表的forward指针，指向各层的下一个节点
};