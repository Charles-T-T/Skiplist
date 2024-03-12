#include <iostream>

template <typename keyType, typename valueType>
class Node
{
private:
    keyType k;
    valueType v;

public:
    Node() {}
    Node(keyType k, valueType v, int);
    ~Node();
    keyType getKey() const;
    valueType getValue() const;
    void setValue(valueType);
    Node<keyType, valueType> **forward;
    int nodeLevel;
};

template <typename keyType, typename valueType>
Node<keyType, valueType>::Node(const keyType k, const valueType v, int level)
{
}

template <typename keyType, typename valueType>
Node<keyType, valueType>::~Node()
{
}
