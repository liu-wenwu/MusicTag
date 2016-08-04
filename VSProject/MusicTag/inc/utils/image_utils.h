#ifndef __IMAGE_UTILS_H__
#define __IMAGE_UTILS_H__

#include <istream>
#include <string>

namespace musictag
{
	typedef struct IMAGE_INFO
	{ 
		unsigned int width;
		unsigned int height;
		unsigned int depth;
	};

	class image_utils
	{
	public:



		enum IMAGE_MIME
		{
			IMAGE_JPEG,
			IMAGE_PNG,
			IMAGE_BMP,
			IMAGE_GIF,
			IMAGE_TIFF,
			IMAGE_UNKNOWN
		};

		static IMAGE_MIME get_image_mime(std::istream &is);
		static std::string mime_to_string(IMAGE_MIME mime);
		static std::string get_image_mime_string(std::istream &is);
		static std::string get_image_mime_string(std::string filepath);

		static IMAGE_INFO get_image_info(std::istream &is);

	private:
		static bool compare(char *a, char *b, int len);
	
	};




}



#endif