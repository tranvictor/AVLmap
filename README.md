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

A unique feature of map among associative containers is that they implement the direct access operator (operator[]), which allows for direct access of the mapped value.
