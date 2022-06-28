#pragma once

#include "common-include.hpp"

/**
 * @brief Returns a vector of directory items, sorted by name (by default). Not recursive.
 * 
 * @tparam Comparator Comparator type.
 * @param path Path to ls in.
 * @param compare Compare function.
 * @return std::vector<fs::directory_entry> Vector of the items in the directory, sorted by name.
 */
template <typename Comparator = std::less<> >
std::vector<fs::directory_entry> ls
[[nodiscard]] // ls-ing might be slow depending on the system and directory
(
	fs::path const& path,
	Comparator compare = Comparator {}
)
{
	std::vector<fs::directory_entry> contents;

	for (auto const& i : fs::directory_iterator(path))
	{
		contents.push_back(i);
	}

	std::ranges::sort(contents, compare);

	return contents;
}

/**
 * @brief like the tree command, prints to stdout.
 * 
 * @tparam Comparator Comparator type.
 * @param path Path to tree.
 * @param recursivity Recursivity information.
 * @param compare Compare function. Sorts by path by default.
 * @param displayDepth Intended for use by this function only ; used to indent output.
 */
template <typename Comparator = std::less<> >
void tree(
	fs::path const& path,
	RecursionInfo const& recursivity,
	Comparator compare = Comparator {},
	std::optional<std::vector<bool>> displayDepth = std::nullopt // see next line
		// using an optional parameter as non-const references can't take default values
		// uninitialised on root call
)
{
	// if this is a root call
	if (!displayDepth.has_value())
	{
		// then initialise displayDepth and print the root call directory
		displayDepth = std::vector<bool>();
		std::cout << path.string() << "\n";
	}
	else // if not a root call
	{
		if (!recursivity.isDepthInfinite
			&& displayDepth->size() > recursivity.depth)
			return;
	}

	auto const folderItems = ls(path, compare);
	for (auto const& item : folderItems)
	{
		// iterate through the display depth vector
		for (bool const level : displayDepth.value())
		{
			if (level) std::cout << "│   ";
			else std::cout << "    ";
		}

		displayDepth->push_back(false);

		if (item == folderItems.back())
		{
			std::cout << "└── ";
			*displayDepth->rbegin() = false;
		}
		else
		{
			std::cout << "├── ";
			*displayDepth->rbegin() = true;
		}

		std::cout << item.path().filename().string()
			<< "\n";

		if (item.is_directory())
		{
			tree(item.path(),
				recursivity,
				compare,
				displayDepth
				);
		}
	
		displayDepth->pop_back();
	}

	return;
}
