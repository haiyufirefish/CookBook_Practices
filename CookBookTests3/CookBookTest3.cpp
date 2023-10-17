// CookBookTest3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <format>
#include <ranges>

template<typename... Args>
void print(const std::string_view fmt_str, Args&&... args)
{
  auto fmt_args{ std::make_format_args(args...) };
  std::string outstr{std::vformat(fmt_str, fmt_args)};
  fputs(outstr.c_str(), stdout);
}

void print(std::ranges::range auto v) {
  print("size: {}  ", v.size());
  print("[ ");
  for (const auto& i : v)print("{} ", i);
  print("]\n");
}

/**************************************3.3******************************************************************/
#include <vector>
#include <list>
template<typename Tc,typename Tv>
void remove_value(Tc& c, const Tv& v)
{
  // before c++ 20 std::remove put the element at the end, retrun iterator front
  auto remove_it = std::remove(c.begin(), c.end(), v); // remove_it is the first element that to be removed
  c.erase(remove_it, c.end());// remove_it to end() range
}


/**************************************3.4******************************************************************/
namespace stdr = std::ranges;
template<typename T>
void quick_delete(T& v, size_t idx)
{
  if (idx < v.size())
  {
    v[idx] = std::move(v.back());
    v.pop_back();
  }
}

template<typename T>
void quick_delete(T& v, typename T::iterator it)
{
  if (it < v.end())
  {
    *it = std::move(v.back());
    v.pop_back();
  }
}

/**************************************3.5******************************************************************/
void test51()
{
  std::vector v2{ 1,2,3,4,5,7 };
  v2[6] = 200;
  auto& i = v2[6];
  print("{}\n", i);
}

void test52() try
{
  std::vector v2{ 1,2,3,4,5,7 };
  auto& i = v2[6];
  print("{}\n", i);
}
catch (std::exception& e) {
  print("{}\n", e.what());
}


void test53()  try
{
  std::vector v2{ 1,2,3,4,5,7 };
  auto& i = v2.at(6);
  print("{}\n", i);
}
catch (std::exception& e) {
  print("{}\n", e.what());
}

/**************************************3.6******************************************************************/
#include <algorithm>
void psorted(stdr::range auto&& v)
{
  if (stdr::is_sorted(v))
    print("sorted: ");
  else
    print("unsorted: ");
  print(v);
}

void insert_sorted(std::vector<std::string> &v, const std::string& s)
{
  const auto pos{ stdr::lower_bound(v,s) };
  v.insert(pos, s);
}

template<stdr::range C, typename E>
void insert_sorted(C &c, const E& e)
{
  const auto pos{ stdr::lower_bound(c,e) };
  c.insert(pos, e);
}

/**************************************3.7******************************************************************/
#include<map>
struct X
{
  std::string s;
  X() { print("default construct\n"); }
  X(const char* s) : s{ s } { print("construct \n"); }
  X(const X&) { print("copy constuct \n"); }
};

void printm(const std::map<int, X>& map)
{
  for (const auto& [k, v] : map)
  {
    print("[ {}:{} ]", k, v.s);
  }
  print("\n");
}

template<class K,class V>
void printm(const std::map<K, V>& maps)
{
  for (const auto& [k, v] : maps)
  {
    print("[ {}:{} ]", k, v);
  }
  print("\n");
}
 
/**************************************3.8******************************************************************/
template<typename M,typename K>
bool node_swap(M& m, K k1, K k2)
{
  auto node1{ m.extract(k1) };
  auto node2{ m.extract(k2) };
  if (node1.empty() || node2.empty())
    return false;
  std::swap(node1.key(), node2.key());
  m.insert(std::move(node1));
  m.insert(std::move(node2));
  return true;
}

/**************************************3.9******************************************************************/
// self defined key value in unordered_map
#include<unordered_map>
#include<set>

struct Coord
{
  int x{};
  int y{};
};
// unodered_map look up and order the elements according to hash value of the object
auto operator==(const Coord& a, const Coord& b)
{
  return a.x == b.x && a.y == b.y;
}

// the default hash function doesn't exist for user-defined types.
namespace std
{
  template<>
  struct hash<Coord>
  {
    size_t operator()(const Coord& a)const
    {
      return static_cast<size_t>(a.x) + static_cast<size_t>(a.y);
    }
  };
}

