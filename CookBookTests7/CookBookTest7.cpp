// CookBookTests7.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma once
#include<format>
#include<iostream>
#include<ranges>
#include<map>
#include<vector>
#include<filesystem>
#include<string>

#define Seven_3 0
#define Seven_4 0
#define Seven_5 0
#define Seven_6 0
#define Seven_7 1
#define Seven_8 1
#define Seven_9 1
#define Seven_10 1
#define Seven_11 1
#define Seven_12 1


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

/**************************************7.3******************************************************************/
//constexpr char char_lower(const char& c)
//{
//  return static_cast<char>(std::to_lower(c));
//}
using namespace std::literals;

std::string_view s()
{
  const char text[]{ "hello" };
  std::string_view greeting{ text };
  return greeting;
}

void f1(const std::string& str)
{

}

void f2(std::string_view str)
{

}

/**************************************7.4******************************************************************/
#include<sstream>
#include<chrono>

using std::chrono::high_resolution_clock;
using std::chrono::duration;

void timer(auto(f)()->std::string)
{
  auto t1 = high_resolution_clock::now();
  std::string s{ f() };
  auto t2 = high_resolution_clock::now();
  duration<double, std::milli>ms = t2 - t1;
  print("{}", s);
  print("duration is {} ms: \n", ms.count());
  
}

std::string concat_string()
{
  print("concat_string\n");
  std::string a{ "a" };
  std::string b{ "b" };
  long n{};
  while (++n)
  {
	std::string x{};
	x += a + ", " + b + "\n";
	if (n >= 10000000)
	  return x;
  }

  return "error\n";
}

std::string append_string()
{
  print("append_string\n");
  std::string a{ "a" };
  std::string b{ "b" };
  long n = 0;
  while (++n)
  {
	std::string x{};
	x.append(a);
	x.append(", ");
	x.append(b);
	x.append("\n");
	if (n >= 10000000)
	  return x;

  }

  return "error\n";
}

std::string concat_format()
{
  print("format_string\n");
  std::string a{ "a" };
  std::string b{ "b" };
  long n = 0;
  while (++n)
  {
	std::string x{};
	x += std::format("{}, {}\n", a, b);
	if (n >= 10000000)
	  return x;

  }
  return "error\n";

}

std::string concat_ostringstream() {
  print("ostringstream\n");
  std::string a{ "a" };
  std::string b{ "b" };
  long n{};
  while (++n) {
	std::stringstream x{};
	x << a << ", " << b << "\n";
	if (n >= 10000000)return x.str();
  }
  return "error\n";
}

/**************************************7.5******************************************************************/
char char_lower(const char& c)
{
  return static_cast<char>(std::tolower(c));
}

char char_upper(const char& c)
{
  return static_cast<char>(std::toupper(c));
}

char rot13(const char& x)
{
  auto rot13a = [](char x, char a)->char
  {
	return a + (x - a + 13) % 26;
  };
  if (x >= 'A' && x <= 'Z')
	return rot13a(x,'A');

  if (x >= 'a' && x <= 'z')
	return rot13a(x, 'a');

  return x;
}

std::string title_case(std::string& s) {
  auto begin{ s.begin() };
  auto end{ s.end() };
  *begin++ = char_upper(*begin);
  bool space_flag{ false };
  for (auto it{ begin }; it != end; ++it) {
	if (*it == ' ')space_flag = true;
	else {
	  if (space_flag)*it = char_upper(*it);
	  space_flag = false;
	}
  }
  return s;
}

/**************************************7.6******************************************************************/
#include<numbers>
#include<vector>
template<typename T>
struct Frac
{
  T n;
  T d;
};

// easy formatter
template<typename T>
struct std::formatter<Frac<T>>
{
  template<typename ParseContext>
  constexpr auto parse(ParseContext& ctx)
  {
	return ctx.begin();
  }
 
  template<typename FormatContext>
  constexpr auto format(const Frac<T>& f, FormatContext& ctx)
  {
	return std::format_to(ctx.out(), " {0:d}/{1:d}", f.n, f.d);
  }
};

