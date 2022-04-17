### Function object in CPP
In C/C++, all functions are global, and there is no concept of life cycle \
(static and namespace are very weak, but simply limit the scope of application\
and avoid name conflicts.\
```
// call a function
void my_square(int x)           // definition
{
    cout << x*x << endl;       // impl
}

auto pfunc = &my_square;       // using pointer to operate function
(*pfunc)(3);                    // call it using *
pfunc(3);                       // or direct call pointer``
```
Lambda format:\
```
\\ auto xxx = [](){};
auto func = [](int x)          // definition
{   
    cout << x*x << endl;      // lambda expression
};  
func(3);                      // call 
```
Because a lambda expression is a variable, we can "assign on demand", define\
a function "in-place" at the call site anytime, anywhere, limit its scope and\
life cycle, and achieve function localization.\
In addition to being called like ordinary functions, lambda expressions in C++ \
have a special ability that ordinary functions do not have, that is, they can "capture"\
external variables and operate directly in the internal code.\
```

int n = 10;                     // external variable

auto func = [=](int x)          // lambda using "=" to capture
{
    cout << x*n << endl;        
};

func(3);                    // call
```
It is so called **closure** in Javasript. Because the external variables captured at the time\
of definition are saved, you can jump away from the definition point and "package" this code\
and pass it to other places for execution, which cannot be done with the entry parameters of the function alone.

### Attentions using lambda
1. good indentation:
```
auto f2 = []()                 // lambda expr 1
{
    cout << "lambda f2" << endl;

    auto f3 = [](int x)         // Nested definition of lambda expressions 2
    {
        return x*x;
    };// lambda f3              // comment it as it ends

    cout << f3(10) << endl;
};  // lambda f2               // comment it as it ends
```
This not only makes the code more concise, but also because of "anonymity", the lambda expression\
does not exist after it is called (and may be copied and saved), which minimizes its scope of\
influence and makes the code more secure.

```
vector<int> v = {3, 1, 8, 5, 0};     // basic vector

cout << *find_if(begin(v), end(v),   // find_if algorithm
            [](int x)                // anonymity lambda, doesn't need auto
            {
                return x >= 5;        // condition
            }                        // lambda end
        )
     << endl;                        // expression doesn't exist 
```	 

 