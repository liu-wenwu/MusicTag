#include "id3v2/id3v2_text_frame.h"
#include "iconv_utils.h"


namespace musictag{



	void id3v2_text_frame::parse(const std::vector<char> &data)
	{
		text = get_string_by_codec(&data[1], data.size() - 1, data[0]);
		encode();
	}

	void id3v2_text_frame::write(std::ostream &os)
	{
		std::cout << get_id_string(tid) << ":" << text << std::endl;
		id3v2_raw_frame frame(tid);
		std::string outstr;
		iconv_utils::convert("GB2312", "UTF-16LE", text, outstr);
		outstr += "\0\0";

		get_frame_size(size(), frame.Size);

		os.write((char*)&frame, 10);
		unsigned char codec = 1;

		os.write((char*)&codec, 1);

		os.write(&outstr[0], outstr.size());

	}


	void id3v2_text_frame::set_text(const std::string &str)
	{
		text = str;
		encode();
	}

	int id3v2_text_frame::size()
	{
		return  10 + encode_text.size() + 1;
	}

	void id3v2_text_frame::encode()
	{
		if (!text.empty() && codec != ID3V2_ISO88591)
			iconv_utils::convert(get_string_of_codec(ID3V2_ISO88591), get_string_of_codec(codec), text, encode_text);
		else
			encode_text = text;
	}

}

