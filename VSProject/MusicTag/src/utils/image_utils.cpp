#include "utils/image_utils.h"
#include <fstream>



namespace musictag
{
	static char BMP_HEAD[2] = { 0x42, 0x4D };
	static char JPEG_HEAD[4] = { 0xFF, 0xD8, 0xFF, 0xE0 };
	static char PNG_HEAD[4] = { 0x89, 0x50, 0x4E, 0x47 };
	static char GIF_HEAD[4] = { 0x47, 0x49, 0x46, 0x38 };
	static char TIFF_HEAD[4] = { 0x49, 0x49, 0x2A, 0x00 };


	bool image_utils::compare(char *a, char *b, int len)
	{
		for (; len > 0; --len)
		{
			if (*a++ != *b++)
				return false;
		}
		return true;
	}

	image_utils::IMAGE_MIME image_utils::get_image_mime(std::istream &is)
	{
		is.seekg(0, std::ios::beg);
		char head[4];
		is.read(head, 4);

		if (compare(head, BMP_HEAD, 2))
			return IMAGE_BMP;
		else if (compare(head, JPEG_HEAD, 4))
			return IMAGE_JPEG;
		else if (compare(head, PNG_HEAD, 4))
			return IMAGE_PNG;
		else if (compare(head, GIF_HEAD, 4))
			return IMAGE_GIF;
		else if (compare(head, TIFF_HEAD, 4))
			return IMAGE_TIFF;
		else
			return IMAGE_UNKNOWN;
	}

	std::string image_utils::mime_to_string(IMAGE_MIME mime)
	{
		switch (mime)
		{
		case musictag::image_utils::IMAGE_JPEG:
			return "image/jpeg";
		case musictag::image_utils::IMAGE_PNG:
			return "image/png";
		case musictag::image_utils::IMAGE_BMP:
			return "image/bmp";
		case musictag::image_utils::IMAGE_GIF:
			return "image/gif";
		case musictag::image_utils::IMAGE_TIFF:
			return "image/tiff";
		case musictag::image_utils::IMAGE_UNKNOWN:
		default:
			return std::string();
		}

	}

	std::string image_utils::get_image_mime_string(std::istream &is)
	{
		return mime_to_string(get_image_mime(is));
	}


	std::string image_utils::get_image_mime_string(std::string filepath)
	{
		std::ifstream ifs(filepath);
		if (!ifs)
			return std::string();
		std::string  mime = image_utils::get_image_mime_string(ifs);

		ifs.close();
		return mime;
	}


	IMAGE_INFO image_utils::get_image_info(std::istream &is)
	{


	}




}



