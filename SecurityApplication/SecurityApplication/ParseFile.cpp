#include "stdafx.h"
#include "ParseFile.h"
#include <iostream>
#include <fstream>

using namespace std;

WordLocation::WordLocation(int lineNum, int charNum, string word)
{
	WordLocation::lineNum = lineNum;
	WordLocation::charNum = charNum;
	WordLocation::word = word;
}

/* 
 * this function 
 * 
 */
list<WordLocation>* findWordFromDictionary(map<string, string> dictionary, string path)
{
	list<WordLocation>* wordsList = new list<WordLocation>;
	ofstream myfile;
	myfile.open(path);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			cout << line << '\n';
		}
		myfile.close();
	}

	else cout << "Unable to open file";
	myfile.close();
	return wordsList;
}
