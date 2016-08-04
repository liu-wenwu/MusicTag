#include "utils/iconv_utils.h"
#include <string.h>

namespace musictag{

	bool iconv_utils::convert(const char *from_charset, const  char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
	{
		iconv_t cd;
		size_t rc;
		const char **pin = &inbuf;
		char **pout = &outbuf;

		cd = iconv_open(to_charset, from_charset);
		if (cd == 0)
			return false;
		memset((void*)outbuf, 0, outlen);
		if (iconv(cd, pin, &inlen, (char **)pout, &outlen) == -1)
			return false;
		iconv_close(cd);
		return true;
	}


	bool iconv_utils::convert(const std::string &from_charset, const std::string &to_charset, const std::string &instr, std::string &outstr)
	{
		std::string form_last;
		
		if (instr.size() > 1 && (
			(from_charset == "UTF-16LE" && (unsigned char)instr[0] == 0xff && (unsigned char)instr[1] == 0xfe) ||
			(from_charset == "UTF-16BE" && (unsigned char)instr[0] == 0xfe && (unsigned char)instr[1] == 0xff)))
		{
		
			form_last = "UTF-16";
		}
		else{
			form_last = from_charset;
		}

		outstr.resize(1024);
		char *outptr = &outstr[0];
		bool ret = convert(form_last.c_str(), to_charset.c_str(),
			instr.c_str(), instr.size(), outptr, outstr.size());

		for (int i = 0; i < 1024; ++i)
		{
			if (outptr[i] == 0 && outptr[i + 1] == 0)
			{
				outstr.resize(i);
				break;
			}
		}

		return ret;

	}




}