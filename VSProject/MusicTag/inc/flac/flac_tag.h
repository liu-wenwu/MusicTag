#ifndef __FLAC_TAG_H__
#define __FLAC_TAG_H__

#include "tag.h"
#include <istream>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "flac_vorbis_comment.h"
#include "flac_picture.h"


namespace musictag{

	class flac_tag : public tag
	{
	public:
		bool read(std::istream &is) override;
		static bool detect(std::istream &is);

		void set_title(const std::string &str);
		void set_artist(const std::string &str);
		void set_album(const std::string &str);


		void write(std::ofstream &os);


		int get_size() const
		{
			return size;
		}


		friend std::ostream & operator<<(std::ostream &os, const flac_tag& tag)
		{
			dump(os, tag);
			return os;
		}
		
	private:
		static void dump(std::ostream& os, const flac_tag& tag);

		void set_vorbis_comment(const std::string &key, const std::string &value);


		void parse_stream_info(std::istream &is,int size);
		void parse_vorbis_comment(std::istream &is);
		void parse_picture(std::istream &is);
		bool add_frame(const std::string &content);

		std::unordered_map<std::string, std::shared_ptr<flac_vorbis_comment> > vc_frames;

		std::unordered_set<std::shared_ptr<flac_picture> >	picture_frames;
		std::vector<char> stream_info;

		int size ;
	};



};

#endif // FLACTAG_H
