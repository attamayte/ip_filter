#include "lib.h"

// -----------------------------------------------------------------
//
//                        DEFINITIONS
//
//------------------------------------------------------------------

ip get_ip(std::string& str)
{
	std::string::size_type sz;
	ip temp;
	temp[0] = std::stoi(str, &sz);
	temp[1] = std::stoi(str.erase(0, sz + 1), &sz);
	temp[2] = std::stoi(str.erase(0, sz + 1), &sz);
	temp[3] = std::stoi(str.erase(0, sz + 1));
	return temp;
}

void populate(std::istream& is, ip_pool& pool)
{
	for (std::string buffer; std::getline(is, buffer);)
		pool.push_back(get_ip(buffer));
}

// format and output
inline void out(std::ostream& os, const ip& i)
{
	os
		<< +i[0] << '.'
		<< +i[1] << '.'
		<< +i[2] << '.'
		<< +i[3] << '\n';
}

inline void print(std::ostream& os, ip_pool_ci begin, ip_pool_ci end)
{
	while (begin != end)
		out(os, *begin++);
}

// a better-looking print call
inline void print(std::ostream& os, const ip_pool& pool)
{
	auto begin = pool.begin(), end = pool.end();
	print(os, begin, end);
}

// filter 1 (& clojure for filter 2)
void variadic_filter(std::ostream& os, ip_pool_ci begin, ip_pool_ci end, const int pos, const char byte)
{
	ip target; target[pos] = byte; // temporary object for comparison
	auto r = std::equal_range(begin, end, target,
		[=](const ip& a, const ip& b) {return a[pos] > b[pos]; }); // using > because the container is sorted in reverse order
	print(os, r.first, r.second);
}

// filter 2
template<typename... Args>
void variadic_filter(std::ostream& os, ip_pool_ci begin, ip_pool_ci end, const int pos, const char byte, Args... bytes)
{
	ip target; target[pos] = byte; // temporary object for comparison
	auto r = std::equal_range(begin, end, target,
		[=](const ip& a, const ip& b) {return a[pos] > b[pos]; }); // using > because the container is sorted in reverse order
	variadic_filter(os, r.first, r.second, pos + 1, bytes...);
}

// this fires both variadic filters
template<typename... Args>
inline void print_if_begins_with(std::ostream& os, const ip_pool& pool, const char byte, Args... bytes)
{
	const int pos = 0; // wrapping up the searching position;
	// (!) must be incremented after each variadic iteration (!)
	variadic_filter(os, pool.begin(), pool.end(), pos, byte, bytes...);
}

// filter 3 (lazy one)
void print_if_includes(std::ostream& os, const ip_pool& pool, const uint8_t byte)
{
	std::for_each(pool.begin(), pool.end(),
		[&os, byte] (const ip& a) {if (a[0] == byte || a[1] == byte || a[2] == byte || a[3] == byte) out(os, a);} );
}

// -----------------------------------------------------------------
//
//                          TEST AREA
//
//------------------------------------------------------------------

// all-in-one test function (and this one is rather FAT and UGLY)
// outputs main() to test_file.tst and compares it to ip_filter.tst
// will return TRUE if both files are std::hash<std::string>-EQUAL

bool output_hash_is_equal() // no parameters; should be an independent module
{
	std::fstream fs("ip_filter.tsv", std::fstream::in); // fin mode
	// replicating main() behavior
	ip_pool test;
	populate(fs, test);
	
	fs.close(); fs.open("test_file.tst", std::fstream::out); // fout mode
	
	test.shrink_to_fit();
	std::sort(test.rbegin(), test.rend());
	print(fs, test);
	print_if_begins_with(fs, test, 1);
	print_if_begins_with(fs, test, 46, 70);
	print_if_includes(fs, test, 46);

	fs.close(); fs.open("test_file.tst", std::fstream::in); // fin mode
	
	std::string test_file; // container for test file
	for (std::string line; std::getline(fs, line); )
		test_file += line;
	
	fs.close(); fs.open("ip_filter.tst", std::fstream::in); // fin mode
	
	std::string ref_file; // container for reference file
	for (std::string line; std::getline(fs, line); )
		ref_file += line;
	
	fs.close();

	std::hash<std::string> hash; 
	return (hash(test_file) == hash(ref_file));
}