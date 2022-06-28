#pragma once

#include "common-include.hpp"

struct RecursionInfo
{
	bool isRecursive {true};
	bool isDepthInfinite {false};
	std::size_t depth {0};

	constexpr RecursionInfo() = default;
	constexpr RecursionInfo(RecursionInfo const& copy) = default;
	constexpr ~RecursionInfo() = default;

	// about this->isRecursive
	constexpr RecursionInfo& makeRecursive();
	constexpr RecursionInfo& makeFlat();
	constexpr RecursionInfo& setRecursivity(bool isNowRecursive);

	// about this->isDepthInfinite
	constexpr RecursionInfo& makeInfinite();
	constexpr RecursionInfo& makeFinite();
	constexpr RecursionInfo& setInfinity(bool isNowInfinite);

	// about this->depth
	constexpr RecursionInfo& setDepth(std::size_t newDepth);
	constexpr RecursionInfo& increaseDepth(std::size_t increaseBy);
	constexpr RecursionInfo& reduceDepth(std::size_t reduceBy);
	constexpr std::size_t depthOr(std::size_t value) const;

	// makes a copy
	constexpr RecursionInfo getNextLayer() const;
};

constexpr RecursionInfo& RecursionInfo::makeRecursive()
{
	this->isRecursive = true;
	return *this;
}

constexpr RecursionInfo& RecursionInfo::makeFlat()
{
	this->isRecursive = false;
	return *this;
}

constexpr RecursionInfo& RecursionInfo::setRecursivity(bool isNowRecursive)
{
	this->isRecursive = isNowRecursive;
	return *this;
}

constexpr RecursionInfo& RecursionInfo::makeInfinite()
{
	this->isDepthInfinite = true;
	return *this;
}

constexpr RecursionInfo& RecursionInfo::makeFinite()
{
	this->isDepthInfinite = false;
	return *this;
}

constexpr RecursionInfo& RecursionInfo::setInfinity(bool isNowInfinite)
{
	this->isDepthInfinite = isNowInfinite;
	return *this;
}

constexpr RecursionInfo& RecursionInfo::setDepth(std::size_t newDepth)
{
	this->depth = newDepth;
	return *this;
}

constexpr RecursionInfo& RecursionInfo::increaseDepth(std::size_t increaseBy)
{
	// doesn't conduct overflow checks because eh
	this->depth += increaseBy;
	return *this;
}

constexpr RecursionInfo& RecursionInfo::reduceDepth(std::size_t reduceBy)
{
	if (reduceBy > this->depth)
		this->depth = 0;
	else
		this->depth -= reduceBy;
	return *this;
}

constexpr std::size_t RecursionInfo::depthOr(std::size_t value) const
{
	if (this->isRecursive && !this->isDepthInfinite)
		return this->depth;
	return value; 
}

constexpr RecursionInfo RecursionInfo::getNextLayer() const
{
	return RecursionInfo(*this).reduceDepth(1);
}
