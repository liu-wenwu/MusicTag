#include "flac/flac_vorbis_comment.h"
#include "utils/iconv_utils.h"
#include <algorithm>

namespace musictag{


	std::shared_ptr<flac_vorbis_comment> flac_vorbis_comment::create(std::istream &is)
	{
		char size[4];

		is.read(size, 4);
		unsigned int elem_size = *(unsigned int*)(size);
		char *elem_str = new char[elem_size];
		is.read(elem_str, elem_size);
		std::string elem(elem_str, elem_size);
		std::string elem_dec;
		iconv_utils::convert("UTF-8", "GB2312", elem, elem_dec);
		
		std::string::const_iterator iter = std::find(elem_dec.begin(), elem_dec.end(), '=');
		if (iter == elem_dec.end())
			return nullptr;
		std::string key(elem_dec.begin(), iter);
		std::string value(iter + 1, elem_dec.end());

		std::transform(key.begin(), key.end(), key.begin(), toupper);

		std::shared_ptr<flac_vorbis_comment> flac_ptr = std::make_shared<flac_vorbis_comment>(key, value);
		return flac_ptr;

	}


	void flac_vorbis_comment::write(std::ostream &os)
	{

	}







}


