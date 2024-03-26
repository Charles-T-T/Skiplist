#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <string>
#include <stdio.h>

// 跳表节点的实现
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

    Node<kType, vType> **forward; // 用二维指针实现跳表的搜索
};

// Node成员函数定义
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

// 跳表结构的实现
template <typename kType, typename vType>
class Skiplist
{
public:
    Skiplist(int);                                    // 构造函数
    ~Skiplist();                                      // 析构函数
    Node<kType, vType> *CreatNode(kType, vType, int); // 创建新节点
    int GetRandLevel();                               // 获取新节点的层级（随机）
    int InsertNode(kType, vType);                     // 插入新节点
    void ShowList();                                  // 展示当前跳表情况
    bool SearchNode(kType);                           // 查找节点
    void DeleteNode(kType);                           // 删除节点
    void DumpFile();                                  // 数据持久化到文件
    void LoadFile();                                  // 从文件读取数据
    void Clear(Node<kType, vType> *);                 // 递归地清除节点
    int CountNode();                                  // 当前跳表节点个数

private:
    int _maxLevel;               // 跳表允许增长到底的最大层数
    int _listLevel;              // 跳表当前具有的层数
    int _elemCount;              // 跳表中所有元素的数量
    Node<kType, vType> *_header; // 跳表头节点
    std::ofstream _fileWriter;   // 文件写入器
    std::ifstream _fileReader;   // 文件读取器
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
    } // 采用随机数确定层级，节点够多时高效

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
        // 从顶层往下查
        while (current->forward[i] && current->forward[i]->getKey() < key) // 层内逐个查直到下一个就大于key
            current = current->forward[i];
        // TODO 感觉可以去掉？
        current = current->forward[0]; // 在底层（最全）查找下一个值
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
     * 采用指针数组update存储待更新的节点
     * 也就是要插入节点的前驱节点
     * 即每层中小于待插入节点的最后一个节点
     */
    Node<kType, vType> *update[_maxLevel + 1];                         // TODO 考虑改用_listLevel?
    memset(update, 0, sizeof(Node<kType, vType> *) * (_maxLevel + 1)); // 为update[]分配空间、初始化
    for (int i = _listLevel; i >= 0; i--)
    {
        while (current->forward[i] && current->forward[i]->getKey() < key)
            current = current->forward[i];
        update[i] = current;
    }

    /**
     * 检查key是否已经存在
     * 若存在：更新其value，返回0
     * 若不存在：插入新节点，返回1
     */
    current = current->forward[0];
    if (current && current->getKey() == key)
    {
        // TODO 原代码没有更新
        current->setValue(value);
        return 0;
    }

    // TODO 删除原代码中似乎没有意义的if判断
    int randLevel = GetRandLevel();
    // 如果得到的新随机层数 > 跳表现有层数，需要给update中新增加的层添加头节点
    if (randLevel > _listLevel)
    {
        for (int i = _listLevel + 1; i <= randLevel; i++)
            update[i] = _header;
        _listLevel = randLevel;
    }

    // 从底层到randLevel，在各层插入新节点
    Node<kType, vType>* newNode = CreatNode(key, value, randLevel);
    for (int i = 0; i <= randLevel; i++)
    {
        newNode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = newNode;
    }

    _elemCount++;
    return 1;
}