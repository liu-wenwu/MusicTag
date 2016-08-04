#ifndef __FLAC_VORBIS_COMMENT_H__
#define __FLAC_VORBIS_COMMENT_H__


#include <iostream>
#include <memory>
#include <string>

namespace musictag{

	class flac_vorbis_comment
	{


	public:

		enum FLAC_VC_TYPE
		{
			TITLE,
			ARTIST,
			COMMENT,
			GENRE,
			COMPOSER,
			ALBUMARTIST,
			ALBUM,
			DATE,
			DISCNUMBER,
			TRACKNUMBER,
			TRACKTOTAL,
			UNKNOWN
		};

		
		flac_vorbis_comment(const std::string &key, const std::string & value) :_key(key), _value(value)
		{

		}

		static std::shared_ptr<flac_vorbis_comment>  create(std::istream &is);


		void write(std::ostream &os);
		std::string key() const
		{
			return _key;
		}
		std::string value() const
		{
			return _value;
		}

		void set_value(const std::string &v)
		{
			_value = v;
		}

		void print(std::ostream &os)
		{
			os << _key << " : " << _value << std::endl;
		}

	private:
		std::string _key;
		std::string _value;

	};






}






#endif
