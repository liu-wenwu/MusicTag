#ifndef __TAG_H__
#define __TAG_H__

#include <string>



namespace musictag{

	class tag
	{
	public:
		tag(){}

		virtual ~tag(){};

		virtual bool read(std::istream &is) = 0;

	protected:

	private:


	};




}
#endif

