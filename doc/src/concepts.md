Basic Concepts
==============

FunctionObject
--------------

Is an object with a `const` call operator:

```cpp
concept FunctionObject
{
    template<class... Ts>
    auto operator()(Ts&&...) const;
};
```

##### Requirements:

The type `F` satisfies `FunctionObject` if

* The type `F` satisfies `std::is_object`, and 

Given

* `f`, an object of type `const F`
* `args...`, suitable argument list, which may be empty 

| Expression | Requirements             |
|------------|--------------------------|
| f(args...) | performs a function call |


UnaryFunctionObject
-------------------

Is an object with a `const` call operator that accepts one parameter:

```cpp
concept UnaryFunctionObject
{
    template<class T>
    auto operator()(T&&) const;
};
```

##### Requirements:

* `FunctionObject`

Given

* `f`, an object of type `const F`
* `arg`, a single argument

| Expression | Requirements             |
|------------|--------------------------|
| f(arg)     | performs a function call |

BinaryFunctionObject
--------------------

Is an object with a `const` call operator that accepts two parameter:

```cpp
concept UnaryFunctionObject
{
    template<class T, class U>
    auto operator()(T&&, U&&) const;
};
```

##### Requirements:

* `FunctionObject`

Given

* `f`, an object of type `const F`
* `arg1`, a single argument
* `arg2`, a single argument

| Expression    | Requirements             |
|---------------|--------------------------|
| f(arg1, arg2) | performs a function call |

MutableFunctionObject
---------------------

Is an object with a `mutable` call operator:

```cpp
concept FunctionObject
{
    template<class... Ts>
    auto operator()(Ts&&...);
};
```

##### Requirements:

The type `F` satisfies `FunctionObject` if

* The type `F` satisfies `std::is_object`, and 

Given

* `f`, an object of type `F`
* `args...`, suitable argument list, which may be empty 

| Expression | Requirements             |
|------------|--------------------------|
| f(args...) | performs a function call |

