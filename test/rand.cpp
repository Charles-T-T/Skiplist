#include <iostream>
#include <random>

using namespace std;

void GetRand()
{
    int level = 0;
    while (rand() % 2)
    {
        level++;
    }
    cout << "Level: " << level << endl;
}

void GetRandII()
{
    int level = 0;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10);

    while (dis(gen) % 2)
        level++;
    cout << "Levcel: " << level << endl;
}

int main()
{
    for (int i = 0; i < 20; i++)
    {
        GetRand();
    }

    cout << "-----------------------" << endl;

    for (int i = 0; i < 20; i++)
    {
        GetRandII();
    }

    cout << "-------------------------" << endl;
    int num = 4;
    string str = "msg" + to_string(num);
    cout << str << endl;
    system("pause");
    return 0;
}