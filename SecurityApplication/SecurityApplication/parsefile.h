#pragma once

#ifndef _PARSE_FILE_H
#define _PARSE_FILE_H

#include <stdio.h>
#include <string>
#include <list>
#include <map>
#include "Dictionary.h"

using namespace std;

class WordData
{
private:
	int lineNum;
	int charNum; // the location in the start of the word in the line
	string word;
public:
	WordData(int lineNum, int charNum, string word);
	int getLineNum() { return lineNum; }
	int getCharNum() { return charNum; }
	string getWord() { return word; }
};

extern list<WordData> g_wordsList;

void findWordFromDictionary(map<string, string> dictionary, string path);
void findWordInLine(string line, int lineNum);

#endif // !_PARSE_FILE_H