template<class T1,class T2>
inline void print(const std::unordered_map<T1, T2>& map)
{
  print("size:{} ", map.size());
  for (auto& [k, v] : map)print("{{{}{}}}: {}", k.x, k.y, v);
  print("\n");
}

template<class T, class T2>
void rprint(std::multimap<T, T2>& todo) {
  for (const auto& i : todo | std::views::reverse) {
    print("{}: {}\n", i.first, i.second);
  }
  print("\n");
}

/**************************************3.13******************************************************************/
bool is_eos(const std::string_view& str)
{
  constexpr const char* end_punkt{ ".!?" };
  for (auto c : str)
  {
    if (strchr(end_punkt, c) != nullptr)
      return true;
  }
  return false;
}

int main()
{
  // 3.3
  //std::vector<int> v1{ 1,2,3,4,5,6 };
  //print(v1);
  //::remove_value(v1, 1);
  //print(v1);

  // 3.4  if the order of removed vector is not important, it can be optimized in O(1)
  //std::vector<int> v2{ 1,3,23,4,5,6 };
  //print(v2);
  //auto it = stdr::find(v2, 3);
  //quick_delete(v2,it);
  //print(v2);

  //quick_delete(v2, 2);
  //print(v2);

  // 3.5
  // test51();
  // test52();
  // test53(); // at vs [], at will check the index out of range or not, then return []

  // 3.6 
  //std::vector<std::string>v1{ "2", "1", "3", "0" };
  //psorted(v1);

  //stdr::sort(v1);
  //psorted(v1);

  ////v1.emplace_back("-1"); 
  ////psorted(v1); // not sorted

  //::insert_sorted(v1, "-1");
  //psorted(v1);

  //// test fully template using list
  //std::list<int> li{ -1,1,2,3,4,5 };
  //psorted(li);
  //::insert_sorted(li, 0);
  //psorted(li);

  // 3.7 insert element efficiently
  //std::map<int, X>map{};
  //map[1] = "aaa";
  //print("\n");
  //map.emplace(2, "bbb");
  //map.emplace(3, "ccc");
  //printm(map);

  //// map.emplace(1, "aaa"); // impossible
  //map.try_emplace(1, "ddd"); // impossible
  //printm(map);

  // 3.8 efficent modify the key value in map
  //std::map<int, std::string>maps{
  //  {1,"a"}, {2,"b"}, {3,"c"}, {4,"d"},{5,"e"}
  //};
  //printm(maps);
  //::node_swap(maps, 3,4);
  //printm(maps);

  //auto node = maps.extract(maps.begin());
  //node.key() = 5;
  //auto t = maps.insert(std::move(node));
  //printm(maps); // fails exist already

  //if (!t.inserted)
  //{
  //  print("insert fails:{}\n", t.position -> second);
  //  print("insert fails:{}\n", t.position -> first);
  //}

  // 3.9
  //std::unordered_map<Coord, std::string>map{ {{1,1},"aaa"}, {{2,2},"bbb"} };
  //print(map);

  // 3.10 using set as inputer and sorted the elements
 /* std::set<std::string>sets;
  std::copy(std::istream_iterator < std::string> {std::cin}, {},
    std::inserter(sets, sets.end()));
  print(sets);*/

  // 3.14 multimap
  //std::multimap<int, std::string>todo{
  //  {1,"a"},
  //  {1,"aaa"},
  //  {2,"b"},
  //  {3,"c"},
  //  {4,"d"}
  //};
  //rprint(todo);

  // 3.13
  std::vector<std::vector<std::string>> vv_sentences{ std::vector<std::string>{} };
  for (std::string s{}; std::cin >> s;)
  {
    vv_sentences.back().emplace_back(s);
    if (is_eos(s))
    {
      vv_sentences.emplace_back(std::vector<std::string>{});
    }
  }

  if (vv_sentences.back().empty())
    vv_sentences.pop_back();
  stdr::sort(vv_sentences, [](const auto& l, const auto& r)
    {
      return l.size() > r.size();
    });

  for (const auto& v : vv_sentences)
  {
    size_t size = v.size();
    print("{}: ", size);
    for (const auto& s : v)
      print("{}", s);
    print("\n");
  }
  print("\n");

  return 0;
}

