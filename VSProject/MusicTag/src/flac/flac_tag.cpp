#include "flac/flac_tag.h"


#include <string>
#include <iostream>

#include <algorithm>

#include "utils/iconv_utils.h"
#include "utils/common_utils.h"


namespace musictag{


	enum Metadatatype{
		STREAMINFO,
		PADDING,
		APPLICATION,
		SEEKTABLE,
		VORBIS_COMMENT,
		CUESHEET,
		PICTURE,
		RESERVED
	};


	typedef struct FlacMetadataHeader
	{
		unsigned char Type;
		char Size[3];
	}FlacMetadataHeader;



	
	 flac_tag::flac_tag()
	{
		size = 0;
	}

	void flac_tag::parse_stream_info(std::istream &is,int size)
	{
		stream_info.resize(size);
		is.read(&stream_info[0], size);
	}


	void flac_tag::parse_vorbis_comment(std::istream &is)
	{
		char size[4];
		is.read(size, 4);
		unsigned int refer_size = *(unsigned int*)(size);

		char *refer_str = new char[refer_size];
		is.read(refer_str, refer_size);

		std::string refer(refer_str, refer_size);

		is.read(size, 4);
		unsigned int count = *(unsigned int*)(size);

		for (unsigned int i = 0; i < count; i++)
		{
			std::shared_ptr<flac_vorbis_comment> ptr = flac_vorbis_comment::create(is);
			vc_frames.insert(std::make_pair(ptr->key(), ptr));
		}

	}

	void flac_tag::parse_picture(std::istream &is)
	{

		char size[4];
		is.read(size, 4);
		unsigned int type = common_utils::get_big_edian32(size);

		is.read(size, 4);
		unsigned int mime_size = common_utils::get_big_edian32(size);
		std::string mime;
		mime.resize(mime_size);
		is.read(&mime[0], mime_size);


		is.read(size, 4);
		unsigned int dis_size = common_utils::get_big_edian32(size);
		if (dis_size > 0)
		{
			std::vector<char> dis(dis_size);
			is.read(&dis[0], dis_size);
		}

		is.read(size, 4);
		unsigned int img_width = common_utils::get_big_edian32(size);

		is.read(size, 4);
		unsigned int img_height = common_utils::get_big_edian32(size);

		is.read(size, 4);
		unsigned int color_depth = common_utils::get_big_edian32(size);

		is.read(size, 4);
		unsigned int color_index = common_utils::get_big_edian32(size);

		is.read(size, 4);
		unsigned int img_length = common_utils::get_big_edian32(size);


		std::vector<char> picture_data(img_length);
		is.read(&picture_data[0], img_length);
	
		picture_frames.insert(std::make_shared<flac_picture>(img_width, img_height, mime, picture_data));
	}




	bool flac_tag::detect(std::istream &is)
	{
		bool ok = false;
		std::streamoff start = is.tellg();
		is.seekg(0, std::ios::beg);
		char flac_id[4];
		is.read(flac_id, 4);

		if (flac_id[0] == 'f'&& flac_id[1] == 'L'&& flac_id[2] == 'a'&& flac_id[3] == 'C')
			ok = true;

		is.seekg(start, std::ios::beg);
		return ok;
	}

	bool flac_tag::read(std::istream &is)
	{

		std::streamoff start = is.tellg();
		is.seekg(0, std::ios::beg);
		char flac_id[4];
		is.read(flac_id, 4);


		if (flac_id[0] != 'f' || flac_id[1] != 'L' || flac_id[2] != 'a' || flac_id[3] != 'C')
			return false;

		while (true)
		{
			FlacMetadataHeader header;

			is.read((char *)&header, 4);


			Metadatatype type = (Metadatatype)(header.Type & 0x7F);

			unsigned int size = common_utils::get_big_edian24(header.Size);

			printf("type:%d  size:%u\n", type, size);

			switch (type) {
			case STREAMINFO:
				parse_stream_info(is, size);
				break;
			case VORBIS_COMMENT:
				parse_vorbis_comment(is);
				break;
			case PICTURE:
				parse_picture(is);
				break;
			default:
				is.seekg(size, std::ios::cur);
				break;
			}

			if (header.Type & 0x80)
				break;

		}

		std::streamoff end = is.tellg();
		size = end - start;

	}



	void flac_tag::set_title(const std::string &str)
	{
		set_vorbis_comment("TITLE", str);
	}

	void flac_tag::set_artist(const std::string &str)
	{
		set_vorbis_comment("ARTIST", str);
	}

	void flac_tag::set_album(const std::string &str)
	{
		set_vorbis_comment("ALBUM", str);
	}



	void flac_tag::set_vorbis_comment(const std::string &key, const std::string &value)
	{
		if (vc_frames.find(key) == vc_frames.end())
		{
			vc_frames.insert(std::make_pair(key, std::make_shared<flac_vorbis_comment>(key, value)));
		}
		else
			vc_frames[key]->set_value(value);
	}




	void flac_tag::dump(std::ostream& os, const flac_tag& tag)
	{
		os << "[ flac ]" << std::endl;
		for (std::unordered_map<std::string, std::shared_ptr<flac_vorbis_comment> >::const_iterator iter = tag.vc_frames.begin();
			iter != tag.vc_frames.end(); ++iter)
		{
			iter->second->print(os);
		}

		for (std::unordered_set<std::shared_ptr<flac_picture> >::const_iterator iter = tag.picture_frames.begin();
			iter != tag.picture_frames.end(); ++iter)
		{
			iter->get()->print(os);
		}

	
	}



	void flac_tag::write(std::ofstream &os)
	{



	}


}

