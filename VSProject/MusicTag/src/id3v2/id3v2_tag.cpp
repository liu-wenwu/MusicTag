#include "id3v2/id3v2_tag.h"
#include <fstream>
#include <iostream>

#include <wchar.h>


#include "iconv_utils.h"
#include <vector>
#include <fstream>


namespace musictag{

	void id3v2_tag::get_tag_size(unsigned int size, unsigned char sizestr[4])
	{
		sizestr[0] = (size >> 21) & 0xff;
		sizestr[1] = (size >> 14) & 0xff;
		sizestr[2] = (size >> 7) & 0xff;
		sizestr[3] = (size >> 0) & 0xff;
	}
	std::string id3v2_tag::id_to_string(id3v2_id id)
	{
		char str[4];
		str[0] = (id >> 24) & 0xff;
		str[1] = (id >> 16) & 0xff;
		str[2] = (id >> 8) & 0xff;
		str[3] = (id >> 0) & 0xff;
		return std::string(str, 4);
	}


	bool id3v2_tag::read(std::istream &is)
	{

		unsigned long pos = is.tellg();
		is.seekg(0, std::ios::beg);
		id3v2_raw_header header;

		is.read((char *)&header, sizeof(id3v2_raw_header));
		subversion = header.Version[0];


		bool have_extend_header = header.Flags & 0x40;
		size = getTagSize(header.Size);

		if (have_extend_header)
		{
			id3v2_raw_extended_header exHeader;
			is.read((char *)&exHeader, sizeof(id3v2_raw_extended_header));
		}

		unsigned int sizecount = 0;

		while (sizecount < size)
		{
			id3v2_raw_frame frame;

			if (!is.read((char *)&frame, 10))
				break;

			if (frame.ID[0] == 0 || frame.ID[1] == 0 || frame.ID[2] == 0 || frame.ID[3] == 0)
			{
				int padding = size - sizecount;
				break;
			}

			id3v2_id id = ID3V2_GET_ID(frame.ID);


			unsigned int frameSize = getFrameSize(frame.Size);

			frame.data.resize(frameSize);

			is.read(&frame.data[0], frameSize);

			if (frame.ID[0] == 'T')
			{
				frames.insert(make_pair(ID3V2_GET_ID(frame.ID), std::make_shared<id3v2_text_frame>(id, frame.data)));
			}
			else
			{
				switch (ID3V2_GET_ID(frame.ID))
				{
				case ID3V2_PICTURE:
					frames.insert(make_pair(ID3V2_GET_ID(frame.ID), std::make_shared<id3v2_picture_frame>(frame.data)));
					break;
				case ID3V2_COMMENT:
					frames.insert(make_pair(ID3V2_GET_ID(frame.ID), std::make_shared<id3v2_comment_frame>(frame.data)));
					break;

				}
			}
			sizecount += frameSize + 10;
		}

		is.seekg(pos, std::ios::beg);
		return true;
	}

	const std::vector<std::string> & id3v2_tag::get_picture_types()
	{
		return id3v2_picture_frame::get_type_strings();
	}

	void id3v2_tag::dump(std::ostream& os, const id3v2_tag& tag)
	{
		os << "[ id3v2." << tag.subversion << " ]" << std::endl;

		for (std::unordered_map<id3v2_id, std::shared_ptr<id3v2_frame> >::const_iterator iter = tag.frames.begin(); iter != tag.frames.end(); ++iter)
		{
			iter->second->print(os);
		}

	}

	bool id3v2_tag::detect(std::istream &is)
	{
		bool ok = false;
		unsigned long pos = is.tellg();
		is.seekg(0, std::ios::beg);

		char id[3];
		is.read(id, 3);

		if (id[0] == 'I' && id[1] == 'D' && id[2] == '3')
			ok = true;

		is.seekg(pos, std::ios::beg);
		return ok;
	}



	void id3v2_tag::set_text(id3v2_id id, const std::string &v)
	{
		if (frames.find(id) == frames.end())
		{

			frames[id] = std::make_shared<id3v2_text_frame>(id, v);
		}
		else
		{
			id3v2_text_frame *frame = dynamic_cast<id3v2_text_frame *>(frames[id].get());
			if (frame)
				frame->set_text(v);
		}

	}

	void id3v2_tag::set_comment(const std::string &short_str, const std::string &full_str)
	{

		if (frames.find(ID3V2_COMMENT) == frames.end())
		{
			frames[ID3V2_COMMENT] = std::make_shared<id3v2_comment_frame>(short_str,full_str);
		}
		else
		{
			id3v2_comment_frame *frame = dynamic_cast<id3v2_comment_frame *>(frames[ID3V2_COMMENT].get());
			if (frame)
				frame->set_text(short_str,full_str);
		}
	}
	void id3v2_tag::set_picture(const std::string &picpath, int type, const std::string &desc)
	{
		if (frames.find(ID3V2_PICTURE) == frames.end())
		{
			//frames[ID3V2_PICTURE] = std::make_shared<id3v2_picture_frame>(ID3V2_PICTURE, full_str);
		}
		else
		{
			id3v2_picture_frame *frame = dynamic_cast<id3v2_picture_frame *>(frames[ID3V2_PICTURE].get());
			//if (frame)
			//	frame->set_text(full_str);
		}
	}




	void id3v2_tag::write(std::ofstream &os)
	{
		/*
		int start_pos = os.tellp();
		struct id3v2_raw_header header;
		os.write((char*)&header, sizeof(struct id3v2_raw_header));
		int frame_pos = os.tellp();

		std::unordered_map<id3v2_id, std::shared_ptr<id3v2_frame> >::iterator iter = frames.begin();
		for (; iter != frames.end(); ++iter)
		{
			iter->second->write(os);
		}

		int frame_end = os.tellp();
		int size = frame_end - frame_pos;

		get_tag_size(size, header.Size);

		os.seekp(start_pos, std::ios::beg);
		os.write((char*)&header, sizeof(struct id3v2_raw_header));
		os.seekp(frame_end, std::ios::beg);
		*/

	}


}
