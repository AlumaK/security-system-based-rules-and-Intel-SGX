#pragma once

#ifndef _PARSE_FILE_H
#define _PARSE_FILE_H

#include <stdio.h>
#include <string>
#include <list>
#include <map>

using namespace std;

class WordLocation
{
private:
	int lineNum;
	int charNum; // the location in the start of the word in the line
	string word;
public:
	WordLocation(int lineNum, int charNum, string word);
	int getLineNum() { return lineNum; }
	int getCharNum() { return charNum; }
	string getWord() { return word; }
};

list<WordLocation> findWordFromDictionary(map<string, string> dictionary);

#endif // !_PARSE_FILE_H
