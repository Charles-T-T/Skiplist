#include "skiplist.h"
#include <chrono>
#include <thread>
#include <vector>
#include <random>
#include <iostream>

constexpr int THREAD_COUNT = 4;     // 支持的线程数量
constexpr int TEST_COUNT = 1000000; // 测试用例的数量
constexpr int MAX_LEVEL = 20;       // 跳表可支持的最大层数
constexpr int MULTI_INSERT = 0;     // 是否采用多线程插入跳表

Skiplist<int, std::string> sl(MAX_LEVEL); // 创建<int, string>跳表

int GetRandNum(std::mt19937 &gen)
{
    std::uniform_int_distribution<int> dis(0, TEST_COUNT - 1);
    return dis(gen);
}

// 执行插入节点的线程函数
void InsertNodeFunc(int tid)
{
    unsigned seed = std::chrono::high_resolution_clock::now().time_since_epoch().count(); // 初始化随机数种子
    std::mt19937 gen(seed);

    int nodeCount = TEST_COUNT / THREAD_COUNT; // 每个线程需要插入的节点数量
    int start = nodeCount * tid;
    int end = nodeCount * (tid + 1);
    for (int i = start; i < end; i++)
    {
        sl.InsertNode(GetRandNum(gen), "test" + std::to_string(GetRandNum(gen)));
    }
}

// 执行搜索节点的线程函数
void SearchNodeFunc(int tid)
{
    unsigned seed = std::chrono::high_resolution_clock::now().time_since_epoch().count(); // 初始化随机数种子
    std::mt19937 gen(seed);

    int nodeCount = TEST_COUNT / THREAD_COUNT;
    int start = nodeCount * tid;
    int end = nodeCount * (tid + 1);
    for (int i = start; i < end; i++)
    {
        sl.SearchNode(GetRandNum(gen));
    }
}

int main()
{
    std::chrono::duration<double> insertTime, searchTime; // 插入、搜索压测用时

    // 插入节点压力测试
    {
        /**
         * 测试发现多线程下，插入压测的IPS变化很小、甚至有所减小
         * 原因应该是对同一个跳表频繁操作导致锁对性能的限制较大
         * 故此处选择不采用多线程进行测试
         */
        std::cout << "start testing node insertion..." << std::endl;
        auto start = std::chrono::high_resolution_clock::now();                               // 开始计时
        unsigned seed = std::chrono::high_resolution_clock::now().time_since_epoch().count(); // 初始化随机数种子
        std::mt19937 gen(seed);

        if (MULTI_INSERT)
        {
            // 创建插入节点的线程
            std::vector<std::thread> threads;
            for (int i = 0; i < THREAD_COUNT; i++)
            {
                std::cout << "main()--creating thread[" << i << "]" << std::endl;
                threads.emplace_back(InsertNodeFunc, i);
            }

            // 等待各个线程结束
            for (auto &t : threads)
            {
                if (t.joinable())
                    t.join();
            }
        }
        else // 单线程插入
        {
            for (int i = 0; i < TEST_COUNT; i++)
            {
                sl.InsertNode(GetRandNum(gen), "test" + std::to_string(GetRandNum(gen)));
            }
        }

        auto finish = std::chrono::high_resolution_clock::now(); // 结束计时
        insertTime = finish - start;                             // 计算用时
    }

    // 搜索节点压力测试
    {
        std::vector<std::thread> threads;
        std::cout << "\nstart testing node search..." << std::endl;
        auto start = std::chrono::high_resolution_clock::now(); // 开始计时

        // 创建搜索节点的线程
        for (int i = 0; i < THREAD_COUNT; i++)
        {
            std::cout << "main()--creating thread[" << i << "]" << std::endl;
            threads.emplace_back(SearchNodeFunc, i);
        }

        // 等待各个线程结束
        for (auto &t : threads)
        {
            if (t.joinable())
                t.join();
        }

        auto finish = std::chrono::high_resolution_clock::now(); // 结束计时
        searchTime = finish - start;                             // 计算用时
    }

    // 打印压测结果
    std::cout << "\n---------------RESULT---------------" << std::endl;
    std::cout << "Number of test cases: " << TEST_COUNT << std::endl;
    std::cout << "Elements in Skiplist now: " << sl.CountNode() << std::endl;
    std::cout << "Insertion test elapsed: " << insertTime.count() << " s" << std::endl;
    std::cout << "Search test elapsed: " << searchTime.count() << " s" << std::endl;
    sl.DumpFile();

    std::cout << "Finish dumping file" << std::endl;

    return 0;
}
