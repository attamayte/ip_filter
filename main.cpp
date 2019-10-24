#include "lib.h"

int main()
{
	ip_pool pool; // container
	populate(std::cin, pool); // fills the container with quad-bytes using the IP scheme X.X.X.X, where X = [0-255]
	pool.shrink_to_fit();

	// Reverse lexicographical sorting
	std::sort(pool.rbegin(), pool.rend());

	// all print_ functions include the std::ostream& parameter;
	// it makes code reusable for the testing (file input/output)
	// I don't like the need to insert an additional argument;
	// will look for a more elegant way to bind the test later
	
	using std::cout;
	print(cout, pool);
	print_if_begins_with(cout, pool, 1); // Filter 1 -> O(2*log)
	print_if_begins_with(cout, pool, 46, 70); // Filter 2 -> O(2*log)
	print_if_includes(cout, pool, 46); // Filter 3 -> O(4n)

	return 0;
}