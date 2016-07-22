#ifndef __ID3V1_TAG_H__
#define __ID3V1_TAG_H__

#include "tag.h"
#include <list>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>


namespace musictag{

	class id3v11_tag : public tag
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
			char Track;
			unsigned char Genre;
		}ID3V11_RAW_HEAD;

	public:

		static bool detect(std::istream &is);

		bool read(std::istream &is) override;

		friend std::ostream & operator<<(std::ostream &os, const id3v11_tag& tag)
		{
			dump(os, tag);
			return os;
		}
		static void dump(std::ostream& os, const id3v11_tag& tag);
		static const std::vector<std::string> &getGeneraList()
		{
			return  ID3Genres;
		}

		int get_size()
		{
			return sizeof(ID3V11_RAW_HEAD);
		}

		void strcopy(char *dst, const std::string &src, int max)
		{
			memcpy(dst, src.c_str(), max<src.size() ?max : src.size());
		}

		void set_artist(const std::string &v)
		{
			artist = v;
			if (head)
				strcopy(head->Artist, artist.c_str(),30);
		}

		void write(std::ofstream &os);
		id3v11_tag();
		~id3v11_tag();
	private:


		std::shared_ptr<ID3V11_RAW_HEAD> head;

		std::string get_string(const char*str, int maxlen);

		static const int id3v1_head_size = 128;
		static const int id3v1_head_offset = -128;
		static const std::vector<std::string> ID3Genres;

		std::string title;
		std::string artist;
		std::string album;
		std::string year;
		std::string comment;
		char track;
		unsigned char genre;
	};


}
#endif // ID3V1_H
