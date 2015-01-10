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

