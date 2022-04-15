### Abstraction and encapsulation
1. Minimize the use of inheritance and virtual functions when designing classes:\
The difference between public/protected/private inheritance methods, multiple inheritance, \
pure virtual interface classes, virtual destructors, and many dangerous traps such as dynamic\
transformation, object slicing, and function overloading can be bypassed, redundant code can \
be reduced, and code performance can be improved. robustness.
**If the inheritance depth exceeds three levels, it means that it is a bit "over-engineered", \
and you need to consider replacing the inheritance relationship \
with a composition relationship, or use templates and generics instead.**\

2. using final to forbid extend:
```
class DemoClass final    
{ ... };
```
3. When using inheritance, it is recommended to use only public inheritance and avoid using virtual and protected.\
```

class Interface        // this interface can be implemented
{ ... };           

class Implement final : // implementation avoid inheritance
      public Interface    // public extend
{ ... };
```

4. **six constructors in C++**:Constructor, Destructor, Copy Constructor, Copy Assignment, Transfer Constructor, and Transfer Assignment
make constructors and destructors with keyword **default**
```
class DemoClass final 
{
public:
    DemoClass() = default;  // 
   ~DemoClass() = default;  // 
};
```
Prevent external calling: using keyword **delete**
```
class DemoClass final 
{
public:
    DemoClass(const DemoClass&) = delete;              // forbid copy constructor
    DemoClass& operator=(const DemoClass&) = delete;  // forbid copy assignment
};
```
To avoid type conversion accidently: using **explicit**
```
class DemoClass final 
{
public:
    explicit DemoClass(const string_type& str)  //
    { ... }

    explicit operator bool()                  // 
    { ... }
};
```