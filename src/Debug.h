/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#pragma once

#include <cassert>
#include <SFML/System.hpp>

/// print error message with file and line number
#define FF_ERROR_MSG(Text) \
{ \
	sf::err() <<  __FILE__ << " Line: " << __LINE__ << " " << Text << std::endl; \
}

/// print error message and assert
#ifdef _DEBUG
#define FF_ASSERT_MSG(Expression, Text) \
{ \
	if(!(Expression)) \
	{ \
		sf::err() <<  __FILE__ << " Line: " << __LINE__ << " " << Text << std::endl; \
		assert(Expression); \
	} \
}
#else
#define FF_ASSERT_MSG(Expression, Text) 
#endif
