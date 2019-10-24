#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <algorithm>

// -----------------------------------------------------------------
//
//                           USING
//
//------------------------------------------------------------------

using ip = std::array<uint8_t, 4>;
using ip_pool = std::vector<ip>;
using ip_pool_ci = ip_pool::const_iterator;

// -----------------------------------------------------------------
//
//                        DECLARATIONS
//
//------------------------------------------------------------------

ip get_ip(std::string& str);

void populate(std::istream& is, ip_pool& pool);

// format and output
void out(std::ostream& os, const ip& i);

void print(std::ostream& os, ip_pool_ci begin, ip_pool_ci end);

// a better-looking print call
void print(std::ostream& os, const ip_pool& pool);

// filter 1 (& clojure for filter 2)
void variadic_filter(std::ostream& os, ip_pool_ci begin, ip_pool_ci end, const int pos, const char byte);

// filter 2
template<typename... Args>
void variadic_filter(std::ostream& os, ip_pool_ci begin, ip_pool_ci end, const int pos, const char byte, Args... bytes);

// this fires both variadic filters
template<typename... Args>
inline void print_if_begins_with(std::ostream& os, const ip_pool& pool, const char byte, Args... bytes);

// filter 3 (lazy one)
void print_if_includes(std::ostream& os, const ip_pool& pool, const uint8_t byte);

// -----------------------------------------------------------------
//
//                          TEST AREA
//
//------------------------------------------------------------------

// all-in-one test function (and this one is rather FAT and UGLY)
// outputs main() to test_file.tst and compares it to ip_filter.tst
// will return TRUE if both files are std::hash<std::string>-EQUAL

bool output_hash_is_equal(); // no parameters; should be an independent module

