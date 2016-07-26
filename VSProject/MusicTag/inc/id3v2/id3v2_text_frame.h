#ifndef __ID3V2_TEXT_FRAME_H__
#define __ID3V2_TEXT_FRAME_H__


#include "id3v2_frame.h"

namespace musictag{


	class id3v2_text_frame :public id3v2_frame
	{

	public:
		id3v2_text_frame(const id3v2_id id, const std::vector<char> &data) :id3v2_frame(id), codec(ID3V2_UTF16LE)
		{
			parse(data);
		}
		id3v2_text_frame(const id3v2_id id, const std::string &data) :id3v2_frame(id), codec(ID3V2_UTF16LE), text(data)
		{
		}
		void set_text(const std::string &str);


		virtual void write(std::ostream &os);
		virtual int size();
	private:

		void parse(const std::vector<char> &data) override;
		void print(std::ostream &os) override
		{
			os << get_id_string(tid) << " : " << text << std::endl;
		};

		void encode();

		ID3V2_CODEC codec;
		std::string text;
		std::string encode_text;
	};


}






#endif

