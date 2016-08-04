#ifndef __ID3V1_TAG_H__
#define __ID3V1_TAG_H__

#include "tag.h"
#include <list>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <sstream>

namespace musictag{

	class id3v1_tag : public tag
	{
	private:

		typedef struct ID3V11_RAW_HEAD
		{
			char Identifier[3];
			char Title[30];
			char Artist[30];
			char Album[30];
			char Year[4];
			char Comment[28];
			char Reserved;
			unsigned char Track;
			unsigned char Genre;
		}ID3V11_RAW_HEAD;




	public:
		enum IDV311_ITEM
		{
			TITLE,
			ARTIST,
			ALBUM,
			YEAR,
			COMMENT,
			TRACK,
			GENRE
		};
		static bool detect(std::istream &is);

		bool read(std::istream &is) override;

		friend std::ostream & operator<<(std::ostream &os, const id3v1_tag& tag)
		{
			dump(os, tag);
			return os;
		}
		static void dump(std::ostream& os, const id3v1_tag& tag);
		static const std::vector<std::string> &getGeneraList()
		{
			return  ID3Genres;
		}

		int get_size()
		{
			return sizeof(ID3V11_RAW_HEAD);
		}

		void strcopy(char *dst, const std::string &src, unsigned int max)
		{
			memcpy(dst, src.c_str(), max < src.size() ? max : src.size());
		}

		void set_item(IDV311_ITEM item, const std::string &v);

		void write(std::ofstream &os);
		id3v1_tag();
		~id3v1_tag();
	private:


		std::shared_ptr<ID3V11_RAW_HEAD> head;

		std::string get_string(const char*str, int maxlen);

		static const unsigned int id3v1_head_size = 128;
		static const int id3v1_head_offset = -128;
		static const std::vector<std::string> ID3Genres;

		std::string title;
		std::string artist;
		std::string album;
		std::string year;
		std::string comment;
		unsigned int track;
		unsigned int genre;
	};


}
#endif // ID3V1_H
