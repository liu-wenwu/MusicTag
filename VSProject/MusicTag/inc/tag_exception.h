#ifndef __TAG_EXCEPTION__
#define __TAG_EXCEPTION__
#include <exception>

namespace musictag{

	class tag_exception : public std::exception
	{
		tag_exception(const char * const & what) :std::exception(what)
		{}
	};







}






#endif