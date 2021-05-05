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
- priority_queue (to learn heaps)

### Run tests
```clang++ main.cpp -std=c++98 -Wall -Wextra -Werror && ./a.out```
