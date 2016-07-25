#include "id3v2_tag.h"
#include <fstream>
#include <iostream>

#include <wchar.h>


#include "iconv_utils.h"
#include <vector>
#include <fstream>


namespace musictag{
	using std::ifstream;

#define getTagSize(x) (((unsigned int)x[0]<<21)+((unsigned int)x[1]<<14)+((unsigned int)x[2]<<7)+x[3])
#define getFrameSize(x) (((unsigned int)x[0]<<24)+((unsigned int)x[1]<<16)+((unsigned int)x[2]<<8)+x[3])
	



	std::string PictureTypeStrings[21] =
	{
		"Other",
		"32x32 pixels 'file icon' (PNG only)",
		"Other file icon",
		"Cover (front)",
		"Cover (back)",
		"Leaflet page",
		"Media (e.g. lable side of CD)",
		"Lead artist/lead performer/soloist",
		"Artist/performer",
		"Conductor",
		"Band/Orchestra",
		"Composer",
		"Lyricist/text writer",
		"Recording Location",
		"During recording",
		"During performance",
		"Movie/video screen capture",
		"A bright coloured fish",
		"Illustration",
		"Band/artist logotype",
		"Publisher/Studio logotype"
	};


	typedef struct id3v2_raw_header
	{
		char Identifier[3];
		char Version[2];
		unsigned char Flags;
		unsigned char Size[4];

		id3v2_raw_header()
		{
			Identifier[0] = 'I';
			Identifier[1] = 'D';
			Identifier[2] = '3';
			Version[0] = 3;
			Version[1] = 0;
			Flags = 0;
			Size[0] = Size[1] =
				Size[2] = Size[3] = 0;
		}


	}id3v2_raw_header;

	typedef struct id3v2_raw_extended_header
	{

		unsigned char  Size[4];
		unsigned char  Flags[2];
		unsigned char  PaddingSize[4];

	}id3v2_raw_extended_header;


	typedef struct id3v2_raw_frame
	{
		char ID[4];
		unsigned char  Size[4];
		unsigned char  Flags[2];
		std::vector<char> data;


		id3v2_raw_frame()
		{};
		id3v2_raw_frame(const std::string& id)
		{
			ID[0] = id[0];
			ID[1] = id[1];
			ID[2] = id[2];
			ID[3] = id[3];
			Flags[0] = Flags[1] = 0;
		};

	}id3v2_raw_frame;


	std::string get_codec_string(char c)
	{
		if (c == ID3V2_ISO88591)
			return "ISO-8859-1";
		else if (c == ID3V2_UTF16BE)
			return "UTF-16BE";
		else if (c == ID3V2_UTF16LE)
			return "UTF-16LE";
		else if (c == ID3V2_UTF8)
			return "UTF-8";
		else throw std::exception("can't get codec");
	}

	void get_size(unsigned int size, unsigned char sizestr[4])
	{
		sizestr[0] = (size >> 24) & 0xff;
		sizestr[1] = (size >> 16) & 0xff;
		sizestr[2] = (size >> 8) & 0xff;
		sizestr[3] = (size >> 0) & 0xff;
	}


	void get_tag_size(unsigned int size, unsigned char sizestr[4])
	{
		sizestr[0] = (size >> 21) & 0xff;
		sizestr[1] = (size >> 14) & 0xff;
		sizestr[2] = (size >> 7) & 0xff;
		sizestr[3] = (size >> 0) & 0xff;
	}



	id3v2_tag::id3v2_tag()
	{
	}

	id3v2_tag::~id3v2_tag()
	{
	}

	std::string get_string_codec(const char *data, int len, int codec)
	{
		std::string str;
		switch (codec)
		{
		case ID3V2_ISO88591:

			str = std::string(data, len);
			break;
		case ID3V2_UTF16LE:
		{
							  unsigned short flag = *(unsigned short*)data;

							  if (flag == 0xfeff)
								  iconv_utils::convert("UTF-16", "GB2312", std::string(data, len), str);
							  else
								  iconv_utils::convert("UTF-16LE", "GB2312", std::string(data, len), str);

							  break;
		}
		case ID3V2_UTF16BE:
		{
							  unsigned short flag = *(unsigned short*)data;

							  if (flag == 0xfffe)
								  iconv_utils::convert("UTF-16", "GB2312", std::string(data, len), str);
							  else
								  iconv_utils::convert("UTF-16BE", "GB2312", std::string(data, len), str);
							  break;
		}

		case ID3V2_UTF8:
			iconv_utils::convert("UYF-8", "GB2312", std::string(data, len), str);
			break;
		default:
			break;
		}
		return str;
	}

	void id3v2_text_frame::parse(const std::vector<char> &data)
	{
		text = get_string_codec(&data[1], data.size() - 1, data[0]);
	}

