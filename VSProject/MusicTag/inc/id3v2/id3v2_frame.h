#ifndef __ID3V2_FRAME_H__
#define __ID3V2_FRAME_H__

#include "id3v2.h"
#include <vector>
#include <iostream>
#include <string>

namespace musictag{


	/* ֡ */
	class id3v2_frame
	{
	public:
		id3v2_frame()
		{
		}

		id3v2_frame(const id3v2_id id) :tid(id)
		{}
		virtual void parse(const std::vector<char> &data){};
		virtual void print(std::ostream &os){};

		virtual void write(std::ostream &os){};

		virtual int size(){ return 0; };


		static std::string get_id_string(id3v2_id id);

		static void get_frame_size(unsigned int size, unsigned char sizestr[4]);
		static std::string get_string_by_codec(const char *data, int len, int codec);
		static std::string get_string_of_codec(char c);

	protected:
		id3v2_id tid;
	};


}




#endif