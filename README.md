# ft_containers
42 ft_containers project about some STL containers from C++98

Works on Arch Linux and macOS. Insanity tests won't work on Windows because there's no `clock_gettime`

For Windows 10/Visual Studio 2019 port check `vsport` branch

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

Additionally:
- distance, make_heap, push_heap, pop_heap, sort_heap algorithms
- priority_queue

### Run tests
```clang++ main.cpp -std=c++98 -Wall -Wextra -Werror && ./a.out```

> compile with `-DSKIPINSANITY` to run only basic tests without lots of data tests
