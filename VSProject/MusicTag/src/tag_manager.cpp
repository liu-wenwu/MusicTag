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

		return ret;
	}


	bool tag_manager::load(std::istream &is)
	{
		is.seekg(0,std::ios::end);
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
		is.seekg(content_start,std::ios::beg);
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


	void save(const std::string &file)
	{
		std::ofstream ofs(file, std::ios::binary | std::ios::trunc);


	}


	void save()
	{
	
	
	}

}