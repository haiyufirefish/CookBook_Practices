// CookBookTests5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma once
#include<format>
#include<iostream>
#include<ranges>
#include<map>
#include<vector>
#include<filesystem>

#define Five_three 0
#define Five_four 0
#define Five_five 0
#define Five_six 0
#define Five_seven 0
#define Five_eight 0
#define Five_nine 1

template < typename... Args>
void print(const std::string_view fmt_str, Args&&... args) {
  auto fmt_args{ std::make_format_args(args...) };
  std::string outstr{ std::vformat(fmt_str, fmt_args) };
  fputs(outstr.c_str(), stdout);
}

namespace fs = std::filesystem;
template<>
struct std::formatter<fs::path> {
  template<typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
	return ctx.begin();
  }
  template<typename FormatContext>
  auto format(const fs::path& p, FormatContext& ctx) {
	return std::format_to(ctx.out(), "{}", p.string());
  }
};

void print(std::ranges::range auto v) {
  print("size: {}  ", v.size());
  print("[ ");
  for (const auto& i : v)print("{} ", i);
  print("]\n");
}

template<class T, class T2>
void print(const std::map<T, T2>& map) {
  print("size: {} ", map.size());
  print("[ ");
  for (auto& [k, v] : map)print("{}:{} ", k, v);
  print("]\n");
}

template<class T, class T2>
void rprint(std::multimap<T, T2>& todo) {
  for (const auto& i : todo | std::views::reverse) {
	print("{}: {}\n", i.first, i.second);
  }
  print("\n");
}

void printc(const std::ranges::range auto& v, std::string_view s = "") {
  if (s.size())print("{}: ", s);
  for (const auto& i : v)print("[{}] ", i);
  print("\n");
}

void printr(const auto& r, std::string_view s = "") {
  auto rbegin = std::rbegin(r);
  auto rend = std::rend(r);
  for (auto it = rbegin; it != rend; ++it) {
	print("{} ", *it);
  }
  print("\n");
}

/**************************************5.4******************************************************************/
bool is_div4(int i)
{
  return i % 4 == 0;
}

struct is_div4_
{
  bool operator()(int i)
  {
	return i % 4 == 0;
  }
};

auto is_div_by(int divisor)
{
  return [=](int i)
  {
	return i % divisor == 0;
  };
}

/**************************************5.5******************************************************************/
#include<functional>
#include<list>
#include<deque>

void hello()
{
  print("hello\n");
}

struct Hello_
{
  void greeting()
  {
	print("hello\n");
  }
};

/**************************************5.6******************************************************************/
//recusiv to link to lambda
template<typename F,typename ...Ts>
auto concat(F t, Ts ...ts)
{
  if constexpr (sizeof...(ts) > 0)
  {
	return [=](auto ...parameters)
	{
	  return t(concat(ts...)(parameters...));
	};
  }
  else
  {
	return t;
  }
}

/**************************************5.7******************************************************************/
static bool begins_with_a(const std::string& s)
{
  return s.find("a") == 0;
}

static bool ends_with_b(const std::string& s)
{
  return s.rfind("b") == s.length() - 1;
}

template<typename A, typename B, typename F>
auto combine(F binary_func, A a, B b)
{
  return [=] (auto param)
  {
	return binary_func(a(param), b( param ));
  };
}
/**************************************5.8******************************************************************/

auto braces(const char a, const char b)
{
  return[a, b](const auto v)
  {
	print("{} {} {}", a, v, b);
  };
}

/**************************************5.9******************************************************************/
const char prompt(const char* p)
{
  std::string r;
  print("{} >", p);
  std::getline(std::cin, r, '\n');

  if (r.size() < 1)
	return '\0';
  if (r.size() > 1)
  {
	print("reaction time too long\n");
	return '\0';
  }

  return toupper(r[0]);
}

