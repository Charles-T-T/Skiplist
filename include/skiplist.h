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
#include <vector>
#include <memory>

#define W_PATH "../res/stress_res.txt"

template <typename K, typename V>
class Skiplist
{
public:
    Skiplist(int maxLevel);
    std::shared_ptr<Node<K, V>> CreateNode(K key, V value, int level);
    int GetRandLevel();
    int InsertNode(K key, V value);
    void DisplayList();
    bool SearchNode(K key);
    void DeleteNode(K key);
    void DumpFile(); // 数据持久化到文件
    void LoadFile(); // 从文件读取数据
    int CountNode();
    void SetStressTest(int i) { STRESS_TEST = i; }

private:
    int _maxLevel;                       // 跳表运行增长到的最大层数
    int _listLevel;                      // 跳表当前具有的层数
    int _elemCount;                      // 跳表中所有元素的数量
    std::shared_ptr<Node<K, V>> _header; // 跳表头节点
    std::ofstream _fileWriter;           // 文件写入器
    std::ifstream _fileReader;           // 文件读取器
    std::mutex _mutex;
    int STRESS_TEST = 1;

    std::string writePath = W_PATH;
    std::string readPath;

    bool IsValidKV(const std::string &str);
    void GetKVfromStr(const std::string &str, K &key, V &value, int &valid);
};

template <typename K, typename V>
Skiplist<K, V>::Skiplist(int maxLevel)
    : _maxLevel(maxLevel), _listLevel(0), _elemCount(0),
      _header(std::make_shared<Node<K, V>>(K(), V(), maxLevel)) {}

template <typename K, typename V>
int Skiplist<K, V>::GetRandLevel()
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

template <typename K, typename V>
std::shared_ptr<Node<K, V>> Skiplist<K, V>::CreateNode(K key, V value, int level)
{
    return std::make_shared<Node<K, V>>(key, value, level);
}

template <typename K, typename V>
bool Skiplist<K, V>::SearchNode(K key)
{
    auto current = _header;
    for (int i = _listLevel; i >= 0; i--)
    {
        // 从顶层往下查
        while (current->_forward[i] && current->_forward[i]->GetKey() < key)
            current = current->_forward[i];
    }
    current = current->_forward[0]; // 在底层查找下一个值（目标）
    if (current && current->GetKey() == key)
    {
        if (!STRESS_TEST)
            std::cout << "Its value: " << current->GetValue() << std::endl;
        return true;
    }
    return false;
}

template <typename K, typename V>
int Skiplist<K, V>::InsertNode(K key, V value)
{
    std::lock_guard<std::mutex> lock(_mutex);
    auto current = _header;
    /**
     * 采用指针数组update存储待更新的节点
     * 也就是要插入节点的前驱节点
     * 即每层中小于待插入节点的最后一个节点
     */
    std::vector<std::shared_ptr<Node<K, V>>> update(_maxLevel + 1);
    std::fill(update.begin(), update.end(), nullptr);

    for (int i = _listLevel; i >= 0; i--)
    {
        while (current->_forward[i] && current->_forward[i]->GetKey() < key)
            current = current->_forward[i];
        update[i] = current;
    }

    /**
     * 检查key是否已经存在
     * 若存在：更新其value，返回0
     * 若不存在：插入新节点，返回1
     */
    current = current->_forward[0];
    if (current && current->GetKey() == key)
    {
        current->SetValue(value);
        return 0;
    }

    int randLevel = GetRandLevel();
    // 如果得到的新随机层数 > 跳表现有层数，需要给update中新增加的层添加头节点
    if (randLevel > _listLevel)
    {
        for (int i = _listLevel + 1; i <= randLevel; i++)
            update[i] = _header;
        _listLevel = randLevel;
    }

    // 从底层到randLevel，在各层插入新节点
    auto newNode = CreateNode(key, value, randLevel);
    for (int i = 0; i <= randLevel; i++)
    {
        newNode->_forward[i] = std::move(update[i]->_forward[i]);
        update[i]->_forward[i] = newNode;
    }

    _elemCount++;
    return 1;
}

