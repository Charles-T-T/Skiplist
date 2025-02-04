#pragma once

#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "skiplist.h"

int LegalChoice(std::string choice, int low, int high);
int MenuChoose();
void clearScreen();
void pauseScreen();

template <typename K, typename V>
void Insert(Skiplist<K, V> &sl);

template <typename K, typename V>
void Delete(Skiplist<K, V> &sl);

template <typename K, typename V>
void Search(Skiplist<K, V> &sl);

template <typename K, typename V>
void Display(Skiplist<K, V> &sl);

template <typename K, typename V>
void InsertRandomSet(Skiplist<K, V> &sl);

template <typename K, typename V>
void DumpFile(Skiplist<K, V> &sl);

template <typename K, typename V>
void LoadFile(Skiplist<K, V> &sl);

// 检测输入值是否为整数
inline bool IsInt(std::string input) {
  if (input.size() > 2) return false;

  for (char const &i : input)
    if (!isdigit(i)) {
      return false;
    }
  return true;
}

// 检查输入是否合法
inline int LegalChoice(std::string choice, int low, int high) {
  while (!IsInt(choice) || stoi(choice) < low || stoi(choice) > high) {
    printf("illegal input, please input an integer from %d to %d: ", low, high);
    getline(std::cin, choice);
  }
  return stoi(choice);
}

// 清屏
inline void clearScreen() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

// 暂停
inline void pauseScreen() {
#ifdef _WIN32
  system("pause");
#else
  std::cout << "Press Enter to continue..." << std::endl;
  std::cin.get();
#endif
}

inline int MenuChoose() {
  clearScreen();
  std::cout << "-----------------------------" << std::endl;
  std::cout << "            MENU             " << std::endl;
  std::cout << "-----------------------------" << std::endl;
  std::cout << "- ATTENTION: " << std::endl;
  std::cout << "- this demo supposes" << std::endl;
  std::cout << "- 'int' as key " << std::endl;
  std::cout << "- 'string' as value" << std::endl;
  std::cout << "-----------------------------" << std::endl;
  std::cout << "1. Insert" << std::endl;
  std::cout << "2. Delete" << std::endl;
  std::cout << "3. Search" << std::endl;
  std::cout << "4. Display" << std::endl;
  std::cout << "5. Insert random KV set" << std::endl;
  std::cout << "6. Dump file" << std::endl;
  std::cout << "7. Load file" << std::endl;
  std::cout << "0. Exit" << std::endl;
  std::cout << "-----------------------------" << std::endl;
  std::cout << "Please choose a function(0-7): ";

  std::string choice;
  getline(std::cin, choice);
  return LegalChoice(choice, 0, 7);
}

template <typename K, typename V>
void Insert(Skiplist<K, V> &sl) {
  int key;
  std::string value, input;
  std::cout << "please input key and value, divided by a space: ";
  getline(std::cin, input);
  std::istringstream iss(input);
  if (!(iss >> key)) {
    std::cerr << "Invalid key input!" << std::endl;
    return;
  }
  if (!(iss >> value)) {
    std::cerr << "Invalid value input!" << std::endl;
    return;
  }

  if (sl.InsertNode(key, value))
    std::cout << "Insert success!" << std::endl;
  else
    std::cout << "The key has existed, value updating success!" << std::endl;
}

template <typename K, typename V>
void Delete(Skiplist<K, V> &sl) {
  std::string input;
  std::cout << "please input the key to be deleted: ";
  getline(std::cin, input);
  sl.DeleteNode(stoi(input));
  std::cout << "Delete Success!" << std::endl;
}

template <typename K, typename V>
void Search(Skiplist<K, V> &sl) {
  std::string input;
  std::cout << "please input the key to be searched: ";
  getline(std::cin, input);
  if (sl.SearchNode(stoi(input)))

    std::cout << "Search Success!" << std::endl;
  else
    std::cout << "Search Fail!" << std::endl;
}

template <typename K, typename V>
void Display(Skiplist<K, V> &sl) {
  sl.DisplayList();
}

template <typename K, typename V>
void InsertRandomSet(Skiplist<K, V> &sl) {
  // 随机生成50对kv
  std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<int> disInt(0, 10000);    // 随机生成int
  std::uniform_int_distribution<char> disChar('a', 'z');  // 随机生成字符

  for (int i = 0; i < 50; ++i) {
    // 随机生成kv
    int rKey = disInt(rng);
    std::string rVal;
    for (int j = 0; j < 5; ++j) {
      rVal += disChar(rng);
    }
    sl.InsertNode(rKey, rVal);
    printf("insert node[%d]\n", i);
  }
  // for (int i = 0; i < 50; i++) {
  //   std::cout << "insert node[" << i << "]" << std::endl;
  //   std::string str = "msg" + std::to_string(i);
  //   sl.InsertNode(i, str);
  // }
  std::cout << "Insert over!" << std::endl;
}

template <typename K, typename V>
void DumpFile(Skiplist<K, V> &sl) {
  sl.DumpFile();
}

template <typename K, typename V>
void LoadFile(Skiplist<K, V> &sl) {
  sl.LoadFile();
}