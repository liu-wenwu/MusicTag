#ifndef __ID3V2_URL_FRAME_H__
#define __ID3V2_URL_FRAME_H__

#include "id3v2_frame.h"

namespace musictag{


	class id3v2_url_frame :public id3v2_frame
	{
	public:
		id3v2_url_frame(const std::vector<char> &data) :id3v2_frame()
		{
			parse(data);
		}


	private:

		void parse(const std::vector<char> &data) override;
	};


}


#endif