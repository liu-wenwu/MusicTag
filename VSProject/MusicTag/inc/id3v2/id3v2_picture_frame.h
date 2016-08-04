#ifndef __ID3V2_PICTURE_FRAME_H__
#define __ID3V2_PICTURE_FRAME_H__

#include "id3v2_frame.h"


namespace musictag{


	class id3v2_picture_frame :public id3v2_frame
	{
	public:
		id3v2_picture_frame(const std::vector<char> &data) :id3v2_frame(ID3V2_PICTURE), desc_codec(ID3V2_ISO88591)
		{
			parse(data);
		}

		id3v2_picture_frame(const std::string &picpath, int type, const std::string &desc);

		void set_type(int type);
		void set_desc(const std::string &desc);
		void set_picture(const std::string &picpath);
		void save_picture(const std::string &path);

		static std::vector<std::string> &get_type_strings();
		virtual void write(std::ostream &os);

		virtual int size();

		 void print(std::ostream &os) override;

	private:

		std::string pic_mime;
		char pic_type;
		std::string pic_desc;
		std::vector<char> pic_data;
		int desc_codec;

		void parse(const std::vector<char> &data) override;

	};


}


#endif