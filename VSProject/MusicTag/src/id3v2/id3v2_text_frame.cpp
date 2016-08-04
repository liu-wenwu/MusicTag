#include "id3v2/id3v2_text_frame.h"
#include "utils/iconv_utils.h"


namespace musictag{



	void id3v2_text_frame::parse(const std::vector<char> &data)
	{
		text = get_string_by_codec(&data[1], data.size() - 1, data[0]);

		encode();
	}

	void id3v2_text_frame::write(std::ostream &os)
	{
		if (encode_text.empty())
			return;

		id3v2_raw_frame frame(tid);


		get_frame_size(size(), frame.Size);

		os.write((char*)&frame, 10);
		unsigned char codec = 1;

		os.write((char*)&codec, 1);

		os.write(&encode_text[0], encode_text.size());


	}


	void id3v2_text_frame::set_text(const std::string &str)
	{
		text = str;
		encode();
	}

	int id3v2_text_frame::size()
	{
		return  encode_text.size() + 1;
	}

	void id3v2_text_frame::encode()
	{
		if (!text.empty())
			iconv_utils::convert("GB2312", get_string_of_codec(codec), text, encode_text);
		else
			encode_text = text;
	}

}

