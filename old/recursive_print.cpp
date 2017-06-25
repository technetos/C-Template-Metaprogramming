#include<iostream>

// Base case of N = 1 argument
template<typename T>
  void print(const T & value)
{
  std::cout << value << "\n";
}

// Recursive case of N > 1 arguments
template<typename T, typename ... Ts>
  void print(const T & head, const Ts & ... tail)
{
  std::cout << head << "\n";
  print(tail...);
}

int main(void)
{
  print("a", "b", "c", 1, "d", "e", 1);
  return 0;
}
