#include "tag_manager.h"
#include <fstream>

namespace musictag{




	bool tag_manager::load(const std::string &file)
	{
		std::ifstream ifs(file, std::ios::binary);
		if (!ifs)
			return false;
		bool ret = load(ifs);

		ifs.close();
		filepath = file;
		return ret;
	}


	bool tag_manager::load(std::istream &is)
	{
		is.seekg(0, std::ios::end);
		file_size = is.tellg();


		if (loaded)
			return false;

		if (id3v11_tag::detect(is))
		{

			p_id3v11_tag = std::make_shared<id3v11_tag>();
			p_id3v11_tag->read(is);

		}

		if (id3v2_tag::detect(is))
		{

			p_id3v2_tag = std::make_shared<id3v2_tag>();
			p_id3v2_tag->read(is);

		}


		if (p_id3v2_tag)
			content_start = p_id3v2_tag->get_size();
		if (p_id3v11_tag)
			content_end = p_id3v11_tag->get_size();




		content_size = file_size - content_start - content_end;


		content.resize(content_size);
		is.seekg(content_start, std::ios::beg);
		is.read(&content[0], content_size);

	}



	bool tag_manager::unload()
	{
		if (!loaded)
			return false;

		p_id3v11_tag.reset();

		return true;
	}

	void tag_manager::print_tags()
	{
		if (p_id3v11_tag)
			std::cout << *p_id3v11_tag << std::endl;
		if (p_id3v2_tag)
			std::cout << *p_id3v2_tag << std::endl;

	}


	void tag_manager::save(const std::string &file)
	{
		std::ofstream ofs(file, std::ios::binary | std::ios::trunc);

		if (p_id3v2_tag)
		{
			p_id3v2_tag->write(ofs);
			std::cout << "write id3v2 ok" << std::endl;
		}

		ofs.write(&content[0], content.size());

		if (p_id3v11_tag)
		{
		//	p_id3v11_tag->write(ofs);
			//std::cout << "write id3v1 ok" << std::endl;
		}

		ofs.close();
	}


	void tag_manager::save()
	{
		std::string tmp = filepath + ".mp3";
		save(tmp);
	}


	bool tag_manager::reload()
	{
		return load(filepath);
	}


}