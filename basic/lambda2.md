### lambda attentions
2. [=] and [&], external capturation. copy construction and reference construction.
```
int x = 33;               // external variable

auto f1 = [=]()           // 
{
    //x += 10;            // x readable, cannot change
};

auto f2 = [&]()         // lambda expression
{
    x += 10;            // x reference, changeable
};

auto f3 = [=, &x]()       // only x reference
{
    x += 20;              // x can be modified. Others not
};
```
Be careful when using the capture function. For small lambda expressions used "in-place",\
you can use "[&]" to reduce the amount of code and keep it clean; for non-local calls and\
long life cycle lambda expressions should be used. Be careful to use "[&]" to capture \
references, and it is better to explicitly write the variable list in "[]" to avoid\
capturing unnecessary variables.

```
class DemoLambda final
{
private:
    int x = 0;
public:
    auto print()              // return a lambda expression for external usage
    {
        return [this]()      // get the pointer explicitly
        {
            cout << "member = " << x << endl;
        };
    }
};
```

3. Generization since **C++14**
```
auto f = [](const auto& x)        // using auto declare input 
{
    return x + x;
};

cout << f(3) << endl;             // paramater type is int
cout << f(0.618) << endl;         // paramater type is double

string str = "matrix";
cout << f(str) << endl;          // paramater type is string
```
The "map+lambda" approach to replace the hard-to-maintain if/else/switch\
is much more readable than a large number of branch statements. 
```
map<int, function<void()>> funcs;
funcs[1] = [](){...};
funcs[7] = [](){...};
funcs[42] = [](){...};

return funcs[x]();
```
As the type of each lambda expression is unique, lambda expressions cannot assign 
to each other, using **std::function**: smart pointer of function.\
The form of lambda expressions is very concise and can replace ordinary \
functions in many places. But it cannot replace the member functions of classes:\
** It cannot define auto inside a class**\
lambda is overloaded by the operator of the class,\
before it, the most used closure is to package the task and push the task to the thread pool or the next process.