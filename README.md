# ft_containers
42 ft_containers project about some STL containers from C++98

Works on Windows 10 (Visual Studio 2019) with ported `clock_gettime` and `rand` replacement ([RAND_MAX is `0x7fff` on Windows](https://docs.microsoft.com/en-us/cpp/c-runtime-library/rand-max?view=msvc-160))

What's done:
- list
- vector
- map
- stack
- queue
- allocator
- reverse_iterator
- iterator_traits and some type_traits adaptations
- pair and some relational operator templates

### Run tests
```clang++ main.cpp -std=c++98 -Wall -Wextra -Werror && ./a.out```
