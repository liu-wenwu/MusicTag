#ifndef __ID3V2_COMMENT_FRAME_H__
#define __ID3V2_COMMENT_FRAME_H__

#include "id3v2_frame.h"

namespace musictag{


	class id3v2_comment_frame :public id3v2_frame
	{

	public:
		id3v2_comment_frame(const std::vector<char> &data) :id3v2_frame(ID3V2_COMMENT), codec(ID3V2_ISO88591)
		{
			parse(data);
		}
		id3v2_comment_frame(const std::string &short_str, const std::string &full_str) :
			id3v2_frame(ID3V2_COMMENT), codec(ID3V2_ISO88591), short_text(short_str), full_text(full_str)
		{
		}
		void set_text(const std::string &short_str, const std::string &full_str)
		{
			short_text = short_str;
			full_text = full_str;
		}


		virtual void write(std::ostream &os);
		void print(std::ostream &os) override
		{
			os << get_id_string(tid) << " : " << full_text << std::endl;
		};

		virtual int size();
	private:

		void parse(const std::vector<char> &data) override;

		int codec;
		std::string short_text;
		std::string full_text;
	};



}

#endif