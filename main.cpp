#include "test.h"

int main()
{
    Skiplist<int, std::string> sl(16);
    while (1)
    {
        // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 忽略输入缓冲区中的所有字符，直到换行符为止

        int choice = MenuChoose();
        switch (choice)
        {
        case 0:
            std::cout << "See you next time~" << std::endl;
            return 0;
            break;

        case 1:
            Insert(sl);
            std::cout << "**********************" << std::endl;
            system("pause");
            system("cls");
            break;

        case 2:
            Delete(sl);
            std::cout << "**********************" << std::endl;
            system("pause");
            system("cls");
            break;

        case 3:
            Search(sl);
            std::cout << "**********************" << std::endl;
            system("pause");
            system("cls");
            break;
        }
    }

    std::cout << "Test over, exiting..." << std::endl;
    return 0;
}
