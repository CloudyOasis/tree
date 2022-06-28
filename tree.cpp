#include "common-include.hpp"

int main(int argc, char **argv)
{
	// settings
	bool showFiles {false};
	auto recursivity { RecursionInfo().makeRecursive() };

	if (argc <= 1) recursivity.makeInfinite();
	else recursivity.makeFinite()
			.setDepth(std::atoi(argv[1]));
	
	tree(fs::current_path(), recursivity);
}
