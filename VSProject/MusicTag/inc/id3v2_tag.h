#ifndef __id3v2_tag_TAG_H__
#define __id3v2_tag_TAG_H__

#include "tag.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <iostream>

typedef int id3v2_id;

#define ID3V2_MAKE_ID(a,b,c,d) ( a<< 4| b<<2 | c<<1 |d )

#define ID3V2_GET_ID(a) ( a[0]<< 4| a[1]<<2 | a[2]<<1 |a[3] )


/*

帧识别ID

*/

/* 标题 */
#define ID3V2_TITLE			ID3V2_MAKE_ID('T','I','T','2') 
/* 专辑 */
#define ID3V2_ALBUM			ID3V2_MAKE_ID('T','A','L','B') 
/* 艺术家 */
#define ID3V2_ARTIST		ID3V2_MAKE_ID('T','P','E','1') 
/* 专辑艺人 */
#define ID3V2_ALBUMARTIST	ID3V2_MAKE_ID('T','P','E','2') 
/* 年份 */
#define ID3V2_YEAR			ID3V2_MAKE_ID('T','Y','E','R') 
/* 日期 */
#define ID3V2_DATE			ID3V2_MAKE_ID('T','D','A','T') 
/* 作曲家 */
#define ID3V2_COMPOSER		ID3V2_MAKE_ID('T','C','O','M') 
/* 光盘编号 */
#define ID3V2_DISCNUMBER	ID3V2_MAKE_ID('T','P','O','S') 
/* 编码器 */
#define ID3V2_ENCODER		ID3V2_MAKE_ID('T','E','N','C') 
/* 分类 */
#define ID3V2_CONTENTTYPE	ID3V2_MAKE_ID('T','C','O','N') 
/* 音轨 */
#define ID3V2_TRACK	ID3V2_MAKE_ID('T','R','C','K') 
/* 备注 */
#define ID3V2_COMMENT		ID3V2_MAKE_ID('C','O','M','M') 
/* 专辑图片 */
#define ID3V2_PICTURE		ID3V2_MAKE_ID('A','P','I','C') 

/*

字符串编码格式

*/
#define ID3V2_ISO88591		0
#define ID3V2_UTF16LE		1
#define ID3V2_UTF16BE		2
#define ID3V2_UTF8			3

namespace musictag{

	/* 帧 */
	class id3v2_frame
	{
	public:
		id3v2_frame()
		{
		}

		id3v2_frame(const std::string &id) :tid(id)
		{}
		virtual void parse(const std::vector<char> &data){};
		virtual void print(std::ostream &os){};

		virtual void write(std::ofstream &os){};
	protected:
		std::string tid;
	};


	class id3v2_text_frame :public id3v2_frame
	{

	public:
		id3v2_text_frame(const std::string &id, const std::vector<char> &data) :id3v2_frame(id)
		{
			parse(data);
		}
		id3v2_text_frame(const std::string &id, const std::string &data) :id3v2_frame(id), str(data)
		{
		}
		void set_text(const std::string &data)
		{
			str = data;
		}
		
		virtual void write(std::ofstream &os);

	private:

		void parse(const std::vector<char> &data) override;
		void print(std::ostream &os) override
		{
			os << tid << " : " << str << std::endl;
		};
		std::string str;
	};

	class id3v2_url_frame :public id3v2_frame
	{
	public:
		id3v2_url_frame(const std::vector<char> &data) :id3v2_frame()
		{
			parse(data);
		}


	private:

		void parse(const std::vector<char> &data) override;
	};

	class id3v2_picture_frame :public id3v2_frame
	{
	public:
		id3v2_picture_frame(const std::string &id, const std::vector<char> &data) :id3v2_frame(id)
		{
			parse(data);
		}

	private:

		std::string mime;
		char pic_type;
		std::string desc;
		std::vector<char> pic_data;
	
		void parse(const std::vector<char> &data) override;
		void print(std::ostream &os) override
		{
			os << tid << " : mime(" << mime << "),desc(" << desc << ") size(" << pic_data .size()<< ")" << std::endl;
		};
	};



	class id3v2_tag : public tag
	{
	public:
		id3v2_tag();
		~id3v2_tag();
		bool read(std::istream &is) override;
		static bool detect(std::istream &is);

		void write(std::ofstream &os);

		friend std::ostream & operator<<(std::ostream &os, const id3v2_tag& tag)
		{
			dump(os, tag);
			return os;
		}

		int get_size()
		{
			return size + 10;
		}


		void set_item(id3v2_id id, const std::string &v);
	private:

		int subversion;//id3v2.x
		static void dump(std::ostream& os, const id3v2_tag& tag);
		std::unordered_map<id3v2_id, std::shared_ptr<id3v2_frame> > frames;
		int size = 0;
	};


}


#endif // id3v2_tag_H


