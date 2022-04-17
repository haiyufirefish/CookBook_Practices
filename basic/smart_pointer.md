### smart pointer
1. **never using naked pointer**
Construtor, destructor, Resource Acquisition Is Initialization(RAII)
C++ 14: make_unique(): implmentation in C++ 11:
```
template<class T, class... Args>              // variadic template
std::unique_ptr<T>                            // return smart pointer
my_make_unique(Args&&... args)                // parameters for variadic templates
{
    return std::unique_ptr<T>(                // construct unique_ptr
            new T(std::forward<Args>(args)...));    // perfect forward.
}
```

2. unique_ptr ownership, only one pointer can have it. using move to transfer it.
```
auto ptr1 = make_unique<int>(42);    // create a unique_ptr
assert(ptr1 && *ptr1 == 42);         // this time pointer valid

auto ptr2 = std::move(ptr1);         // using move to change ownership
assert(!ptr1 && ptr2);               // ptr1 is empty pointer
```

3. shared_ptr: ownership can be shared safely.
```
auto ptr1 = make_shared<int>(42);    // create pointer
assert(ptr1 && ptr1.unique() );     // unique pointer

auto ptr2 = ptr1;                  // copy assignment
assert(ptr1 && ptr2);              // two poiners are valid

assert(ptr1 == ptr2);             // shared_ptr can be compared

// count is 2
assert(!ptr1.unique() && ptr1.use_count() == 2); 
assert(!ptr2.unique() && ptr2.use_count() == 2); 
```
4. using shared_ptr:\
**Be especially careful with object destructors, don't have very complex, heavily blocking operations**\\
```
class DemoShared final      //   
{
public:
    DemoShared() = default;
   ~DemoShared()            // complex operations make shared_ptr destrution error
   {
       // Stop The World ...
   }
};
```
5. Another problem is **circular reference**:\
```

class Node final
{
public:
    using this_type     = Node;
    using shared_type   = std::shared_ptr<this_type>;
public:
    shared_type     next;      // using shared_ptr to point next node
};

auto n1 = make_shared<Node>();   // 
auto n2 = make_shared<Node>();   // 

assert(n1.use_count() == 1);    // count is 1
assert(n2.use_count() == 1);

n1->next = n2;                 // circular reference
n2->next = n1;

assert(n1.use_count() == 2);    // count is 2
assert(n2.use_count() == 2);    // make memory leak
```

6. weak_prt is to address this problem:\
```
// no ownership, temporary pointer 
class Node final
{
public:
    using this_type     = Node;

    // weak_ptr
    using shared_type   = std::weak_ptr<this_type>;
public:
    shared_type     next;    // using alias
};

auto n1 = make_shared<Node>();  
auto n2 = make_shared<Node>();  

n1->next = n2;             // point to each other, circular reference
n2->next = n1;

assert(n1.use_count() == 1);    // weak_ptr，count is 1
assert(n2.use_count() == 1);   // avoid memory leak

if (!n1->next.expired()) {     // check if pointer is valid
    auto ptr = n1->next.lock();  // lock()get shared_ptr 
    assert(ptr == n2);
}
//or 
if(ptr = next.lock()){
	prt->dosomething
}
```

7. Tradeoff and summary:\
Unique_ptr is used more by default. On the contrary, shared_ptr is not used much,\
because it is worried about the circular reference, resource consumption, thread \
safety and other issues mentioned in the article. Most of the time, unique_ptr can \
completely replace raw pointers. If it's pure standard C++ code, it's really comfortable\
to use smart pointers, just treat them as a normal type. However, at the same time,\
as C++ programmers, we have to deal with raw pointers. Whether it is Linux or Windows,\
we inevitably have to use their system api, so we have to use get to convert smart \
pointers into raw pointers. And the memory pointed to by this pointer may not be arbitrarily \
destructed in the system api, because the memory may continue to be used within the system, \
so in this scenario, the characteristics of smart pointers may be a disservice. In response \
to this kind of problem, what I think of is to extend the life cycle of this smart pointer, \
or directly use release to release the raw pointer, but in this way, delete is indispensable.\
**RAII to manage, or use shared_ptr's custom delete function**

