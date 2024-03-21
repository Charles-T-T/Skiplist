#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <fstream>

// 跳表节点的实现
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

    Node<keyType, valueType> **forward; // 用二维指针实现跳表的搜索
};

// Node成员函数定义
template <typename keyType, typename valueType>
Node<keyType, valueType>::Node(const keyType k, const valueType v, int level)
{
    this->key = k;
    this->value = v;
    this->nodeLevel = level;
    this->forward = new Node<keyType, valueType> *[level + 1];                  // level从0算起
    memset(this->forward, 0, sizeof(Node<keyType, valueType> *) * (level + 1)); // 将所有指针初始化
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

// 跳表结构的实现
template <typename keyType, typename valueType>
class Skiplist
{
public:
    Skiplist(int); // 构造函数
    ~Skiplist(); // 析构函数
    Node<keyType, valueType> *CreatNode(keyType, valueType, int); // 创建新节点
    int GetRandLevel(); // 获取新节点的层级（随机）
    int InsertNode(keyType, valueType); // 插入新节点
    void ShowList(); // 展示当前跳表情况
    bool SearchNode(keyType); // 查找节点
    void DeleteNode(keyType); // 删除节点
    void DumpFile(); // 数据持久化到文件
    void LoadFile(); // 从文件读取数据
    void Clear(Node<keyType, valueType> *); // 递归地清除节点
    int CountNode(); // 当前跳表节点个数

private:
    int _maxLevel; // 跳表最大层数
    int _curLevel; // 跳表当前的层数
    int _elemCount; // 跳表中所有节点数量
    Node<keyType, valueType> *_header; // 跳表头节点
    std::ofstream _fileWriter; // 文件写入器
    std::ifstream _fileReader; // 文件读取器
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
