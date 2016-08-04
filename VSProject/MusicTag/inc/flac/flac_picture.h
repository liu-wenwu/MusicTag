#ifndef __FLAC_PICTURE_H__
#define __FLAC_PICTURE_H__

#include <iostream>
#include <vector>
#include <string>

namespace musictag{

	class flac_picture
	{
	public:

		enum FLAC_PicType
		{
			Other,
			FileIcon32X32PNG,
			OtherFileIcon,
			FrontCover,
			BackCover,
			LeafletPage,
			Media,
			LeadArtist,
			Artist,
			Conductor,
			Band,
			Composer,
			Lyricist,
			RecordingLocation,
			DuringRecording,
			DuringPerformance,
			MovieOrvideoScreenCapture,
			ABrightColouredFish,
			Illustration,
			BandOrArtistLogotype,
			PublisherOrStudioLogotype
		};


		void read(std::istream &is);
		void write(std::ostream &os);


		flac_picture(int width,int height,const std::string& mime, const std::vector<char>&data) 
			:pic_width(width), pic_height(height), pic_mine(mime), pic_data(data)
		{
		}

		void print(std::ostream &os)
		{
			os << pic_mine << " : " << pic_width << "x" << pic_height << std::endl;
		}


	private:
		unsigned int pic_width;
		unsigned int pic_height;
		unsigned int pic_color_depth;
		std::string pic_mine;
		std::vector<char> pic_data;
		

	};

}




#endif