#ifndef __ID3V2_H__
#define __ID3V2_H__
#include <vector>

namespace musictag{




	typedef int id3v2_id;


#define ID3V2_MAKE_ID(a,b,c,d) ( a<< 24| b<<16| c<<8 |d )

#define ID3V2_GET_ID(a) ( a[0]<< 24| a[1]<<16 | a[2]<<8 |a[3] )

#define getTagSize(x) (((unsigned int)x[0]<<21)+((unsigned int)x[1]<<14)+((unsigned int)x[2]<<7)+x[3])
#define getFrameSize(x) (((unsigned int)x[0]<<24)+((unsigned int)x[1]<<16)+((unsigned int)x[2]<<8)+x[3])

	/*

	帧识别ID

	*/

	/* 标题 */
	const int ID3V2_TITLE = ID3V2_MAKE_ID('T', 'I', 'T', '2');
	/* 专辑 */
	const int  ID3V2_ALBUM = ID3V2_MAKE_ID('T', 'A', 'L', 'B');
	/* 艺术家 */
	const int  ID3V2_ARTIST = ID3V2_MAKE_ID('T', 'P', 'E', '1');
	/* 专辑艺人 */
	const int  ID3V2_ALBUMARTIST = ID3V2_MAKE_ID('T', 'P', 'E', '2');
	/* 年份 */
	const int  ID3V2_YEAR = ID3V2_MAKE_ID('T', 'Y', 'E', 'R');
	/* 日期 */
	const int  ID3V2_DATE = ID3V2_MAKE_ID('T', 'D', 'A', 'T');
	/* 作曲家 */
	const int ID3V2_COMPOSER = ID3V2_MAKE_ID('T', 'C', 'O', 'M');
	/* 光盘编号 */
	const int ID3V2_DISCNUMBER = ID3V2_MAKE_ID('T', 'P', 'O', 'S');
	/* 编码器 */
	const int ID3V2_ENCODER = ID3V2_MAKE_ID('T', 'E', 'N', 'C');
	/* 分类 */
	const int ID3V2_CONTENTTYPE = ID3V2_MAKE_ID('T', 'C', 'O', 'N');
	/* 音轨 */
	const int ID3V2_TRACK = ID3V2_MAKE_ID('T', 'R', 'C', 'K');
	/* 备注 */
	const int ID3V2_COMMENT = ID3V2_MAKE_ID('C', 'O', 'M', 'M');
	/* 专辑图片 */
	const int ID3V2_PICTURE = ID3V2_MAKE_ID('A', 'P', 'I', 'C');

	/*

	字符串编码格式

	*/

	enum ID3V2_CODEC
	{
		ID3V2_ISO88591,
		ID3V2_UTF16LE,
		ID3V2_UTF16BE,
		ID3V2_UTF8
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
		id3v2_raw_frame(const id3v2_id id)
		{
			ID[0] = (id >> 24) & 0xff;
			ID[1] = (id >> 16) & 0xff;
			ID[2] = (id >> 8) & 0xff;
			ID[3] = (id >> 0) & 0xff;
			Flags[0] = Flags[1] = 0;
		};

	}id3v2_raw_frame;


}


#endif