	/*

	<Header for 'Attached picture', ID: "APIC">
	Text encoding   $xx
	MIME type       <text string> $00
	Picture type    $xx
	Description     <text string according to encoding> $00 (00)
	Picture data    <binary data>

	*/
	void id3v2_picture_frame::parse(const std::vector<char> &data)
	{
		char desc_codec = data[0];
		int read_pos = 0;
		while (read_pos < data.size() && data[++read_pos] != 0);

		mime = std::string(&data[1], read_pos - 1);

		++read_pos;

		pic_type = data[read_pos];
		++read_pos;
		int desc_start = read_pos;
		int desc_len = 0;
		if (desc_codec == 1 || desc_codec == 2)//UTF-16
		{
			while (read_pos < data.size() && data[read_pos] != 0 && data[read_pos + 1] != 0)
				++read_pos;

			int desc_len = read_pos - desc_start;

			read_pos += 2;
		}
		else
		{
			while (read_pos < data.size() && data[read_pos] != 0)
				++read_pos;

			desc_len = read_pos - desc_start;

			++read_pos;
		}


		desc = get_string_codec(&data[desc_start], desc_len, desc_codec);
		pic_data = std::vector<char>(data.begin() + read_pos, data.end());
	}

	/*

	<Header for 'Comment', ID: "COMM">
	Text encoding           $xx
	Language                $xx xx xx
	Short content descrip.  <text string according to encoding> $00 (00)
	The actual text         <full text string according to encoding>

	*/


	void id3v2_comment_frame::parse(const std::vector<char> &data)
	{

		char codec = data[0];
		std::string lang(&data[1],3);

		
		if (codec == ID3V2_ISO88591)
		{
			int mid = 4;
			for (; mid < data.size() && data[mid]!='\0'; ++mid);
			short_text = std::string(&data[4],mid-4);
			full_text = std::string(&data[mid+1], data.size()-mid-1);
		}
		else
		{
			int mid = 4;
			for (; mid < data.size() - 1 && (data[mid] != '\0' || data[mid+1] != '\0'); ++mid);
			std::string short_str = std::string(&data[4], mid - 4);
			std::string full_str = std::string(&data[mid + 2], data.size() - mid - 1);

			iconv_utils::convert(get_codec_string(codec), "ISO-8859-1", short_str,short_text);
			iconv_utils::convert(get_codec_string(codec), "ISO-8859-1", full_str, full_text);
		}
	}

	void id3v2_comment_frame::write(std::ofstream &os)
	{
		
		std::cout << tid << ":" << full_text << std::endl;
		id3v2_raw_frame frame(tid);

		int size = 4 + short_text.size() + 1 + full_text.size();
		get_size(size, frame.Size);
		
		printf("%d	%d\n", size,full_text.size());
		
		os.write((char*)&frame, 10);
		
		
		char chead[4] = {0x00,'X','X','X'};

		os.write(chead,4);
		if (short_text.size() > 0)
			os.write(short_text.c_str(), short_text.size());
		os.write(chead, 1);
		if (full_text.size() > 0)
			os.write(full_text.c_str(), full_text.size());
	
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

			std::string id = std::string(frame.ID, 4);
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
					frames.insert(make_pair(ID3V2_GET_ID(frame.ID), std::make_shared<id3v2_picture_frame>(id, frame.data)));
					break;
				case ID3V2_COMMENT:
					frames.insert(make_pair(ID3V2_GET_ID(frame.ID), std::make_shared<id3v2_comment_frame>(id, frame.data)));
					break;

				}
			}
			sizecount += frameSize + 10;
		}

		is.seekg(pos, std::ios::beg);
		return true;
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

	std::string id_to_string(id3v2_id id)
	{
		char str[4];
		str[0] = (id >> 24) & 0xff;
		str[1] = (id >> 16) & 0xff;
		str[2] = (id >> 8) & 0xff;
		str[3] = (id >> 0) & 0xff;
		return std::string(str, 4);
	}

	void id3v2_tag::set_item(id3v2_id id, const std::string &v)
	{
		switch (id)
		{
		case ID3V2_TITLE:
		case ID3V2_ALBUM:
		case ID3V2_ARTIST:
		case ID3V2_ALBUMARTIST:
		case ID3V2_YEAR:
		case ID3V2_DATE:
		case ID3V2_COMPOSER:
		case ID3V2_ENCODER:
		case ID3V2_CONTENTTYPE:
		case ID3V2_TRACK:
			if (frames.find(id) == frames.end())
			{

				frames[id] = std::make_shared<id3v2_text_frame>(id_to_string(id), v);
			}
			else
			{
				id3v2_text_frame *frame = dynamic_cast<id3v2_text_frame *>(frames[id].get());
				if (frame)
					frame->set_text(v);
			}
			break;

		case ID3V2_COMMENT:

			if (frames.find(id) == frames.end())
			{
				frames[id] = std::make_shared<id3v2_comment_frame>(id_to_string(id), v);
			}
			else
			{
				id3v2_comment_frame *frame = dynamic_cast<id3v2_comment_frame *>(frames[id].get());
				if (frame)
					frame->set_text(v);
			}
			break;

		default:
			break;
		}


	}







	void id3v2_text_frame::write(std::ofstream &os)
	{
		std::cout << tid << ":" << text << std::endl;
		id3v2_raw_frame frame(tid);
		std::string outstr;
		iconv_utils::convert("GB2312", "UTF-16LE", text, outstr);
		outstr += "\0\0";
		int size = outstr.size() + 1;//+codec
		get_size(size, frame.Size);

		os.write((char*)&frame, 10);
		unsigned char codec = 1;

		os.write((char*)&codec, 1);

		os.write(&outstr[0], outstr.size());

	}




	void id3v2_tag::write(std::ofstream &os)
	{
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

	}


}
