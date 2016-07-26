#ifndef __ID3V2_PICTURE_FRAME_H__
#define __ID3V2_PICTURE_FRAME_H__

#include "id3v2_frame.h"


namespace musictag{


	class id3v2_picture_frame :public id3v2_frame
	{
	public:
		id3v2_picture_frame(const std::vector<char> &data) :id3v2_frame(ID3V2_PICTURE), codec(ID3V2_ISO88591)
		{
			parse(data);
		}

		id3v2_picture_frame(const std::string &picpath, int type, const std::string &desc):codec(ID3V2_ISO88591)
		{
		}

		static std::vector<std::string> &get_type_strings();
		virtual void write(std::ostream &os);

		virtual int size();
	private:

		std::string mime;
		char pic_type;
		std::string desc;
		std::vector<char> pic_data;
		int codec;

		void parse(const std::vector<char> &data) override;
		void print(std::ostream &os) override
		{
			os << tid << " : " << mime << ",desc[" << desc << "] size:" << pic_data.size() << std::endl;
		};
	};


}


#endif