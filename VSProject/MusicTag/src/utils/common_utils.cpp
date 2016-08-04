#include "utils/common_utils.h"

namespace musictag
{

	unsigned int common_utils::get_big_edian24(char *x)
	{
		char tmp[4];
		tmp[0] = x[2];
		tmp[1] = x[1];
		tmp[2] = x[0];
		tmp[3] = 0;
		return *(unsigned int*)tmp;
	}



	unsigned int common_utils::get_big_edian32(char *x)
	{
		char tmp[4];
		tmp[0] = x[3];
		tmp[1] = x[2];
		tmp[2] = x[1];
		tmp[3] = x[0];
		return *(unsigned int*)tmp;
	}


}