int main()
{

  // 5.3 scoped reusable code 
#if Five_three
  auto one = []() {return "one"; };
  auto two = []() {return "two"; };
  print("{} {}\n", one(), two());

  auto p = [](auto f)
  {
	print("{}\n", f());
  };
  p([] {return "hhhh"; });

  auto p2 = []<typename T>(T && f)
  {
	print("{}\n", f());
  };
  p2(one);
  p2(std::move(one));

  [] <typename T>(T&& f)
  {
	print("{}\n", f());
  }(two);

  int num{};
  auto p3 = [num]()mutable {++num; };
  for (auto i = 0; i < 5; ++i)
	p3();

  print("{}\n", num);

  auto p4 = [&]()mutable {++num; };
  print("{}\n", sizeof(p4));

  constexpr int n = []()constexpr {return 10 * 10; }();

  auto p5 = []()->int {return 10; };

#endif // Five_three

  // As a predicate in the algorithm library
#if Five_four
  std::vector v{ 1,2,3,4,5,44,10,32 };
  auto count1 = std::count_if(v.begin(), v.end(), is_div4);
  auto count2 = std::count_if(v.begin(), v.end(), is_div4_{});
  print("{} {}\n", count1, count2);
  auto count3 = std::count_if(v.begin(), v.end(), [](int i) {return i % 4 == 0; });
  print("{}\n", count3);

  for (int i : {3, 4, 5, 6})
  {
	auto count = std::ranges::count_if(v, is_div_by(i));
	print("{} ", count);
  }

  int(*p)(int) = [](int a) {return a;  };
  print("{}\n", p(100));
#endif

#if Five_five
  std::deque<int>d;
  std::list<int>l;
  std::vector<int>v;

  auto print_c = [](const auto& c)
  {
	for (const auto& i : c)
	  print("{}", i);
	print("\n");
  };

  auto push_c = [](auto& container) {
	return[&container](auto value)
	{
	  container.push_back(value);
	};
  };

  const std::vector<std::function<void(int)>>consumers{ push_c(d),push_c(l),push_c(v) };

  for (auto& i : consumers)
  {
	for (size_t j = 0; j < 10; ++j) {
	  i(j);
	}
  }
  
  print_c(d);
  print_c(l);
  print_c(v);

  std::function f{ hello };
  f();
  Hello_ h;
  std::function<void(void)>ff{ std::bind(&Hello_::greeting,&h) };
  ff();
  std::bind(&Hello_::greeting, &h)();

#endif

#if Five_six
  auto twice = [](auto i) {return i * 2; };
  auto three = [](auto i) {return i * 3; };

  auto combined = concat(three, twice, std::plus<int>{});
  print("{} \n", combined(2, 3));
#endif

#if Five_seven
  auto a_xxx_b{ combine(std::logical_and<int>{},begins_with_a,ends_with_b) };
  
  std::copy_if(std::istream_iterator<std::string>{std::cin}, {}, std::ostream_iterator<std::string>{std::cout, ", "}, a_xxx_b);
  std::cout << '\n';
#endif


#if Five_eight
  auto a = braces('(', ')');
  auto b = braces('[', ']');
  auto c = braces('{', '}');
  auto d = braces('|', '|');

  for (int i : {1, 2, 3, 4, 5})
  {
	for (auto x : { a,b,c,d })
	{
	  x(i);
	}
	print("\n");
  }
#endif


#if Five_nine
  using jumpfunc = void(*)();

  std::map<const char, jumpfunc>jumpmap{
	{'A',[] {print("func A\n"); }},
	{'B',[] {print("func B\n"); }},
	{'C',[] {print("func C\n"); }},
	{'D',[] {print("func D\n"); }},
	{'X',[] {print("Bye\n"); }},
  };

  char select{};
  while (select != 'X')
  {
	if ((select = prompt("select A/B/C/DX")))
	{
	  auto it = jumpmap.find(select);
	  if (it != jumpmap.end())
		it->second();
	  else
		print("no choice\n");
	}
  }
  
#endif
  return 0;
}


