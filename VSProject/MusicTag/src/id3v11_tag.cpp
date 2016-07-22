#include "id3v11_tag.h"

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>

namespace musictag{

	using std::vector;
	using std::string;
	using std::ifstream;
	using std::stringstream;

	const vector<string> id3v11_tag::ID3Genres = {
		"Blues",
		"Classic Rock",
		"Country",
		"Dance",
		"Disco",
		"Funk",
		"Grunge",
		"Hip-Hop",
		"Jazz",
		"Metal",
		"New Age",
		"Oldies",
		"Other",
		"Pop",
		"R&B",
		"Rap",
		"Reggae",
		"Rock",
		"Techno",
		"Industrial",
		"Alternative",
		"Ska",
		"Death Metal",
		"Pranks",
		"Soundtrack",
		"Euro-Techno",
		"Ambient",
		"Trip-Hop",
		"Vocal",
		"Jazz+Funk",
		"Fusion",
		"Trance",
		"Classical",
		"Instrumental",
		"Acid",
		"House",
		"Game",
		"Sound Clip",
		"Gospel",
		"Noise",
		"AlternRock",
		"Bass",
		"Soul",
		"Punk",
		"Space",
		"Meditative",
		"Instrumental Pop",
		"Instrumental Rock",
		"Ethnic",
		"Gothic",
		"Darkwave",
		"Techno-Industrial",
		"Electronic",
		"Pop-Folk",
		"Eurodance",
		"Dream",
		"Southern Rock",
		"Comedy",
		"Cult",
		"Gangsta",
		"Top 40",
		"Christian Rap",
		"Pop/Funk",
		"Jungle",
		"Native American",
		"Cabaret",
		"New Wave",
		"Psychedelic",
		"Rave",
		"Showtunes",
		"Trailer",
		"Lo-Fi",
		"Tribal",
		"Acid Punk",
		"Acid Jazz",
		"Polka",
		"Retro",
		"Musical",
		"Rock & Roll",
		"Hard Rock",
		"Folk",
		"Folk-Rock",
		"National Folk",
		"Swing",
		"Fast Fusion",
		"Bebob",
		"Latin",
		"Revival",
		"Celtic",
		"Bluegrass",
		"Avantgarde",
		"Gothic Rock",
		"Progressive Rock",
		"Psychedelic Rock",
		"Symphonic Rock",
		"Slow Rock",
		"Big Band",
		"Chorus",
		"Easy Listening",
		"Acoustic",
		"Humour",
		"Speech",
		"Chanson",
		"Opera",
		"Chamber Music",
		"Sonata",
		"Symphony",
		"Booty Bass",
		"Primus",
		"Porn Groove",
		"Satire",
		"Slow Jam",
		"Club",
		"Tango",
		"Samba",
		"Folklore",
		"Ballad",
		"Power Ballad",
		"Rhythmic Soul",
		"Freestyle",
		"Duet",
		"Punk Rock",
		"Drum Solo",
		"A Capella",
		"Euro-House",
		"Dance Hall",
		"Goa",
		"Drum & Bass",
		"Club-House",
		"Hardcore",
		"Terror",
		"Indie",
		"BritPop",
		"Negerpunk",
		"Polsk Punk",
		"Beat",
		"Christian Gangsta Rap",
		"Heavy Metal",
		"Black Metal",
		"Crossover",
		"Contemporary Christian",
		"Christian Rock",
		"Merengue",
		"Salsa",
		"Trash Metal",
		"Anime",
		"JPop",
		"SynthPop" };


	id3v11_tag::id3v11_tag()
	{
	}

	id3v11_tag::~id3v11_tag()
	{
	}



	std::string id3v11_tag::get_string(const char*str, int maxlen)
	{
		int len = 0;
		for (; len < maxlen && str[len] != '\0'; ++len);

		return string(str, len);
	}


	bool id3v11_tag::read(std::istream &is)
	{

		unsigned long pos = is.tellg();
		if (!head)
			head = std::make_shared<ID3V11_RAW_HEAD>();

		is.seekg(id3v1_head_offset, std::ios::end);


		is.read((char *)head.get(), id3v1_head_size);

		title = get_string(head->Title, 30);
		artist = get_string(head->Artist, 30);
		album = get_string(head->Album, 30);
		year = get_string(head->Year, 4);
		comment = get_string(head->Year, 28);
		track = head->Track;
		genre = head->Genre;
		is.seekg(pos, std::ios::beg);

		return true;

	}


	bool id3v11_tag::detect(std::istream &is)
	{
		unsigned long pos = is.tellg();
		bool ok = true;
		is.seekg(id3v1_head_offset, std::ios::end);

		char id[3];
		is.read(id, 3);
		if ((id[0] != 'T') || (id[1] != 'A') || (id[2] != 'G'))
			ok = false;

		is.seekg(pos, std::ios::beg);
		return ok;
	}

	void id3v11_tag::dump(std::ostream& os, const id3v11_tag& tag)
	{
		os << "[ id3v11 ]" << std::endl;

		os << "Title:" << tag.title << std::endl;
		os << "Artist:" << tag.artist << std::endl;
		os << "Album:" << tag.album << std::endl;
		os << "Year:" << tag.year << std::endl;
		os << "Comment:" << tag.comment << std::endl;
		os << "Track:" << (int)tag.track << std::endl;

		if (tag.genre < ID3Genres.size())
			os << "Genre:" << ID3Genres[tag.head->Genre] << std::endl;
	
	}


	void id3v11_tag::write(std::ofstream &os)
	{
		
		os.write((char *)head.get(), id3v1_head_size);
		printf("写入id3v1:%u",os.tellp());
	}


}