#pragma once

#include <iostream>
#include <memory>
#include <vector>

// 跳表节点
template <typename K, typename V>
class Node
{
private:
    K _key;
    V _value;
    int nodeLevel;

public:
    Node(K k, V v, int level) // 带参数的构造函数
        : _key(std::move(k)), _value(std::move(v)), nodeLevel(level), _forward(level + 1)
    {
    }
    Node() = delete;   // 删除默认构造函数
    ~Node() = default; // 默认析构函数

    K GetKey() const { return _key; }
    V GetValue() const { return _value; }
    void SetValue(V v) { _value = std::move(v); } // 支持移动语义

    std::vector<std::shared_ptr<Node<K, V>>> _forward; // 用于实现搜索跳表的forward指针，指向各层的下一个节点
};