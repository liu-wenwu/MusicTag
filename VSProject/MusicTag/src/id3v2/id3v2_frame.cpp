#include "id3v2/id3v2_frame.h"
#include "iconv_utils.h"

namespace musictag{

	void id3v2_frame::get_frame_size(unsigned int size, unsigned char sizestr[4])
	{
		sizestr[0] = (size >> 24) & 0xff;
		sizestr[1] = (size >> 16) & 0xff;
		sizestr[2] = (size >> 8) & 0xff;
		sizestr[3] = (size >> 0) & 0xff;
	}

	std::string id3v2_frame::get_string_of_codec(char c)
	{
		if (c == ID3V2_ISO88591)
			return "ISO-8859-1";
		else if (c == ID3V2_UTF16BE)
			return "UTF-16BE";
		else if (c == ID3V2_UTF16LE)
			return "UTF-16LE";
		else if (c == ID3V2_UTF8)
			return "UTF-8";
		else throw std::exception("can't get codec");
	}

	std::string id3v2_frame::get_id_string(id3v2_id id)
	{
		std::string str;
		str.resize(4);

		str[0] = (id >> 24) & 0xff;
		str[1] = (id >> 16) & 0xff;
		str[2] = (id >> 8) & 0xff;
		str[3] = (id >> 0) & 0xff;

		return str;

	}


	std::string id3v2_frame::get_string_by_codec(const char *data, int len, int codec)
	{
		std::string str;
		switch (codec)
		{
		case ID3V2_ISO88591:

			str = std::string(data, len);
			break;
		case ID3V2_UTF16LE:
		{
							  unsigned short flag = *(unsigned short*)data;

							  if (flag == 0xfeff)
								  iconv_utils::convert("UTF-16", "GB2312", std::string(data, len), str);
							  else
								  iconv_utils::convert("UTF-16LE", "GB2312", std::string(data, len), str);

							  break;
		}
		case ID3V2_UTF16BE:
		{
							  unsigned short flag = *(unsigned short*)data;

							  if (flag == 0xfffe)
								  iconv_utils::convert("UTF-16", "GB2312", std::string(data, len), str);
							  else
								  iconv_utils::convert("UTF-16BE", "GB2312", std::string(data, len), str);
							  break;
		}

		case ID3V2_UTF8:
			iconv_utils::convert("UYF-8", "GB2312", std::string(data, len), str);
			break;
		default:
			break;
		}
		return str;
	}
}

