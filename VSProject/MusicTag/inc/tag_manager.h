#ifndef __MUSICTAG_H__
#define __MUSICTAG_H__

#include "id3v11_tag.h"
#include "id3v2_tag.h"

#include <memory>

namespace musictag{



	class tag_manager
	{
	public:

		tag_manager() :loaded(false)
		{}

		bool load(const std::string &file);
		bool load(std::istream &is);

		bool reload();
		bool unload();

		bool is_loaded() const{
			return loaded;
		}
		void print_tags();


		void set_title(const std::string &str){
			_title = str;
			if (p_id3v11_tag)
				p_id3v11_tag->set_item(id3v11_tag::TITLE, str);

			if (p_id3v2_tag)
				p_id3v2_tag->set_item(ID3V2_TITLE, str);
		}
		std::string title() const{
			return _title;
		}

		void set_artist(const std::string &str){
			_artist = str;
			if (p_id3v11_tag)
				p_id3v11_tag->set_item(id3v11_tag::ARTIST, str);
			if (p_id3v2_tag)
				p_id3v2_tag->set_item(ID3V2_ARTIST, str);
		}
		std::string artist() const{
			return _artist;
		}
		std::string album() const{
			return _album;
		}
		void set_album(const std::string &str){
			_album = str;
			if (p_id3v11_tag)
				p_id3v11_tag->set_item(id3v11_tag::ALBUM, str);
			if (p_id3v2_tag)
				p_id3v2_tag->set_item(ID3V2_ALBUM, str);
		}

		void  set_comment(const std::string &str){
			_comment = str;
			if (p_id3v11_tag)
				p_id3v11_tag->set_item(id3v11_tag::COMMENT, str);

			if (p_id3v2_tag)
				p_id3v2_tag->set_item(ID3V2_COMMENT, str);
		}
		void  set_year(const std::string &str){
			_year = str;
			if (p_id3v11_tag)
				p_id3v11_tag->set_item(id3v11_tag::YEAR, str);
			if (p_id3v2_tag)
				p_id3v2_tag->set_item(ID3V2_YEAR, str);
		}
		void  set_track(const std::string &str){
			_year = str;
			if (p_id3v11_tag)
				p_id3v11_tag->set_item(id3v11_tag::TRACK, str);
			if (p_id3v2_tag)
				p_id3v2_tag->set_item(ID3V2_TRACK, str);
		}

		std::shared_ptr<id3v11_tag> get_id3v11_tag() const{
			return p_id3v11_tag;
		}
		std::shared_ptr<id3v11_tag> get_id3v2_tag() const {
			return p_id3v11_tag;
		}


		void save(const std::string &file);
		void save();

	private:
		std::shared_ptr<id3v11_tag> p_id3v11_tag;
		std::shared_ptr<id3v2_tag> p_id3v2_tag;
		bool loaded;

	
		unsigned long file_size;
		unsigned long content_size;
		unsigned long content_start;
		unsigned long content_end;

		std::vector<char> content;
		std::string filepath;
		/**/
		std::string  _title;
		std::string  _artist;
		std::string  _album;

		std::string _comment;

		std::string _year;
		std::string _date;
		std::string _genre;
	};




}




#endif

