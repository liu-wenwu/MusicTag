#include "id3v2/id3v2_tag.h"
#include <fstream>
#include <iostream>

#include <wchar.h>

#include <vector>
#include <fstream>
#include <algorithm>

namespace musictag{



	unsigned int id3v2_tag::get_frame_size(unsigned char x[4])
	{
		return (((unsigned int)x[0] << 24) + ((unsigned int)x[1] << 16) + ((unsigned int)x[2] << 8) + x[3]);
	}

	unsigned int id3v2_tag::get_tag_size(unsigned char x[4])
	{
		return (((unsigned int)x[0] << 21) + ((unsigned int)x[1] << 14) + ((unsigned int)x[2] << 7) + x[3]);
	}

	void id3v2_tag::tag_size_to_string(unsigned int size, unsigned char sizestr[4])
	{
		int idx = 3;
		while (idx >= 0)
		{
			sizestr[idx] = size % 128;
			size /= 128;
			--idx;
		}

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
		size = get_tag_size(header.Size);

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


			unsigned int frameSize = get_frame_size(frame.Size);


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
					//frames.insert(make_pair(ID3V2_GET_ID(frame.ID), std::make_shared<id3v2_picture_frame>(frame.data)));
					pic_frames.insert(std::make_shared<id3v2_picture_frame>(frame.data));

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

		for (std::unordered_set<std::shared_ptr<id3v2_picture_frame> >::const_iterator iter = tag.pic_frames.begin(); iter != tag.pic_frames.end(); ++iter)
		{
			iter->get()->print(os);
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
			frames[ID3V2_COMMENT] = std::make_shared<id3v2_comment_frame>(short_str, full_str);
		}
		else
		{
			id3v2_comment_frame *frame = dynamic_cast<id3v2_comment_frame *>(frames[ID3V2_COMMENT].get());
			if (frame)
				frame->set_text(short_str, full_str);
		}
	}
	void id3v2_tag::set_picture(const std::string &picpath, int type, const std::string &desc)
	{
		if (frames.find(ID3V2_PICTURE) == frames.end())
		{
			frames[ID3V2_PICTURE] = std::make_shared<id3v2_picture_frame>(picpath, type, desc);
		}
		else
		{
			id3v2_picture_frame *frame = dynamic_cast<id3v2_picture_frame *>(frames[ID3V2_PICTURE].get());
			if (frame)
			{
				frame->set_picture(picpath);
				frame->set_type(type);
				frame->set_desc(desc);
			}
		}
	}


	bool cmp_frame_size(const std::shared_ptr<id3v2_frame> &a, const std::shared_ptr<id3v2_frame> &b)
	{
		return a->size() < b->size();
	}


	void id3v2_tag::write(std::ofstream &os)
	{

		int start_pos = os.tellp();
		struct id3v2_raw_header header;
		os.write((char*)&header, sizeof(struct id3v2_raw_header));
		int frame_pos = os.tellp();

	
		for (std::unordered_map<id3v2_id, std::shared_ptr<id3v2_frame> >::iterator iter = frames.begin(); iter != frames.end(); ++iter)
		{
			iter->second->write(os);
		}

		for (std::unordered_set<std::shared_ptr<id3v2_picture_frame> >::iterator iter = pic_frames.begin(); iter != pic_frames.end(); ++iter)
		{
			iter->get()->write(os);
		}


		int frame_end = os.tellp();
		int size = frame_end - frame_pos;

		tag_size_to_string(size, header.Size);

		os.seekp(start_pos, std::ios::beg);
		os.write((char*)&header, sizeof(struct id3v2_raw_header));
		os.seekp(frame_end, std::ios::beg);


	}


	void id3v2_tag::save_picture(const std::string &path)
	{
		if (frames.find(ID3V2_PICTURE) == frames.end())
			return;

		id3v2_picture_frame *frame = dynamic_cast<id3v2_picture_frame *>(frames[ID3V2_PICTURE].get());
		frame->save_picture(path);

	}



}
