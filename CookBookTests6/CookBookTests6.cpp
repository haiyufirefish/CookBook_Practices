// CookBookTests6.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma once
#include<format>
#include<iostream>
#include<ranges>
#include<map>
#include<vector>
#include<filesystem>

#define Six_two 0
#define Six_three 0
#define Six_four 0
#define Six_five 0
#define Six_six 0
#define Six_seven 0
#define Six_eight 0
#define Six_nine 0
#define Six_ten 1


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

/**************************************6.2******************************************************************/
namespace stdr = std::ranges;

/**************************************6.3******************************************************************/
#include<numbers>
namespace bw
{
  template<typename T>
  std::ostream& join(T it, T end_it, std::ostream& o, std::string_view sep = "")
  {
	if (it != end_it)o << *it++;
	while (it != end_it)
	  o << sep << *it++;
	return o;
  }

  template<typename T>
  std::string join(T it, T end_it, std::string_view sep = "")
  {
	std::ostringstream ostr;
	join(it, end_it, ostr, sep);
	return ostr.str();
  }

  std::string join(const auto& c, std::string_view sep = "")
  {
	return join(std::begin(c), std::end(c), sep);
  }
}
/**************************************6.4******************************************************************/
#include<random>
void check_sorted(auto& c)
{
  if (!std::is_sorted(c.begin(), c.end()))
	print("un");
  print("sorted ");
}

void print_c(const auto& c)
{
  check_sorted(c);
}

void randomize(auto& c)
{
  static std::random_device rd;
  static std::default_random_engine rng(rd());
  std::shuffle(c.begin(), c.end(), rng);
}

struct things
{
  std::string s_;
  int i_;
  std::string str()const {
	return std::format("({},{})", s_, i_);
  }
};

void print_things(const auto& c)
{
  for (auto& v : c)
	print("{} ", v.str());
  print("\n");
}

/**************************************6.5******************************************************************/
// transfrom to modify the content of container
std::string str_lower(const std::string& s)
{
  std::string outstr{};
  for (const char& c : s)
  {
	outstr += tolower(c);
  }
  return outstr;
}

/**************************************6.6******************************************************************/
struct City
{
  std::string name{};
  unsigned pop{};
  bool operator==(const City& o)const
  {
	return name == o.name;
  }

  std::string str()const
  {
	return std::format("[{},{}]", name, pop);
  }
};

/**************************************6.7******************************************************************/
constexpr int ilow{ 0 };
constexpr int ihigh{ 500 };

void printc_(const std::ranges::range auto& v, std::string_view s = "")
{
  for (const auto& i : v)
	print("{:>5}", i);
  print("\n");
}


