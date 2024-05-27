#pragma once

#include "node.h"
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <sstream>
#include <random>
#include <mutex>

#define IS_STRESS_TEST 1


template <typename kType, typename vType>
class Skiplist
{
public:
    Skiplist(int); // 构造函数
    // ~Skiplist();                                      // 析构函数
    Node<kType, vType> *CreatNode(kType, vType, int); // 创建新节点
    int GetRandLevel();                               // 获取新节点的层级（随机）
    int InsertNode(kType, vType);                     // 插入新节点
    void DisplayList();                               // 展示当前跳表情况
    bool SearchNode(kType);                           // 查找节点
    void DeleteNode(kType);                           // 删除节点
    void DumpFile();                                  // 数据持久化到文件
    void LoadFile();                                  // 从文件读取数据
    // void Clear(Node<kType, vType> *);                 // 递归地清除节点
    // int CountNode();                                  // 当前跳表节点个数
    std::string writePath;                            // 写入文件地址
    std::string readPath;                             // 载入文件地址

    bool IsValidKV(const std::string &str);                                          // 检验一组kv字符串是否合法
    void GetKVfromStr(const std::string &str, std::string *key, std::string *value); // 从字符串中解析出key和value

private:
    int _maxLevel;               // 跳表允许增长到底的最大层数
    int _listLevel;              // 跳表当前具有的层数
    int _elemCount;              // 跳表中所有元素的数量
    Node<kType, vType> *_header; // 跳表头节点
    std::ofstream _fileWriter;   // 文件写入器
    std::ifstream _fileReader;   // 文件读取器
    std::mutex _mutex;
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

    // 利用随机数决定新节点的层级，保证了节点被分配到越高层的概率越小
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10);
    while (dis(gen) % 2)
        level++;

    return std::min(level, _maxLevel);
}

template <typename kType, typename vType>
Node<kType, vType> *Skiplist<kType, vType>::CreatNode(const kType k, const vType v, int rdLevel)
{
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
        while (current->forward[i] && current->forward[i]->GetKey() < key) // 层内逐个查直到下一个节点的键就不小于key
            current = current->forward[i];
    }
    current = current->forward[0]; // 在底层查找下一个值（目标）
    if (current && current->GetKey() == key)
    {
        if (!IS_STRESS_TEST)
            std::cout << "Its value: " << current->GetValue() << std::endl;
        return true;
    }
    else
    {
        std::cout << "Oops, this key does not exit!" << std::endl;
        std::cout << "Oops, this key does not exit!" << std::endl;
        return false;
    }
}

template <typename kType, typename vType>
int Skiplist<kType, vType>::InsertNode(kType key, vType value)
{
    std::lock_guard<std::mutex> lock(_mutex); // 使用互斥锁保护共享资源

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
        while (current->forward[i] && current->forward[i]->GetKey() < key)
            current = current->forward[i];
        update[i] = current;
    }

    /**
     * 检查key是否已经存在
     * 若存在：更新其value，返回0
     * 若不存在：插入新节点，返回1
     */
    current = current->forward[0];
    if (current && current->GetKey() == key)
    {
        // TODO 原代码没有更新
        current->SetValue(value);
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
    Node<kType, vType> *newNode = CreatNode(key, value, randLevel);
    for (int i = 0; i <= randLevel; i++)
    {
        newNode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = newNode;
    }

    _elemCount++;
    return 1;
}

template <typename kType, typename vType>
void Skiplist<kType, vType>::DeleteNode(kType key)
{
    Node<kType, vType> *current = this->_header;
    Node<kType, vType> *update[_maxLevel + 1];
    memset(update, 0, sizeof(Node<kType, vType> *) * (_maxLevel + 1));

    // 从高层向下搜索每层待删除的节点
    for (int i = _listLevel; i >= 0; i--)
    {
        while (current->forward[i] && current->forward[i]->GetKey() < key)
        {
            current = current->forward[i];
        }
        update[i] = current; // 记录待删除节点的前驱
    }

    current = current->forward[0];
    if (current && current->GetKey() == key)
    {
        // 从高层向下逐层删除节点
        for (int i = _listLevel; i >= 0; i--)
        {
            // TODO 此处if判断是原代码的，感觉没用？将break改为了continue方可继续运行
            if (update[i]->forward[i] != current)
                continue;
            update[i]->forward[i] = current->forward[i];
        }

        // 调整跳表的当前高度
        while (_listLevel > 0 && _header->forward[_listLevel] == NULL)
            _listLevel--;

        // 释放内存
        delete current;
        _elemCount--;
        // TODO 考虑update的内存也释放？
    }
}

template <typename kType, typename vType>
void Skiplist<kType, vType>::DisplayList()
{
    // 从顶层向下逐层展示kv
    std::cout << "------------------------------" << std::endl;
    for (int i = _listLevel; i >= 0; i--)
    {
        Node<kType, vType> *curNode = _header->forward[i];
        printf("Level[%d]: ", i);
        while (curNode)

        {
            std::cout << "<" << curNode->GetKey() << ":" << curNode->GetValue() << "> ";
            curNode = curNode->forward[i];
        }
        std::cout << std::endl;
    }
    std::cout << "------------------------------" << std::endl;
}

template <typename kType, typename vType>
void Skiplist<kType, vType>::DumpFile()
{
    _fileWriter.open(writePath);
    while (!_fileWriter.is_open())
    {
        std::cout << "Invalid file path, please input another one: ";
        std::getline(std::cin, writePath);
        _fileWriter.open(writePath);
    }

    // 遍历底层节点并写入文件
    Node<kType, vType> *node = _header->forward[0];
    while (node)
    {
        _fileWriter << node->GetKey() << ":" << node->GetValue() << "\n";
        node = node->forward[0];
    }
    _fileWriter.flush(); // 刷新缓冲区确保数据全部写入
    _fileWriter.close(); // 关闭文件
}

template <typename kType, typename vType>
bool Skiplist<kType, vType>::IsValidKV(const std::string &str)
{
    return !str.empty() && str.find(':') != std::string::npos;
}

template <typename kType, typename vType>
void Skiplist<kType, vType>::GetKVfromStr(const std::string &str, std::string *key, std::string *value)
{
    if (!IsValidKV(str))
        return;

    *key = str.substr(0, str.find(':'));
    *value = str.substr(str.find(':') + 1);
}

template <typename kType, typename vType>
void Skiplist<kType, vType>::LoadFile()
{
    _fileReader.open(readPath);
    while (!_fileReader.is_open())
    {
        std::cout << "Invalid file path, please input another one: ";
        getline(std::cin, readPath);
        _fileReader.open(readPath);
    }

    std::cout << "start loading file..." << std::endl;
    std::string line;
    std::string *key = new std::string();
    std::string *value = new std::string();
    while (getline(_fileReader, line))
    {
        GetKVfromStr(line, key, value);
        if (key->empty() || value->empty())
            continue;
        this->InsertNode(stoi(*key), *value);
    }

    delete key;
    delete value;
    _fileReader.close();
    std::cout << "File loaded successfully!" << std::endl;
}