template<typename T>
struct Frac2 {
  T n;
  T d;
};
template<typename T>
struct std::formatter<Frac2<T>> {
  constexpr auto parse(auto& ctx) {
	m_fmt[m_buffer_len++] = '{';
	auto iter = ctx.begin();
	if (iter == ctx.end() || *iter == '}') {
	  m_fmt[m_buffer_len++] = '}';
	  return iter;
	}
	m_fmt[m_buffer_len++] = ':';
	for (; iter != ctx.end() && *iter != '}'; ++iter)
	  m_fmt[m_buffer_len++] = *iter;
	m_fmt[m_buffer_len++] = '}';
	return iter;
  }
  constexpr auto format(const Frac2<T>& f, auto& ctx) {
	std::string fmt{};
	fmt += m_fmt, fmt += "/", fmt += m_fmt;
	auto iter = std::vformat_to(ctx.out(), fmt, std::make_format_args(f.n, f.d));
	return iter;
  }
private:
  char m_fmt[16]{};
  size_t m_buffer_len = 0;
};


/**************************************7.7******************************************************************/
std::string trimstr(const std::string& s) {
  constexpr const char* whitespace{ " \t\r\n\v\f" };
  if (s.empty())return s;
  const auto first{ s.find_first_not_of(whitespace) };
  if (first == std::string::npos)return{};
  const auto last{ s.find_last_not_of(whitespace) };
  return s.substr(first, (last - first + 1));
}

/**************************************7.9******************************************************************/
size_t wordcount(auto& is)
{
  using it_t = std::istream_iterator<std::string>;
  return std::distance(it_t{ is }, {});
}

/**************************************7.11******************************************************************/
constexpr char char_lower_(const char& c)
{
  if (c >= 'A' && c <= 'Z')
	return c + 32;
  else
	return c;
}

int main()
{
  //string_view
#if Seven_3
  char str1[10]{ "hello" };
  std::string str2{ str1 }; // copy constructor
  print("{}\n", str2);
  str1[0] = 'a';
  print("{}\n", str2); // not change

  std::string_view str3{ str1 }; // view reference taken
  print("{}\n", str3);
  str1[0] = 'c';
  print("{}\n", str3); //  change

 std::string_view t = s();
  print("view:{}\n", t);

  print("{}\n", "hello"sv.substr(1, 4));

  constexpr std::string_view str4{ "hhhhh" }; // ok
 // constexpr std::string str5{ "hhhhh" }; // not ok

  std::string str5{ "1" };
  const std::string str6{ "2" };
  f1(str5);
  f1(str6);
  f1("a"); // not recommend

  // ok and better
  f2(str5);
  f2(str6);
  f2("a");
#endif

#if Seven_4
  // concat <=append < osstringstream < format
  timer(append_string);
  timer(concat_format);
  timer(concat_ostringstream);
  timer(concat_string);
#endif

#if Seven_5
  std::string s{ "hello jimi\n" };
  print("{}", s);
  std::transform(s.begin(), s.end(), s.begin(), char_upper);
  print("{}", s);
  for (auto& c : s)c = rot13(c);
  print("{}", s);
  for (auto& c : s)c = rot13(char_lower(c));
  print("{}", s);

  title_case(s);
  print("{}", s);
#endif

#if Seven_6
  const int a{ 47 };
  const char* human{ "earthlings" };
  const std::string_view alien{ "vulacans" };
  const double df_pi{ std::numbers::pi };
  const int inta{ 47 };

  print("Hex: {0:x} Octal: {0:o} Decunak {0:d}\n", a);
  print("Hello {1} we are {0}\n", human, alien);
  print("Pi is {}\n", df_pi);
  print("Pi is {:.5}\n", df_pi);

  print("inta is [{:10}]\n", inta);
  print("inta is [{:<10}]\n", inta);
  print("inta is [{:>10}]\n", inta);

  print("inta is [{:*<10}]\n", inta);
  print("inta is [{:0>10}]\n", inta);

  print("inta is [{:^10}]\n", inta);
  print("inta is [{:_^10}]\n", inta);

  print("{:>8}: [{:04x}]\n", "Hex", inta);
  print("{:>8}: [{:4o}]\n", "Octal", inta);
  print("{:>8}: [{:4d}]\n", "Decimal", inta);

  Frac<long>n1{ 3,5 };
  //print("{}\n", n1); // error
  //print("{:0x}\n", n);//error
 // Frac2<long>n2{ 10,5 }; 
  //print("{:0x}\n", n2);// not ok
  int array[] = { 1,2,3,4,5,6 };
  std::vector v = { 122, 1222, 1222 };
 // print("{:0x}\n", v); // error
 // print("{:0x}\n", array); // error
#endif

#if Seven_7
  std::string s{ " \t ten-thumbed input \t  \n \t" };
  print("[{}]\n", s);
  print("[{}]\n", trimstr(s));
#endif
}


