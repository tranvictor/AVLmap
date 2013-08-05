AVLmap
======

This is a C++ map using AVL tree instead of red-black tree.
# Introduction

Maps are associative containers that store elements formed by a combination of a key value and a mapped value, following a specific order.

In a map, the key values are generally used to sort and uniquely identify the elements, while the mapped values store the content associated to this key. The types of key and mapped value may differ, and are grouped together in member type value_type, which is a pair type combining both:

``` 
typedef pair<const Key, T> value_type;
```

Internally, the elements in a map are always sorted by its key following a specific strict weak ordering criterion indicated by its internal comparison object (of type Compare).

map containers are generally slower than unordered_map containers to access individual elements by their key, but they allow the direct iteration on subsets based on their order.

Maps are typically implemented as binary search trees. AVLmap is implemented as AVL binary search tree.

A unique feature of map among associative containers is that they implement the direct access *operator (operator[])*, which allows for direct access of the mapped value.

# Member functions

1. **(constructor)**: Construct map
2. **(destructor)** : Map destructor
3. **operator=**    : Copy container content

## Iterator
1. **begin**        : Return iterator to beginning
2. **end**          : Return iterator to end 
3. **rbegin**       : Return reverse iterator to reverse beginning
4. **rend**         : Return reverse iterator to reverse end 

## Capacity
1. **empty**        : Test whether container is empty
2. **size**         : Return container size

## Element access
1. **operator[]**
2. **at**

## Modifiers
1. **insert**       : Insert element 
2. **erase**        : Erase element  
3. **swap**         : Swap content
4. **clear**        : Clear content

## Observers
1. **key_comp**     : Return key comparation object
2. **value_comp**   : Return value comparation object

## Operations
1. **find**         : Get iterator to element
2. **count**        : Count elements with specific key
3. **lower_bound**  : Return iterator to lower bound
4. **upper_bound**  : Return iterator to upper bound
5. **equal_range**  : Get range of equal elements

## Allocator
1. **get_allocator**: Get allocator

# Installation
Getting to use this AVLmap is simple, you just have to copy the header file from ```avlmap/avlmap.h``` to your header folder. That's it!

# Testing
A very simple test is provided in ```avlmap/main.cpp```. I planned to write more tests with higher coverage using gtest. If anyone wants to write tests, please dont hesitate to make a pull request.

# License
The MIT License (MIT)

Copyright (c) 2013 Victor Tran

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