template <typename K, typename V>
void Skiplist<K, V>::DeleteNode(K key)
{
    auto current = _header;
    std::vector<std::shared_ptr<Node<K, V>>> update(_maxLevel + 1);
    std::fill(update.begin(), update.end(), nullptr);
    // 从高层向下搜索每层待删除的节点
    for (int i = _listLevel; i >= 0; i--)
    {
        while (current->_forward[i] && current->_forward[i]->GetKey() < key)
            current = current->_forward[i];
        update[i] = current; // 记录待删除节点的前驱
    }

    current = current->_forward[0];
    if (current && current->GetKey() == key)
    {
        // 从高层向下逐层删除节点
        for (int i = _listLevel; i >= 0; i--)
        {
            if (update[i]->_forward[i] != current)
                continue;
            update[i]->_forward[i] = std::move(current->_forward[i]);
        }
        // 调整跳表的当前高度
        while (_listLevel > 0 && !_header->_forward[_listLevel])
            _listLevel--;

        _elemCount--;
    }
}

template <typename K, typename V>
void Skiplist<K, V>::DisplayList()
{
    // 从顶层向下逐层展示kv
    std::cout << "------------------------------" << std::endl;
    for (int i = _listLevel; i >= 0; i--)
    {
        auto curNode = _header->_forward[i];
        printf("Level[%d]: ", i);
        while (curNode)
        {
            std::cout << "<" << curNode->GetKey() << ":" << curNode->GetValue() << "> ";
            curNode = curNode->_forward[i];
        }
        std::cout << std::endl;
    }
    std::cout << "------------------------------" << std::endl;
}

template <typename K, typename V>
void Skiplist<K, V>::DumpFile()
{
    _fileWriter.open(writePath);
    while (!_fileWriter.is_open())
    {
        std::cout << "Invalid file path, please input another one: ";
        std::getline(std::cin, writePath);
        _fileWriter.open(writePath);
    }
    // 遍历底层节点并写入文件
    auto node = _header->_forward[0];
    while (node)
    {
        _fileWriter << node->GetKey() << ":" << node->GetValue() << "\n";
        node = node->_forward[0];
    }
    _fileWriter.flush(); // 刷新缓冲区确保数据全部写入
    _fileWriter.close(); // 关闭文件
}

template <typename K, typename V>
bool Skiplist<K, V>::IsValidKV(const std::string &str)
{
    return !str.empty() && str.find(':') != std::string::npos;
}

template <typename K, typename V>
void Skiplist<K, V>::GetKVfromStr(const std::string &str, K &key, V &value, int &valid)
{
    if (!IsValidKV(str))
        return;

    std::string strKey = str.substr(0, str.find(':'));
    std::string strValue = str.substr(str.find(':') + 1);
    if (strKey.empty() || strValue.empty())
    {
        valid = 0;
        return;
    }

    std::istringstream issKey(strKey);
    issKey >> key;
    std::istringstream issValue(strValue);
    issValue >> value;
}

template <typename K, typename V>
void Skiplist<K, V>::LoadFile()
{
    _fileReader.open(readPath);
    while (!_fileReader.is_open())
    {
        std::cout << "Invalid file path, please input another one: ";
        std::getline(std::cin, readPath);
        _fileReader.open(readPath);
    }

    std::cout << "start loading file..." << std::endl;
    std::string line;
    K key;
    V value;
    int valid = 1;
    while (getline(_fileReader, line))
    {
        GetKVfromStr(line, key, value, valid);
        if (!valid)
            continue;
        this->InsertNode(key, value);
    }

    _fileReader.close();
    std::cout << "File loaded successfully!" << std::endl;
}

template <typename K, typename V>
int Skiplist<K, V>::CountNode()
{
    return _elemCount;
}
