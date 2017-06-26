#include "stdafx.h"
#include "parsefile.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

bool g_isComment = false;
list<WordData> g_wordsList;

WordData::WordData(int lineNum, int charNum, string word)
{
	WordData::lineNum = lineNum;
	WordData::charNum = charNum;
	WordData::word = word;
}

/*
* this function
*
*/

void findWordInLine(string line, int lineNum)
{
	string str = "";
	int len = line.length();
	for (int i = 0; i < len; i++)
	{
		char c = line.at(i);
		if (g_isComment)
		{
			if (c == '*' && i < (len - 1))
			{
				c = line.at(i + 1);
				if (c == '/')
				{
					g_isComment = false;
				}
				i++;
			}
		}
		else if (c == ' ' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ';' || i == (len - 1))
		{
			if (g_dictionary.count(str) > 0)
			{
				cout << str << endl;
				cout << g_dictionary.at(str) << endl;
				int charNum = i - str.length();
				cout << line.at(charNum) << endl;
				WordData* data = new WordData(lineNum, charNum, str);
				g_wordsList.insert(data);
			}
		}
		else if (c == '#')
		{
			break;
		}
		else if ((c == '/' && i < (len - 1) && line.at(i + 1) == '/') || (c == '/' && i < (len - 1) && line.at(i + 1) == '*'))
		{
			i++;
			c = line.at(i);
			if (c == '/')
			{
				break;
			}
			else if (c == '*')
			{
				g_isComment = true;
				cout << "is comment" << endl;
			}
			str = "";
		}
		else
		{
			str += line.at(i);
		}
	}
}
void findWordFromDictionary(map<string, string> dictionary, string path)
{
	ifstream myfile;
	myfile.open(path);
	string line;
	int lineNum = 0;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			findWordInLine(line, lineNum);
			lineNum++;
		}
		myfile.close();
	}

	else cout << "Unable to open file";
	myfile.close();
}
