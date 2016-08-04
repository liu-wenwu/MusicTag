#include "id3v2/id3v2_picture_frame.h"
#include "utils/image_utils.h"
#include <fstream>


namespace musictag{


	static std::vector<std::string> g_picture_type_strings =
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

	id3v2_picture_frame::id3v2_picture_frame(const std::string &picpath, int type, const std::string &desc)
		:desc_codec(ID3V2_ISO88591), pic_type(type), pic_desc(desc)
	{
		pic_mime = image_utils::get_image_mime_string(picpath);
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

		pic_mime = std::string(&data[1], read_pos - 1);

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

		pic_desc = get_string_by_codec(&data[desc_start], desc_len, desc_codec);
		pic_data = std::vector<char>(data.begin() + read_pos, data.end());
	}



	std::vector<std::string> &id3v2_picture_frame::get_type_strings()
	{
		return g_picture_type_strings;
	}


	void id3v2_picture_frame::write(std::ostream &os)
	{
		
		if (pic_data.empty())
			return;


		id3v2_raw_frame frame(tid);

		get_frame_size(size(), frame.Size);

		os.write((char *)(&frame), 10);

		char codec = 0;
		os.write(&codec, 1);
		os.write(&pic_mime[0], pic_mime.size());
		char char0 = 0;
		os.write(&char0, 1);

		os.write(&pic_type, 1);

		os.write(&pic_desc[0], pic_desc.size());
		os.write(&char0, 1);
		if (codec != ID3V2_ISO88591)
			os.write(&char0, 1);
		os.write(&pic_data[0], pic_data.size());

		printf("\nid3v2_picture_frame :%d!!!\n", pic_data.size());



	}


	int id3v2_picture_frame::size()
	{
		//codec+mime+$00+type+desc+$00($00)+pic
		return 1 + pic_mime.size() + 1 + 1 + pic_desc.size() + (desc_codec == ID3V2_ISO88591 ? 1 : 2) + pic_data.size();
	}


	void id3v2_picture_frame::set_type(int type)
	{
		pic_type = type;
	}

	void id3v2_picture_frame::set_desc(const std::string &desc)
	{
		pic_desc = desc;
	}

	void id3v2_picture_frame::set_picture(const std::string &picpath)
	{
		std::ifstream ifs(picpath,std::ios::binary);
		if (!ifs)
			return;

		pic_mime = image_utils::get_image_mime_string(ifs);

		std::cout << "pic_mime:" << pic_mime << std::endl;

		ifs.seekg(0, std::ios::end);
		int length = ifs.tellg();
		ifs.seekg(0, std::ios::beg);

		printf("\nsize:%d\n",length);
		pic_data.resize(length);
		ifs.read(&pic_data[0], length);

		ifs.close();
	}




	void id3v2_picture_frame::save_picture(const std::string &path)
	{

		std::ofstream ofs("E:/test.jpg", std::ios::binary);
	
		ofs.write(&pic_data[0],pic_data.size());


		ofs.close();


	}

	void id3v2_picture_frame::print(std::ostream &os) 
	{
		os << get_id_string(tid) << ":type[" << g_picture_type_strings[pic_type] << "] mime[" << pic_mime << "] desc[" << pic_desc << "] size[" << pic_data.size() << "]" << std::endl;
	};

}

