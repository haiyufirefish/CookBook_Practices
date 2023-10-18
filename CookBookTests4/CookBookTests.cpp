// CookBookTests4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <format>
#include <ranges>
#include <algorithm>
#include <map>

template < typename... Args>
void print(const std::string_view fmt_str, Args&&... args) {
  auto fmt_args{ std::make_format_args(args...) };
  std::string outstr{ std::vformat(fmt_str, fmt_args) };
  fputs(outstr.c_str(), stdout);
}

// vector, list....
void print(std::ranges::range auto v) {
  print("size: {}  ", v.size());
  print("[ ");
  for (const auto& i : v)print("{} ", i);
  print("]\n");
}

//map
template<class T, class T2>
void print(const std::map<T, T2>& map) {
  print("size: {} ", map.size());
  print("[ ");
  for (auto& [k, v] : map)print("{}:{} ", k, v);
  print("]\n");
}

// reverse multimap
template<class T, class T2>
void rprint(std::multimap<T, T2>& todo) {
  for (const auto& i : todo | std::views::reverse) {
	print("{}: {}\n", i.first, i.second);
  }
  print("\n");
}

void printc(const std::ranges::range auto& v, std::string_view s = "")
{
  if (s.size())
    print("{} ", s);
  for (const auto& i : v)
    print("[{}]", i);
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

/**************************************4.3 4.4******************************************************************/

// iterator way 
template<class T>
class Seq
{
  T _star{};
  T _end{};
public:
  Seq(T star, T end) : _star(star), _end(end) {}
  struct iterator
  {
    T value{};

    using value_type = std::remove_cv_t<T>;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    explicit iterator(T v = 0) :value(v) {}

    iterator& operator++()
    {
      ++value;
      return *this;
    }
    // these two are important
    iterator operator++(int)
    {
      auto t{ *this };
      ++* this;
      return t;
    }
    T operator*() const
    {
      return value;
    }
    bool operator!=(const iterator& l)const noexcept
    {
      return this->value != l.value;
    }

    bool operator==(const iterator& l)const noexcept
    {
      return this->value == l.value;
    }
  };

  iterator begin() const
  {
    return iterator{ _star };
  }

  iterator end() const
  {
    return iterator{ _end };
  }
};

template<class T>
requires std::forward_iterator<typename T::iterator>
void printc(const T& r)
{
  for(const auto& i : r)
    print("{}\t", i);
  print("\n");
}



// array way for self defined iterator
template<class T, size_t size>
struct X {
  T array[size]{};
  T* begin()
  {
    return array;
  }

  T* end()
  {
    return array + size;
  }
};

#include <deque>
#include <sstream>
#include <vector>
/**************************************4.6******************************************************************/
class fib_generator
{
  using fib_t = unsigned long;
  fib_t stop_{};
  fib_t count_{ 0 };
  fib_t a_{ 0 };
  fib_t b_{ 1 };
  constexpr void do_fib()
  {
    const fib_t old_d = b_;
    b_ += a_;
    a_ = old_d;
  }
public:
  //using here are necessary
  using iteraor_concept = std::forward_iterator_tag;
  using iteraor_catagory = std::forward_iterator_tag;
  using value_type = std::remove_cv_t<fib_t>;
  using difference_type = std::ptrdiff_t; 
  using pointer = const fib_t*;
  using reference = const fib_t&;

  explicit fib_generator(fib_t stop = 0) :stop_{ stop } {}

  fib_t operator*()const { return b_; }
  constexpr fib_generator& operator++()
  {
    do_fib();
    ++count_;
    return *this;
  }

  fib_generator operator++(int)
  {
    auto tmp{ *this };
    ++* this;
    return tmp;
  }

  bool operator==(const fib_generator& o)const
  {
    return count_ == o.count_;
  }
  const fib_generator& begin()const
  {
    return *this;
  }
  const fib_generator end()const
  {
    auto sentinel = fib_generator();
    sentinel.count_ = stop_;
    return sentinel;
  }

  fib_t size()
  {
    return stop_;
  }

};

/**************************************4.8******************************************************************/
class cstr_it
{
  const char* s{};
  static constexpr const char nullchar = '\0';

public:
  explicit cstr_it(const char* str) :s{ str } {}
  char operator*() const { return *s; }
  cstr_it& operator++()
  {
    ++s; 
    return *this;
  }
  bool operator!=(const char)const
  {
    return s != nullptr && *s != nullchar;
  }
  cstr_it begin() const
  {
    return *this;
  }
  const char end()const
  {
    return nullchar;
  }
};

void print_cstr(const char* s)
{
  print("{}: ", s);
  for (char c : cstr_it(s))
  {
    print("{:02x}: ", c);
  }
  print("\n");
}

/**************************************4.9******************************************************************/
template<typename T>
class zip_iterator
{
  using val_t = typename T::value_type;
  using ret_t = std::pair<val_t, val_t>;
  using it_t = typename T::iterator;

  it_t ita_{};
  it_t itb_{};
  it_t ita_begin_{};
  it_t ita_end_{};
  it_t itb_begin_{};
  it_t itb_end_{};

  zip_iterator(it_t ita, it_t itb) :ita_{ ita }, itb_{ itb }{}

public:
  using value_type = std::pair<val_t, val_t>;
  using difference_type = long int;
  using pointer = const val_t*;
  using reference = const val_t&;

  zip_iterator(T& a,T& b):
    ita_{ a.begin() }, itb_{ b.begin() }, ita_begin_{ ita_}, itb_begin_{ itb_ }, ita_end_{ a.end() }, itb_end_{ b.end() }{}

  zip_iterator& operator++()
  {
    ++ita_;
    ++itb_;
    return *this;
  }

  bool operator==(const zip_iterator& o)const
  {
    return ita_ == o.ita_ || itb_ == o.itb_;
  }
  bool operator!=(const zip_iterator& o)const
  {
    return !operator==(o);
  }
  ret_t operator*()const
  {
    return { *ita_,*itb_ };
  }
  zip_iterator begin()const
  {
    return { ita_begin_,itb_begin_ };
  }
  zip_iterator end()const
  {
    return { ita_end_,itb_end_ };
  }
};

/**************************************4.10******************************************************************/
#include<memory>

template<typename T>
class Container
{
  class iterator
  {
    T* ptr_;

  public:
    using iterator_concept = std::contiguous_iterator_tag;
    using iterator_category = std::contiguous_iterator_tag;
    using value_type = std::remove_cv_t<T>;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    iterator(T* ptr_ = nullptr) :ptr_{ ptr_ } {}

    const auto operator<=>(const iterator& o)const
    {
      ptr_ <=> o.ptr_;
    }

    iterator operator+(size_t size)const
    {
      return { ptr_ + size };
    }

    friend const iterator operator+(size_t size, const iterator& o)
    {
      return { o.ptr_ + size };
    }

    const iterator operator-(size_t size)const {
      return{ ptr_ - size };
    }

    const size_t operator-(const iterator& o)const {
      return  ptr_ - o.ptr_;
    }

    iterator& operator++() {
      ++ptr_;
      return *this;
    }

    iterator operator++(int) {
      auto tmp{ *this };
      ++* this;
      return tmp;
    }

    iterator& operator--() {
      --ptr_;
      return *this;
    }

    iterator operator--(int) {
      auto tmp{ *this };
      --* this;
      return tmp;
    }

    iterator& operator+=(size_t size)const {
      ptr_ += size;
      return *this;
    }

    iterator& operator-=(size_t size)const {
      ptr_ -= size;
      return *this;
    }

    const reference operator[](size_t size)const {
      return ptr_[size];
    }

    const bool operator==(const iterator& o)const {
      return ptr_ == o.ptr_;
    }

    bool operator!=(const iterator& o)const {
      return ptr_ != o.ptr_;
    }

    reference operator*()const {
      return *ptr_;
    }

    T* operator->()const {
      return ptr_;
    }

  };
  size_t n_elements_{};
  std::unique_ptr<T[]> c_{};

public:
  Container(std::initializer_list<T> l) :n_elements_{ l.size() }, c_{ std::make_unique<T[]>(n_elements_) }
  {
    for (size_t index{}; auto e:l)
    {
      c_[index++] = e;
    }
  }

  Container(size_t sz) :n_elements_{ sz }, c_{ std::make_unique<T[]>(n_elements_) } {}

  size_t size()const {
    return n_elements_;
  }

  const T& operator[](size_t index)const {
    return c_[index];
  }

  const T& at(size_t index)const {
    return index < n_elements_ ? c_[index] : throw std::out_of_range{ "Container::at(): index out of range" };
  }

  bool empty() const {
    return (n_elements_ == 0);
  }

  iterator begin()const { return { c_.get() }; }
  iterator end()const { return { c_.get() + n_elements_ }; }
};

template<typename T>
Container(std::initializer_list<T>l)->Container<T>;

struct XX {
  int x;
};

int main()
{
    // 4.3 create iterate range
  //Seq<int> v1{ 1,10 };
  //for (auto i : v1)
  //{
  //  print("{}", i);
  //}
  //print("\n");
  //X<int, 10> x{ 1,2,3,4,5,6,7,8,9,10 };
  //for (int i : x)
  //{
  //  print("{}", i);
  //}

  // 4.4 make stl iterator compatibel with self defined iterator, see Seq
  //Seq<int>r{ 100,110 };
  //auto [min_it, max_it] = std::ranges::minmax_element(r);

  //print("max:{} min:{}\n", max_it.value, min_it.value);
  //printc(r);
  //// using this to check if it is compatibel
  //static_assert(std::ranges::forward_range<Seq<int>>);

  // 4.5  front, back, insert
  //std::deque<int>d1{ 1,2,3,4,5 };
  //std::deque<int>d2(d1.size());
  //std::copy(d1.begin(), d1.end(), d2.begin());
  //printc(d2, "d2 after copy");

  //std::copy(d1.begin(), d1.end(), std::back_inserter(d2));
  //printc(d2, "d2 after back insret");

  //std::deque<int>d3{ 47,41,73,113,24 };
  //std::copy(d3.begin(), d3.end(), std::front_inserter(d2));
  //printc(d2, "d2 after front insret");

  //auto it2(d2.begin() + 2);
  //std::copy(d1.begin(), d1.end(), std::inserter(d2,it2));
  //printc(d2, "d2 after middle insret");

  // 4.6 create fib iterator generator
 /* printc(fib_generator(11));
  fib_generator fib(12);
  auto x = std::ranges::views::transform(fib, [](auto i) {return i * i; });
  printc(x, "square");
  return 0;*/

  // 4.7 reverse decorator and generator
  //int array[]{ 1,2,3,4,5 };
  //printc(array, "c-array");
  //auto it = std::begin(array);
  //auto it_end = std::end(array);
  //while (it != it_end)
  //{
  //  print("{} ", *it++);
  //}
  //print("\n");

  //auto it2 = std::rbegin(array);
  //auto it2_end = std::rend(array);
  //while (it2 != it2_end)
  //{
  //  print("{} ", *it2++);
  //}
  //print("\n");

  //printr(array, "rev c-aary");
  //std::vector<int>v{ 1,2,3,4,7 };
  //printc(v, "vec");
  //printr(v, "vec");

  // 4.8 using sentinel to iterate unknown length object
 /* const char carray[]{ "array" };
  print_cstr(carray);
  const char* cstr{ "c-string" };
  print_cstr(cstr);*/

  // 4.9 create zip iterator
  /*std::vector<std::string>vec_a{ "ss","aa","jj","hh" };
  std::vector<std::string>vec_b{ "11","33","55","88" };

  printc(vec_a, "vec_a");
  printc(vec_b, "vec_b");

  print("zipped: ");
  for (const auto& [a, b] : zip_iterator(vec_a, vec_b))
  {
    print("[{},{}]", a, b);
  }
  print("\n");

  std::map<std::string, std::string>name_maps;
  for (auto [a, b] : zip_iterator(vec_a, vec_b))
  {
    name_maps.emplace(a, b);
  }
  print(name_maps);*/

  // 4.10 create random iterator
  Container c{ 1,2,34,5,6 };
  for (const auto& i : c)
  {
    print("{}", i);
  }
  print("\n");

  Container<XX>c3{ {1},{2} };
  auto ret = c3.begin();
  ret->x;

  print("{}\n", std::ranges::forward_range<Container<int>>);
  print("{}\n", std::ranges::range<Container<int>>);
  print("{}\n", std::ranges::viewable_range<Container<int>>);
  print("{}\n", std::bidirectional_iterator<Container<int>>);

  for (const auto& i : c | std::views::reverse) {
    print("{} ", i);
  }
}

