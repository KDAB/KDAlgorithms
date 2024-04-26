# Version 1.3 released.
* Introduce algorithms sum and sum_if
* Do not require operator< if not needed in remove_duplicates(3 months ago)

# Version 1.2 released
* Added new algorithm multi_transform
* compile fix: void_t isn't available before C++17
* Call std::invoke if we have it (#58)
* renamed the bits subdirectory to kdabalgorithms_bits
* Improved sort_by (#57)

# Version 1.1 released
* algorithms sort_by and sorted_by.
* Remove std::move() on temporaries, as detected by gcc 13 (#55)
* Fix issue #50 - find_if creates dangling references
* Ensure min_value and max_value works on containers without .empty() method.
* Renamed min_element to min_value, and max_element to max_value (see issue #11)

# 2023-02-27 Version 1.0.0 released
