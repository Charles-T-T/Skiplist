#pragma once 

#include <iostream>

// 跳表节点
template <typename kType, typename vType>
class Node
{
private:
    kType key;
    vType value;

public:
    Node() {}
    Node(kType k, vType v, int);
    ~Node();
    int nodeLevel;

    kType GetKey() const;
    vType GetValue() const;
    void SetValue(vType);

    Node<kType, vType> **forward; // 用二维指针实现跳表的搜索
};


template <typename kType, typename vType>
Node<kType, vType>::Node(const kType k, const vType v, int level)
{
    this->key = k;
    this->value = v;
    this->nodeLevel = level;
    this->forward = new Node<kType, vType> *[level + 1];                  // level从0算起
    memset(this->forward, 0, sizeof(Node<kType, vType> *) * (level + 1)); // 将所有指针初始化
}

template <typename kType, typename vType>
Node<kType, vType>::~Node()
{
    delete[] forward;
}

template <typename kType, typename vType>
kType Node<kType, vType>::GetKey() const
{
    return key;
}

template <typename kType, typename vType>
vType Node<kType, vType>::GetValue() const
{
    return value;
}

template <typename kType, typename vType>
void Node<kType, vType>::SetValue(vType v)
{
    this->value = v;
}

