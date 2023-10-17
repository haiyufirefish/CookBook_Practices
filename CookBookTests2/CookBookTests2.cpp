// CookBookTests2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <format>
#include <span>
#include <string_view>

template <typename... Args>
void print1(const std::string_view fmt_str, Args&&... args)
{
  auto fmt_args{ std::make_format_args(args...) };
  std::string outstr{ std::vformat(fmt_str,fmt_args) };
  fputs(outstr.c_str(), stdout);
}

/**************************************2.2******************************************************************/
template<class T>
void psspan(std::span<T> s)
{
  print1("numbers of elements: {}\n", s.size());
  print1("size of span: {}\n", s.size_bytes());
  for (auto i : s)
    print1("{}", i);
  endl(std::cout);
}

/**************************************2.3******************************************************************/
#include<map>
#include<tuple>

struct X
{
  int a;
  double b;
  std::string str;
};

auto func3()->std::tuple<int, int>
{
  return { 1,2 };
}

/**************************************2.4******************************************************************/
#include<mutex>
#include<thread>

std::mutex m;
bool flag = true;

void func4(int n)
{
  // the mutex is locked in the if statement
  if (std::lock_guard lg{ m }; flag)
  {
    print1("hha {}'\t",n);
    print1("oh no '\n");
  }
}

void t41()
{
  if (auto flag = [](int n) {return n * n; }(10); flag != 0)
  {
    print1("horse horse \n");
  }
}

void t42()
{
  switch (char c = getchar(); c)
  {
  case 'a':
    print1("a\n");
    break;
  case 'b':
    print1("b\n");
    break;
  case 'c':
    print1("c\n");
    break;
  case 'd':
    print1("d\n");
    break;
  default:
    print1("nothing\n");
    break;
  }
}

/**************************************2.5******************************************************************/
using namespace std::string_literals;

// template parameter deduction
template<class T>
struct X5
{
  T v{};
  template<class...Args>
  X5(Args&&...args) :v{ (args + ...) } {}
};

template<class...Ts>
X5(Ts...ts)->X5<std::common_type_t<Ts... >> ;

/**************************************2.6******************************************************************/
// compile time if
template<class T>
auto f6(const T& v)
{
  if constexpr (std::is_pointer_v<T>)
    print1("pointer\n");
  else
    print1("no pointer\n");
}

template<class T, class...Args>
void show(T t, Args&&...args)
{
  print1("{}\t", t);
  if constexpr (sizeof...(args))
  {
    show(args...);
  }
}


int main()
{
  // 2.2
  //int array[]{ 1,2,3,4,5 };
  //psspan<int>(array);

  // 2.3 total array assigend concret values
  //int array[]{ 1,2,3,4,5 };
  //auto& [a, b, c, d, e] = array;
  //print1("{} {} {} {} {}\n", a, b, c, d, e);
  //a = 10;
  //print1("{}\n", array[0]);

  //std::tuple<int, double, std::string>tu{ 10,3.14,"fff" };
  //auto [t1, t2, t3] = tu;
  //print1("{} {} {}\n", t1, t2, t3);

  //auto [f1, f2] = func3();
  //print1("{} {}\n", f1,f2);

  //std::map<int, std::string>mp{ {1,"**"},{2,"%%%"} };
  //for (const auto& [k, v] : mp)
  //{
  //  print1("{} {} \n", k, v);
  //}
  
  // 2.4
  //for (int i = 0; i < 10; ++i)
  //{
  //  std::jthread th1{ func4,i*2 };
  //  std::jthread th2{ func4,i*3 };
  //}
  //t41();
  //t42();
   
  // 2.5 here must add s at the end, eles error
  //X5 x5("10", "55"s);
  //print1("{}\n", x5.v);


  // 2.6
  int* p{};
  f6(p);
  delete p;
  f6(1);
  show(5.311, "hha", '*');
  print1("\n");

  return 0;
}

