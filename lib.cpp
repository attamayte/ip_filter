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
void out(std::ostream& os, const ip& i)
{
	os
		<< +i[0] << '.'
		<< +i[1] << '.'
		<< +i[2] << '.'
		<< +i[3] << '\n';
}

void print(std::ostream& os, ip_pool_ci begin, ip_pool_ci end)
{
	while (begin != end)
		out(os, *begin++);
}

// a better-looking print call
void print(std::ostream& os, const ip_pool& pool)
{
	auto begin = pool.begin(), end = pool.end();
	print(os, begin, end);
}

// variadic filter (filter 1 + filter 2)
template<typename... Args>
void print_if_begins_with(std::ostream& os, const ip_pool& pool, const uint8_t byte, Args... bytes)
{
	std::pair<ip_pool_ci, ip_pool_ci> r{ pool.begin(), pool.end() };
	ip target{ byte, (uint8_t)bytes... };

	for (size_t pos = 0; pos < (1 + sizeof...(bytes)); ++pos)
		r = std::equal_range(r.first, r.second, target, [=](const ip& a, const ip& b) {return a[pos] > b[pos]; });

	print(os, r.first, r.second);
}

// filter 3 (REWORKED)
void print_if_includes(std::ostream& os, const ip_pool& pool, const uint8_t target)
{
	for (const auto& p : pool)
		if (std::any_of(p.begin(), p.end(), [=](uint8_t mybyte) {return mybyte == target; }))
			out(os, p);
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
	std::fstream fs("ip_filter.tsv", std::fstream::in);
	// replicating main() behavior
	ip_pool test;
	populate(fs, test);
	test.shrink_to_fit();
	
	fs.close(); fs.open("test_file.tst", std::fstream::out);

	std::sort(test.begin(), test.end(), std::greater<ip>());
	
	print(fs, test);
	print_if_begins_with(fs, test, 1);
	print_if_begins_with(fs, test, 46, 70);
	print_if_includes(fs, test, 46);

	fs.close(); fs.open("test_file.tst", std::fstream::in);

	std::string test_file; // container for test file
	for (std::string line; std::getline(fs, line); )
		test_file += line;

	fs.close(); fs.open("ip_filter.tst", std::fstream::in);

	std::string ref_file; // container for reference file
	for (std::string line; std::getline(fs, line); )
		ref_file += line;

	fs.close();

	std::hash<std::string> hash;
	return (hash(test_file) == hash(ref_file));
}