/**************************************6.8******************************************************************/
int iround(const double& d)
{
  // explict cast, round return double
  return static_cast<int>(std::round(d));
}
int main()
{

  //6.2 copy based on iterator
#if Six_two
  std::vector<std::string>v1{ "alpha","beta","gamma","delta","epsilon" };
  printc(v1, "v1");

  std::vector<std::string>v2(v1.size());
  std::copy(v1.begin(), v1.end(), v2.begin());
  printc(v2, "v2");

  std::copy(v1.begin(), v1.end(), std::back_inserter(v2));
  printc(v2, "v2");

  std::vector<std::string>v3{};
  std::copy(v1.begin(), v1.end(), std::back_inserter(v3));
  printc(v3, "v3");

  // ok
  std::vector<std::string>v4{};
 /* std::copy_if(v1.begin(), v1.end(), std::back_inserter(v4), [](auto& s) {
	return s.size() > 4; });
  printc(v4, "v4");*/

  // the same as above
  stdr::copy_if(v1.begin(), v1.end(), std::back_inserter(v4), [](auto& s) {
	return s.size() > 4; });
  printc(v4, "v4");

  // using copy for std::cout
  stdr::copy(v1, std::ostream_iterator<std::string>(std::cout, " "));
  print("\n");
  // std::move has this function no
  stdr::move(v1, v2.begin());
  printc(v1, "v1 after move");
  printc(v2, "v2 after move");

#endif // Six_two

  // joint the container elements
#if Six_three
  std::vector<std::string>greek{ "alpha","beta","gamma","delta","epsilon" };
  for (const auto& c : greek)
	std::cout << c << ",";
  print("\n");

  auto greek_view = greek | std::views::join;
  for (const auto& c : greek_view)
	std::cout << c ;
  print("\n");

  bw::join(greek.begin(), greek.end(), std::cout, ", ") << '\n';

  auto s = bw::join(greek.begin(), greek.end(), ", ");
  print("{}\n", s);

  auto s2 = bw::join(greek, ".. ");
  print("{}\n", s2);

  std::list<double>list{ std::numbers::pi,std::numbers::e,std::numbers::sqrt2 };
  print("{}\n", bw::join(list, "; "));

#endif

  //sort the elements
#if Six_four
  std::vector<int>v{ 1,2,3,4,5,6,7,8,9,10 };
  print_c(v);
  printc(v, "vecto rbefore partition");


 // for (int i{ 3 }; i; --i)
 // {
	//randomize(v);
	//print_c(v);
 // }

 // std::sort(v.begin(), v.end());
 // print_c(v);

 // print("partial sorted:\n");
 // randomize(v);

 // auto middle{ v.begin() + (v.size()) / 2 };
 // std::partial_sort(v.begin(), middle, v.end());
 // print_c(v);

  std::partition(v.begin(), v.end(), [](int i) {return i > 5; });
  print_c(v);
  printc(v, "vector after partition");

#endif
  // transform the elements
#if Six_five
  std::vector<int>v1{ 1,2,3,4,5,6,7,8,9,10 };
  std::vector<int>v2;
  printc(v1, "v1");
  std::transform(v1.begin(), v1.end(), std::back_inserter(v2), [](int x) {return x * 4; });
  printc(v2, "v2");

  std::vector<std::string>vstr1{ "Aaa","Bbb","Ccc","DdD" };
  std::vector<std::string>vstr2;
  printc(vstr1, "vstr1");
  print("str_ to lower:\n");
  std::transform(vstr1.begin(), vstr1.end(), std::back_inserter(vstr2), [](std::string& x) {return str_lower(x); });
  printc(vstr2, "vstr2");

  print("ranges sequences:\n");
  auto view1 = std::views::transform(v1, [](int x) {return x * x; });
  printc(view1, "view1");

  v2.clear();
  std::ranges::transform(v1, std::back_inserter(v2), [](int x) {return x * x; });
  printc(v2, "v2");
#endif

  // find specific choices
#if Six_six
  const std::vector<int>v{ 1,2,3,4,5,6,7,8,9,10 };
  auto it1 = std::find(v.begin(), v.end(), 7);
  print("{}" ,it1 == v.end()?0:*it1);

  const std::vector<City>c{
	{"London",2374895},
	{"Paris",8732895},
	{"Lisbon",1233895},
	{"Munich",9987655},
	{"Berlin",12155}
  };
  auto it2 = std::find(c.begin(), c.end(), City{ "Berlin" });
  if (it2 != c.end())
	print("found: {}\n", it2->str());

  auto it3 = std::find_if(std::begin(c), std::end(c), [](const City& item) {
	return item.pop > 200000; });

  if (it3 != c.end())
	print("found: {}\n", it3->str());

  auto vwl = std::views::filter(c, [](const City& item) {
	return item.pop > 200000; });
  for (const City& e : vwl)
  {
	print("{}\n", e.str());
  }
#endif

  // using clamp to restrict the range
#if Six_seven
  auto il = { 1,5,16,3,10,-100,2,1555,121,4440,777 };
  std::vector<int>voi{ il };
  print("vector voi before:\n");
  printc_(voi);

  print("vector voi after clamp:\n");
  for (auto& e : voi)
	e = std::clamp(e, ilow, ihigh);
  printc_(voi);

  print("list loi before:\n");
  std::list<int>loi{ il };
  printc_(loi);
  std::transform(loi.begin(), loi.end(), loi.begin(), [](auto e) {
	return std::clamp(e, ilow, ihigh); });

  print("list loi after\n");
  printc_(loi);
#endif

  // using sample to smaple data
#if Six_eight 
  constexpr size_t n_data{ 200000 };// data size
  constexpr size_t n_samples{ 500 };// container size
  constexpr int mean{ 0 };// mean value
  constexpr size_t dev{ 3 }; // variance

  std::random_device rd;  // access to hardware random generator 
  std::mt19937 rng{ rd() }; // random generator

  std::normal_distribution<>dist{ mean,dev };
  std::array<int, n_data>data{};
  // round value
  for (auto& e : data)e = iround(dist(rng));

  std::array<int, n_samples>samples{};
  std::sample(data.begin(), data.end(), samples.begin(), n_samples, rng);

  std::map<int, size_t>hist{};
  for (const int i : samples)
	++hist[i];

  constexpr size_t scale{ 3 };
  print("{:>3} {:>5} {:<}/ {}\n", "n", "count", "graph", "sacle");
  for (const auto& [value, count] : hist)
  {
	print("{:>3},({:>3}) {}\n", value, count, std::string(count, '*'));
  }
#endif

  // generate ordered sequences
#if Six_nine
  std::vector<std::string>vs{ "dog","cat","velociraptor" };
  std::sort(vs.begin(), vs.end());
  do
  {
	printc(vs);
  } while (std::next_permutation(vs.begin(), vs.end()));

  std::vector<int>v{ 1,2,3,4 };
 // do
 // {
	//printc(v);
 // } while (std::next_permutation(v.begin(), v.end()));

  print("small to big:\n");
  std::sort(v.begin(), v.end(), std::less<int>{});
  do {
	printc(v);
  } while (std::next_permutation(v.begin(), v.end()));
#endif

  // merge sorted container
#if Six_ten
  std::vector<std::string>vs1{ "dog","cat","veloiraptor" };
  std::vector<std::string>vs2{ "kirk","sulu","spock" };
  std::vector<std::string>dest{};
  printc(vs1, "vs1");
  printc(vs2, "vs2");

  std::ranges::sort(vs1);
  std::ranges::sort(vs2);
  printc(vs1, "vs1");
  printc(vs2, "vs2");

  std::merge(vs1.begin(), vs1.end(), vs2.begin(), vs2.end(), std::back_inserter(dest));
  printc(dest, "dest");
#endif
}


