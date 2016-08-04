#ifndef __id3v2_tag_TAG_H__
#define __id3v2_tag_TAG_H__

#include "tag.h"

#include "id3v2.h"
#include "id3v2_frame.h"
#include "id3v2_text_frame.h"
#include "id3v2_url_frame.h"
#include "id3v2_comment_frame.h"
#include "id3v2_picture_frame.h"

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <memory>
#include <iostream>



namespace musictag{



	class id3v2_tag : public tag
	{
	public:
		id3v2_tag(){}
		~id3v2_tag(){}
		bool read(std::istream &is) override;
		static bool detect(std::istream &is);

		void write(std::ofstream &os);

		friend std::ostream & operator<<(std::ostream &os, const id3v2_tag& tag)
		{
			dump(os, tag);
			return os;
		}

		int get_size()
		{
			return size + 10;
		}

		static const std::vector<std::string> & get_picture_types();

		static std::string id_to_string(id3v2_id id);

		void set_text(id3v2_id id, const std::string &text);
		void set_comment(const std::string &short_str, const std::string &full_str);
		void set_picture(const std::string &picpath ,int type, const std::string &desc);
		void save_picture(const std::string &path);
	private:

		static unsigned int get_frame_size(unsigned char x[4]);
		static unsigned int get_tag_size(unsigned char sizestr[4]);
		static void tag_size_to_string(unsigned int size, unsigned char sizestr[4]);
		int subversion;//id3v2.x
		static void dump(std::ostream& os, const id3v2_tag& tag);
		std::unordered_map<id3v2_id, std::shared_ptr<id3v2_frame> > frames;

		std::unordered_set<std::shared_ptr<id3v2_picture_frame> > pic_frames;

		unsigned int size = 0;
	};


}


#endif // id3v2_tag_H


