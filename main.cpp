#include "test.h"

int main()
{
    Skiplist<int, std::string> sl(16);
    while (1)
    {
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

        case 4:
            Display(sl);
            std::cout << "**********************" << std::endl;
            system("pause");
            system("cls");
            break;

        case 5:
            InsertSet(sl);
            std::cout << "**********************" << std::endl;
            system("pause");
            system("cls");
            break;
        }
    }

    std::cout << "Test over, exiting..." << std::endl;
    return 0;
}
