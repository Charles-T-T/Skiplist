#include "skiplist.h"

int LegalChoice(std::string choice, int low, int high);
int MenuChoose();

template <typename kType, typename vType>
void Insert(Skiplist<kType, vType> &sl);

template <typename kType, typename vType>
void Delete(Skiplist<kType, vType> &sl);

template <typename kType, typename vType>
void Search(Skiplist<kType, vType> &sl);

template <typename kType, typename vType>
void Display(Skiplist<kType, vType> &sl);

template <typename kType, typename vType>
void InsertSet(Skiplist<kType, vType> &sl);

// 检测输入值是否为整数
bool IsInt(std::string input)
{
    if (input.size() > 2)
        return false;

    for (char const &i : input)
        if (!isdigit(i))
        {
            return false;
        }
    return true;
}
// 检查输入是否合法
int LegalChoice(std::string choice, int low, int high)
{
    while (!IsInt(choice) || stoi(choice) < low || stoi(choice) > high)
    {
        printf("illegal input, please input an integer from %d to %d: ", low, high);
        getline(std::cin, choice);
    }
    return stoi(choice);
}

int MenuChoose(){
    system("cls");

    std::cout << "---------------------" << std::endl;
    std::cout << "         MENU" << std::endl;
    std::cout << "key-int, value-string" << std::endl;
    std::cout << "---------------------" << std::endl;
    std::cout << "1. Insert" << std::endl;
    std::cout << "2. Delete" << std::endl;
    std::cout << "3. Search" << std::endl;
    std::cout << "4. Display" << std::endl;
    std::cout << "5. Insert set" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "---------------------" << std::endl;
    std::cout << "Please choose a function(0-5): ";

    std::string choice;
    getline(std::cin, choice);
    return LegalChoice(choice, 0, 5);
}

template <typename kType, typename vType>
void Insert(Skiplist<kType, vType> &sl)
{
    int key;
    std::string value, input;
    std::cout << "please input key and value, divided by a space: ";
    getline(std::cin, input);
    std::istringstream iss(input);
    if (!(iss >> key)){
        std::cerr << "Invalid key input!" << std::endl;
        return;
    }
    if (!(iss >> value)){
        std::cerr << "Invalid value input!" << std::endl;
        return;
    }

    if (sl.InsertNode(key, value))
        std::cout << "Insert success!" << std::endl;
    else
        std::cout << "The key has existed, value updating success!" << std::endl;
}

template <typename kType, typename vType>
void Delete(Skiplist<kType, vType> &sl)
{
    std::string input;
    std::cout << "please input the key to be deleted: ";
    getline(std::cin, input);
    sl.DeleteNode(stoi(input));
    std::cout << "Delete Success!" << std::endl;
}

template <typename kType, typename vType>
void Search(Skiplist<kType, vType> &sl)
{
    std::string input;
    std::cout << "please input the key to be searched: ";
    getline(std::cin, input);
    if (sl.SearchNode(stoi(input)))
        std::cout << "Search Success!" << std::endl;
    else
        std::cout << "Search Fail!" << std::endl;
}

template <typename kType, typename vType>
void Display(Skiplist<kType, vType> &sl){
    sl.DisplayList();
}

template <typename kType, typename vType>
void InsertSet(Skiplist<kType, vType> &sl){
    for (int i = 0; i < 50; i++)
    {
        std::cout << "insert node[" << i << "]" << std::endl;
        std::string str = "msg" + std::to_string(i);
        sl.InsertNode(i, str);
    }
    std::cout << "Insert over!" << std::endl;
}