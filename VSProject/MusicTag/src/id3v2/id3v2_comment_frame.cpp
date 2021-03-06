#include "id3v2/id3v2_comment_frame.h"
#include "utils/iconv_utils.h"


namespace musictag{



	/*

	<Header for 'Comment', ID: "COMM">
	Text encoding           $xx
	Language                $xx xx xx
	Short content descrip.  <text string according to encoding> $00 (00)
	The actual text         <full text string according to encoding>

	*/
	void id3v2_comment_frame::parse(const std::vector<char> &data)
	{
		
		char codec = data[0];
		std::string lang(&data[1], 3);


		if (codec == ID3V2_ISO88591)
		{
			int mid = 4;
			for (; mid < data.size() && data[mid] != '\0'; ++mid);
			std::string short_str = std::string(&data[4], mid - 4);
			std::string full_str = std::string(&data[mid + 1], data.size() - mid - 1);

			iconv_utils::convert(get_string_of_codec(codec), "GB2312", short_str, short_text);
			iconv_utils::convert(get_string_of_codec(codec), "GB2312", full_str, full_text);
		}
		else
		{
			int mid = 4;
			for (; mid < data.size() - 1 && (data[mid] != '\0' || data[mid + 1] != '\0'); ++mid);
			std::string short_str = std::string(&data[4], mid - 4);
			std::string full_str = std::string(&data[mid + 2], data.size() - mid - 2);
		
			iconv_utils::convert(get_string_of_codec(codec), "GB2312", short_str, short_text);
			iconv_utils::convert(get_string_of_codec(codec), "GB2312", full_str, full_text);

			printf("id3v2_comment_frame:%s \n", get_string_of_codec(codec).c_str());
		}
	


	}


	int id3v2_comment_frame::size()
	{
		return  1 + 3 + short_text.size() + (codec==ID3V2_ISO88591?1:2)+full_text.size();
	}

	void id3v2_comment_frame::write(std::ostream &os)
	{
		if (full_text.empty())
			return;

		std::cout << get_id_string(tid) << ":" << full_text << std::endl;
		id3v2_raw_frame frame(tid);

		int size = 4 + short_text.size() + 1 + full_text.size();
		get_frame_size(size, frame.Size);


		os.write((char*)&frame, 10);

		char chead[4] = { 0x00, 'X', 'X', 'X' };

		os.write(chead, 4);
		if (short_text.size() > 0)
			os.write(short_text.c_str(), short_text.size());
		os.write(chead, 1);
	
		os.write(full_text.c_str(), full_text.size());
		
	}




}

