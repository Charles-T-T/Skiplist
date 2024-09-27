#include "menu.h"

#include "skiplist.h"

int main() {
  Skiplist<int, std::string> skiplist(16);
  skiplist.SetStressTest(0);
  while (1) {
    int choice = MenuChoose();
    switch (choice) {
      case 0:
        std::cout << "See you next time~" << std::endl;
        return 0;
        break;

      case 1:
        Insert(skiplist);
        std::cout << "*****************************" << std::endl;
        system("pause");
        system("cls");
        break;

      case 2:
        Delete(skiplist);
        std::cout << "*****************************" << std::endl;
        system("pause");
        system("cls");
        break;

      case 3:
        Search(skiplist);
        std::cout << "*****************************" << std::endl;
        system("pause");
        system("cls");
        break;

      case 4:
        Display(skiplist);
        std::cout << "*****************************" << std::endl;
        system("pause");
        system("cls");
        break;

      case 5:
        InsertSet(skiplist);
        std::cout << "*****************************" << std::endl;
        system("pause");
        system("cls");
        break;

      case 6:
        DumpFile(skiplist);
        std::cout << "*****************************" << std::endl;
        system("pause");
        system("cls");
        break;

      case 7:
        LoadFile(skiplist);
        std::cout << "*****************************" << std::endl;
        system("pause");
        system("cls");
        break;
    }
  }

  std::cout << "Test over, exiting..." << std::endl;
  return 0;
}
