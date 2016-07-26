#include "id3v2/id3v2_picture_frame.h"

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

		desc = get_string_by_codec(&data[desc_start], desc_len, desc_codec);
		pic_data = std::vector<char>(data.begin() + read_pos, data.end());
	}



	std::vector<std::string> &id3v2_picture_frame::get_type_strings()
	{
		return g_picture_type_strings;
	}


	void id3v2_picture_frame::write(std::ostream &os)
	{
		id3v2_raw_frame frame(tid);

		int size = 1 + mime.size() + 1 + desc.size() + (codec == ID3V2_ISO88591 ? 1 : 2) + pic_data.size();
		get_frame_size(size, frame.Size);

		os.write((char *)(&frame), 10);


		char codec = 0;
		os.write(&codec, 1);
		os.write(&mime[0], mime.size());
		char char0 = 0;
		os.write(&char0, 1);

		os.write(&desc[0], desc.size());
		os.write(&char0, 1);

		os.write(&pic_data[0], pic_data.size());

	}


	int id3v2_picture_frame::size()
	{


	}




}

