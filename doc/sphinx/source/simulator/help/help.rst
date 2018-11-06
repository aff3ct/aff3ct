Help
====

Contact
-------

You can contact us on github by opening an issue.



Known issues
------------

Eclipse IDE
^^^^^^^^^^^

You may encounter an issue with Eclipse IDE, which doesn't handle C++11 as a
default behavior. To solve it you have to add the followings.

In Eclipse's symbols (*C/C++ General > Path and Symbols > Symbols*), add::

   GXX_EXPERIMENTAL_CXX0X (or __GXX_EXPERIMENTAL_CXX0X__)

In Compiler Options (*C/C++ General > Preprocessor Include Paths, Macros etc. > Providers > DCT GCC Built-in Compiler Settings > Commands to get compiler specs*):

   * add **-std=c++11** (or **-std=c++1y**).
   * uncheck **"Use global provider shared between projects"**: