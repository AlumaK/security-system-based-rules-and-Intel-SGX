<<<<<<< HEAD
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

extern list<WordLocation> g_wordsList;

void findWordFromDictionary(map<string, string> dictionary, string path);

#endif // !_PARSE_FILE_H
=======
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
>>>>>>> 02a2bd6865d691f248192f21a43041cc72f71b62
