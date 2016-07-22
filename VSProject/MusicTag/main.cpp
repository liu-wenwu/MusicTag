// MusicTag.cpp : 定义控制台应用程序的入口点。
//

#include <tchar.h>
#include "id3v11_tag.h"
#include "id3v2_tag.h"
#include "tag_manager.h"

#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>



typedef bool(*cmd_func)();

musictag::tag_manager manager;



/*
编辑功能函数
*/

typedef void(*edit_func)(const std::string & value);

void edit_artist(const std::string & value)
{
	std::cout << "演唱者：" << value << std::endl;
	manager.set_artist(value);
}
void edit_title(const std::string & value)
{
	std::cout << "标题：" << value << std::endl;
}
void edit_album(const std::string & value)
{
	std::cout << "专辑：" << value << std::endl;
}
void edit_picture(const std::string & value)
{
	std::cout << "图片：" << value << std::endl;
}

std::unordered_map<std::string, edit_func> edit_map = {
	std::make_pair("artist", edit_artist),
	std::make_pair("title", edit_title),
	std::make_pair("album", edit_album),
	std::make_pair("picture", edit_picture),
};

void set_value(const std::string & item, const std::string & value)
{
	if (edit_map.find(item) != edit_map.end())
	{
		edit_func  func = edit_map[item];
		func(value);
	}
}


bool cmd_edit()
{
	std::cout << "\nenter edit mode,input 'q' to quit\n"
		<< "line format:\n"
		<< "item=value\n"
		<< "eg. comment=this is a nice song!\n"
		<< "[surpport item]\n"
		<< "artist title album comment picture picdesc\n"
		<< std::endl;

	char buff[2014];

	while (std::cin.getline(buff, 1024))
	{
		if (buff[0] == 'q')
			break;

		std::string opstr(buff);

		if (buff[0]=='\0')
			continue;
		int sep = opstr.find_first_of("=");
		if (sep <= 0)
		{
			std::cout << "format error"<< std::endl;
			continue;
		}

		std::string item = opstr.substr(0, sep);
		std::string value = opstr.substr(sep+1, opstr.size() - sep-1);

		set_value(item, value);
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

void cmd_invaild(const std::string &cmd)
{
	std::cout << cmd << " is invaild" << std::endl;
}

std::unordered_map<std::string, cmd_func> cmd_map = {
	std::make_pair("help", cmd_help),
	std::make_pair("clear", cmd_clear),
	std::make_pair("edit", cmd_edit),
};

int main(int argc, char* argv[])
{
	/*
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


	manager.load(filepath);*/

	manager.load("F:/Music/卡洛儿-假如爱有天意.mp3");
	std::cout << "-------    the tag info   ---------" << std::endl;
	manager.print_tags();

	std::cout << "-------    the tag info   ---------" << std::endl;

	std::cout << "enter cmd mode,input 'help' for more information" << std::endl;
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

