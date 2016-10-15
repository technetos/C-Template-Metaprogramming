#include<iostream>

// Base case of N = 1 argument
template<typename T>
constexpr T sum(const T & value)
{
  return value;
}

// Recursive case of N > 1 arguments
template<typename T, typename ... Ts>
constexpr T sum(const T & head, const Ts & ... tail)
{
  return sum(head + sum(tail...));
}

int main(void)
{
  std::cout << sum(5, 5, 5, 5, 5, 5, 5, 5, 5) << "\n";
  return 0;
}
