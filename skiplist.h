#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <fstream>

// ����ڵ��ʵ��
template <typename keyType, typename valueType>
class Node
{
private:
    keyType key;
    valueType value;

public:
    Node() {}
    Node(keyType k, valueType v, int);
    ~Node();
    int nodeLevel;

    keyType getKey() const;
    valueType getValue() const;
    void setValue(valueType);

    Node<keyType, valueType> **forward; // �ö�άָ��ʵ�����������
};

// Node��Ա��������
template <typename keyType, typename valueType>
Node<keyType, valueType>::Node(const keyType k, const valueType v, int level)
{
    this->key = k;
    this->value = v;
    this->nodeLevel = level;
    this->forward = new Node<keyType, valueType> *[level + 1];                  // level��0����
    memset(this->forward, 0, sizeof(Node<keyType, valueType> *) * (level + 1)); // ������ָ���ʼ��
};

template <typename keyType, typename valueType>
Node<keyType, valueType>::~Node()
{
    delete[] forward;
};

template <typename keyType, typename valueType>
keyType Node<keyType, valueType>::getKey() const{
    return key;
};

template <typename keyType, typename valueType>
valueType Node<keyType, valueType>::getValue() const{
    return value;
};

template <typename keyType, typename valueType>
void Node<keyType, valueType>:: setValue(valueType v){
    this->value = v;
};

// ����ṹ��ʵ��
template <typename keyType, typename valueType>
class Skiplist
{
public:
    Skiplist(int); // ���캯��
    ~Skiplist(); // ��������
    Node<keyType, valueType> *CreatNode(keyType, valueType, int); // �����½ڵ�
    int GetRandLevel(); // ��ȡ�½ڵ�Ĳ㼶�������
    int InsertNode(keyType, valueType); // �����½ڵ�
    void ShowList(); // չʾ��ǰ�������
    bool SearchNode(keyType); // ���ҽڵ�
    void DeleteNode(keyType); // ɾ���ڵ�
    void DumpFile(); // ���ݳ־û����ļ�
    void LoadFile(); // ���ļ���ȡ����
    void Clear(Node<keyType, valueType> *); // �ݹ������ڵ�
    int CountNode(); // ��ǰ����ڵ����

private:
    int _maxLevel; // ����������
    int _curLevel; // ����ǰ�Ĳ���
    int _elemCount; // ���������нڵ�����
    Node<keyType, valueType> *_header; // ����ͷ�ڵ�
    std::ofstream _fileWriter; // �ļ�д����
    std::ifstream _fileReader; // �ļ���ȡ��
};

template <typename keyType, typename valueType>
Skiplist<keyType, valueType>::Skiplist(int maxLevel){
    this->_maxLevel = maxLevel;
    this->_curLevel = 0;
    this->_elemCount = 0;
    keyType k;
    valueType v;
    this->_header = new Node<keyType, valueType>(k, v, maxLevel);
}

template <typename keyType, typename valueType>
int Skiplist<keyType, valueType>::GetRandLevel()
{
    int level = 0;
    while (rand() % 2)
    {
        level++;
    }

    return (level <= _maxLevel) ? level : _maxLevel;
};

template <typename keyType, typename valueType>
int Skiplist<keyType, valueType>::InsertNode(keyType k, valueType v){

};
