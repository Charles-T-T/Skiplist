#include "../skiplist.h"
#include <chrono>
#include <pthread.h>
#include <time.h>
#include <cstdint>

#define THREAD_COUNT 3     // 支持的线程数量
#define TEST_COUNT 1000000 // 测试用例的数量

Skiplist<int, std::string> sl(18); // 创建最大可支持层数为18的跳表

// 执行插入节点的线程函数
void *InsertNodeFunc(void *threadID)
{
    intptr_t tid;                               // 线程ID
    tid = reinterpret_cast<intptr_t>(threadID); // TODO 改为c++风格的类型转换
    // printf("thread[%ld] starts inserting...\n", tid);
    int nodeCount = TEST_COUNT / THREAD_COUNT; // 每个线程需要插入的节点数量
    int start = nodeCount * tid;
    int end = nodeCount * (tid + 1);
    for (int i = start; i < end; i++)
    {
        sl.InsertNode(rand() % TEST_COUNT, "test" + std::to_string(i));
    }
    pthread_exit(NULL);
}

void *SearchNodeFunc(void *threadID)
{
    intptr_t tid;                               // 线程ID
    tid = reinterpret_cast<intptr_t>(threadID); // TODO 改为c++风格的类型转换
    // printf("thread[%ld] starts searching...\n", tid);
    int nodeCount = TEST_COUNT / THREAD_COUNT;
    int start = nodeCount * tid;
    int end = nodeCount * (tid + 1);
    for (int i = start; i < end; i++)
    {
        sl.SearchNode(rand() % TEST_COUNT);
    }
    pthread_exit(NULL);
}

int main()
{
    // TODO 改为更好的随机数
    srand(time(NULL));                                    // 初始化随机数种子
    std::chrono::duration<double> insertTime, searchTime; // 插入、搜索压测用时

    // 插入节点压力测试
    {
        pthread_t threads[THREAD_COUNT];
        int rc; // 用于接收pthread_create的返回值
        std::cout << "start testing node insertion..." << std::endl;
        auto start = std::chrono::high_resolution_clock::now(); // 开始计时

        // 创建插入节点的线程
        for (int i = 0; i < THREAD_COUNT; i++)
        {
            printf("main()--creating thread[%d]\n", i);
            rc = pthread_create(&threads[i], NULL, InsertNodeFunc, (void *)i);
            if (rc)
            {
                // TODO 考虑用perror等方法输出错误
                std::cout << "Error: fail to create thread, " << rc << std::endl;
                exit(-1); // 线程创建失败，退出测试
            }
        }

        void *rj; // 用于接收pthread_join的返回值

        // 等待各个线程结束
        for (int i = 0; i < THREAD_COUNT; i++)
        {
            if (pthread_join(threads[i], &rj))
            {
                perror("pthread_join() fail");
                exit(3); // 等待线程结束失败，退出测试
            }
        }

        auto finish = std::chrono::high_resolution_clock::now(); // 结束计时
        insertTime = finish - start;                             // 计算用时
    }

    // 搜索节点压力测试
    {
        pthread_t threads[THREAD_COUNT];
        int rc; // 用于接收pthread_create的返回值
        std::cout << "\nstart testing node insertion..." << std::endl;
        auto start = std::chrono::high_resolution_clock::now(); // 开始计时

        // 创建搜索节点的线程
        for (int i = 0; i < THREAD_COUNT; i++)
        {
            printf("main()--creating thread[%d]\n", i);
            rc = pthread_create(&threads[i], NULL, SearchNodeFunc, (void *)i);
            if (rc)
            {
                // TODO 考虑用perror等方法输出错误
                std::cout << "Error: fail to create thread, " << rc << std::endl;
                exit(-1); // 线程创建失败，退出测试
            }
        }

        void *rj; // 用于接收pthread_join的返回值

        // 等待各个线程结束
        for (int i = 0; i < THREAD_COUNT; i++)
        {
            if (pthread_join(threads[i], &rj))
            {
                perror("pthread_join() fail");
                exit(3); // 等待线程结束失败，退出测试
            }
        }

        auto finish = std::chrono::high_resolution_clock::now(); // 结束计时
        searchTime = finish - start;                             // 计算用时
    }

    // 打印压测结果
    std::cout << "\n---------------RESULT---------------" << std::endl;
    std::cout << "Number of test cases: " << TEST_COUNT << std::endl;
    std::cout << "Insertion test elapsed: " << insertTime.count() << " s" << std::endl;
    std::cout << "Search test elapsed: " << searchTime.count() << " s" << std::endl;
    pthread_exit(NULL);
    return 0;
}