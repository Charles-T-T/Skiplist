#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <string>
#include <stdio.h>

// ����ڵ��ʵ��
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

    kType getKey() const;
    vType getValue() const;
    void setValue(vType);

    Node<kType, vType> **forward; // �ö�άָ��ʵ�����������
};

// Node��Ա��������
template <typename kType, typename vType>
Node<kType, vType>::Node(const kType k, const vType v, int level)
{
    this->key = k;
    this->value = v;
    this->nodeLevel = level;
    this->forward = new Node<kType, vType> *[level + 1];                  // level��0����
    memset(this->forward, 0, sizeof(Node<kType, vType> *) * (level + 1)); // ������ָ���ʼ��
}

template <typename kType, typename vType>
Node<kType, vType>::~Node()
{
    delete[] forward;
}

template <typename kType, typename vType>
kType Node<kType, vType>::getKey() const
{
    return key;
}

template <typename kType, typename vType>
vType Node<kType, vType>::getValue() const
{
    return value;
}

template <typename kType, typename vType>
void Node<kType, vType>::setValue(vType v)
{
    this->value = v;
}

// ����ṹ��ʵ��
template <typename kType, typename vType>
class Skiplist
{
public:
    Skiplist(int);                                    // ���캯��
    ~Skiplist();                                      // ��������
    Node<kType, vType> *CreatNode(kType, vType, int); // �����½ڵ�
    int GetRandLevel();                               // ��ȡ�½ڵ�Ĳ㼶�������
    int InsertNode(kType, vType);                     // �����½ڵ�
    void ShowList();                                  // չʾ��ǰ�������
    bool SearchNode(kType);                           // ���ҽڵ�
    void DeleteNode(kType);                           // ɾ���ڵ�
    void DumpFile();                                  // ���ݳ־û����ļ�
    void LoadFile();                                  // ���ļ���ȡ����
    void Clear(Node<kType, vType> *);                 // �ݹ������ڵ�
    int CountNode();                                  // ��ǰ����ڵ����

private:
    int _maxLevel;               // ���������������׵�������
    int _listLevel;              // ����ǰ���еĲ���
    int _elemCount;              // ����������Ԫ�ص�����
    Node<kType, vType> *_header; // ����ͷ�ڵ�
    std::ofstream _fileWriter;   // �ļ�д����
    std::ifstream _fileReader;   // �ļ���ȡ��
};

template <typename kType, typename vType>
Skiplist<kType, vType>::Skiplist(int maxLevel)
{
    this->_maxLevel = maxLevel;
    this->_listLevel = 0;
    this->_elemCount = 0;
    kType k;
    vType v;
    this->_header = new Node<kType, vType>(k, v, maxLevel);
}

template <typename kType, typename vType>
int Skiplist<kType, vType>::GetRandLevel()
{
    int level = 0;
    while (rand() % 2)
    {
        level++;
    } // ���������ȷ���㼶���ڵ㹻��ʱ��Ч

    return std::max(level, _maxLevel);
}

template <typename kType, typename vType>
Node<kType, vType>* Skiplist<kType, vType>::CreatNode(const kType k, const vType v, int rdLevel){
    Node<kType, vType> *n = new Node(k, v, rdLevel);
    return n;
}


template <typename kType, typename vType>
bool Skiplist<kType, vType>::SearchNode(kType key)
{
    Node<kType, vType> *current = _header;
    for (int i = _listLevel; i >= 0; i--)
    {
        // �Ӷ������²�
        while (current->forward[i] && current->forward[i]->getKey() < key) // ���������ֱ����һ���ʹ���key
            current = current->forward[i];
        // TODO �о�����ȥ����
        current = current->forward[0]; // �ڵײ㣨��ȫ��������һ��ֵ
        if (current && current->getKey() == key)
            return true;
        else
            return false;
    }
    return false;
}

template <typename kType, typename vType>
int Skiplist<kType, vType>::InsertNode(kType key, vType value)
{
    Node<kType, vType> *current = _header;

    /**
     * ����ָ������update�洢�����µĽڵ�
     * Ҳ����Ҫ����ڵ��ǰ���ڵ�
     * ��ÿ����С�ڴ�����ڵ�����һ���ڵ�
     */
    Node<kType, vType> *update[_maxLevel + 1];                         // TODO ���Ǹ���_listLevel?
    memset(update, 0, sizeof(Node<kType, vType> *) * (_maxLevel + 1)); // Ϊupdate[]����ռ䡢��ʼ��
    for (int i = _listLevel; i >= 0; i--)
    {
        while (current->forward[i] && current->forward[i]->getKey() < key)
            current = current->forward[i];
        update[i] = current;
    }

    /**
     * ���key�Ƿ��Ѿ�����
     * �����ڣ�������value������0
     * �������ڣ������½ڵ㣬����1
     */
    current = current->forward[0];
    if (current && current->getKey() == key)
    {
        // TODO ԭ����û�и���
        current->setValue(value);
        return 0;
    }

    // TODO ɾ��ԭ�������ƺ�û�������if�ж�
    int randLevel = GetRandLevel();
    // ����õ������������ > �������в�������Ҫ��update�������ӵĲ����ͷ�ڵ�
    if (randLevel > _listLevel)
    {
        for (int i = _listLevel + 1; i <= randLevel; i++)
            update[i] = _header;
        _listLevel = randLevel;
    }

    // �ӵײ㵽randLevel���ڸ�������½ڵ�
    Node<kType, vType>* newNode = CreatNode(key, value, randLevel);
    for (int i = 0; i <= randLevel; i++)
    {
        newNode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = newNode;
    }

    _elemCount++;
    return 1;
}