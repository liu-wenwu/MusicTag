#ifndef __ICONV_UTILS_H__
#define __ICONV_UTILS_H__


#include <string>
#include "iconv.h"



namespace musictag
{




	class iconv_utils
	{
	public:
		static bool convert(const char *from_charset, const char *to_charset, const  char *inbuf, size_t inlen,  char *outbuf, size_t outlen);

		static bool convert(const std::string &from_charset,const std::string &to_charset,const std::string &instr, std::string &outstr);


	};





}


#endif