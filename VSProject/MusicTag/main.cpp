

#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>


#include "tag_manager.h"



#define TAG_DEBUG 1
#define TAG_DEBUG_FILE "F:/Music/Disco.mp3"



/*
唯一全局变量
*/
musictag::tag_manager manager;


/*
编辑功能函数
*/

typedef void(*edit_func)(const std::string & value);

void edit_artist(const std::string & value)
{
	manager.set_artist(value);
}
void edit_title(const std::string & value)
{
	manager.set_title(value);
}
void edit_album(const std::string & value)
{
	manager.set_album(value);
}
void edit_comment(const std::string & value)
{
	manager.set_comment(value);
}
void edit_year(const std::string & value)
{
	manager.set_year(value);
}
void edit_track(const std::string & value)
{
	manager.set_track(value);
}


void edit_picture(const std::string & value)
{
	
	const std::vector<std::string> & types = musictag::id3v2_tag::get_picture_types();

	for (int i = 0; i < types.size(); ++i)
	{
		std::cout << "[" << i << "]" << types[i] << std::endl;
	}
	std::cout << "picture types list as above,input the type number:";
	int type = 0;
	do
	{
		std::cin >> type;
	} while (type < 0 && type >= types.size());

	char buff[2014];

	std::cout << "input description:";

	std::string desc(buff);
	while (std::cin.getline(buff, 1024))
	{
		if (desc.size()>0)
			break;
	}

	manager.set_picture(value,type,desc);
}

std::unordered_map<std::string, edit_func> edit_map = {
	std::make_pair("artist", edit_artist),
	std::make_pair("title", edit_title),
	std::make_pair("album", edit_album),
	std::make_pair("year", edit_year),
	std::make_pair("comment", edit_comment),
	std::make_pair("track", edit_track),
	std::make_pair("picture", edit_picture),
};

void set_value(const std::string & item, const std::string & value)
{

	if (edit_map.find(item) != edit_map.end())
	{
		edit_func  func = edit_map[item];
		func(value);
		std::cout << "\t[ok]" << std::endl;
	}
	else
		std::cout << item << " not surpport" << std::endl;
}

/*
命令功能函数
*/
typedef bool(*cmd_func)();
bool cmd_edit()
{
	std::cout << "\n[edit mode]\ninput 'q' to quit\n"
		<< "line format:\n"
		<< "item=value\n"
		<< "eg. comment=this is a nice song!\n"
		<< "[surpport item]\n"
		<< "artist title album comment year track genre picture picdesc\n"
		<< std::endl;

	char buff[2014];

	std::cout << "[edit]>";
	while (std::cin.getline(buff, 1024))
	{
		if (buff[0] == 'q')
			break;

		std::string opstr(buff);

		if (buff[0] == '\0')
			continue;
		int sep = opstr.find_first_of("=");
		if (sep <= 0)
		{
			std::cout << "format error" << std::endl;
			continue;
		}

		std::string item = opstr.substr(0, sep);
		std::string value = opstr.substr(sep + 1, opstr.size() - sep - 1);

		set_value(item, value);

		std::cout << "[edit]>";
	}


	return true;
}

bool cmd_help()
{
	std::cout << "[ help menu ]\n"
		<< "help	: get help for more information\n"
		<< "clear	: clear the screen\n"
		<< "list	: list all tags' information\n"
		<< "save 	: save the file\n"
		<< "edit	: enter the edit mode,input 'q' to quit\n"
		<< std::endl;
	return true;
}
bool cmd_clear()
{
	system("cls");
	return true;
}
bool cmd_save()
{
	manager.save();
	manager.reload();
	std::cout << " \t[ok]" << std::endl;
	return true;
}
bool cmd_list()
{
	manager.print_tags();

	return true;
}

void cmd_invaild(const std::string &cmd)
{
	std::cout << cmd << " is invaild" << std::endl;
}

std::unordered_map<std::string, cmd_func> cmd_map = {
	std::make_pair("help", cmd_help),
	std::make_pair("clear", cmd_clear),
	std::make_pair("list", cmd_list),
	std::make_pair("edit", cmd_edit),
	std::make_pair("save", cmd_save),
};

int main(int argc, char* argv[])
{

#if (TAG_DEBUG)
	manager.load(TAG_DEBUG_FILE);
#else

	if (argc != 2)
	{
		std::cout << "MusicTag V1.0 by liu-wenwu\n"
			"usage:musictag [INPUT]\n"
			<< std::endl;
		return 1;
	}

	std::string filepath(argv[1]);


	std::ifstream ifs(filepath);

	if (!ifs)
	{
		std::cout << filepath << " read failed!!!" << std::endl;
		return 1;
	}

	manager.load(filepath);
#endif



	std::cout << "-------    the tag info   ---------" << std::endl;
	manager.print_tags();

	std::cout << "-------    the tag info   ---------" << std::endl;

	std::cout << "[cmd mode]\ninput 'help' for more information" << std::endl;
	std::cout << ">";

	std::string cmd;

	while (std::cin >> cmd)
	{
		if (cmd_map.find(cmd) != cmd_map.end())
		{
			cmd_func func = cmd_map[cmd];
			func();
		}
		else
			cmd_invaild(cmd);
		std::cout << ">";
	}

	return 0;
}



