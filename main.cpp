#include "skiplist.h"

template <typename kType, typename vType>
void InsertSearch(Skiplist<kType, vType>*);

int main(){
    Skiplist<int, std::string> *sl = new Skiplist<int, std::string>(16);

    enum Mood
    {
        def_node_list,
        insert_search
    } mood;

    mood = insert_search;

    switch (mood)
    {
    case insert_search:
        InsertSearch(sl);
        break;
    
    default:
        break;
    }

    std::cout << "Test over, exiting..." << std::endl;
    return 0;
}

template<typename kType, typename vType>
void InsertSearch(Skiplist<kType, vType>* sl){
    int keyNum, seachNum;
    std::cout << "please input the num of keys to be inserted: ";
    std::cin >> keyNum;
    std::cout << "please inputh the num of keys to be searched: ";
    std::cin >> seachNum;
    
    // insert
    for (int i = 0; i < keyNum; i++)
    {
        int key;
        std::string value;
        printf("k-v(%d): ", i);
        std::cin >> key >> value;
        if (sl->InsertNode(key, value))
            std::cout << "Insert successfully!" << std::endl;
        else
            std::cout << "Key existed, its value updated successfully!" << std::endl;
    }

    // search
    for (int i = 0; i < seachNum; i++)
    {
        int key;
        std::cout << "please input the key to be searched: ";
        std::cin >> key;
        if (sl->SearchNode(key))
            std::cout << "Search successfully!" << std::endl;
        else
            std::cout << "Search fail!" << std::endl;
    }

}