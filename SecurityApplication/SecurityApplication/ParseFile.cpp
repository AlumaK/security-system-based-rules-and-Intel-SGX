<<<<<<< HEAD
#include "stdafx.h"
#include "ParseFile.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

list<WordLocation> g_wordsList;

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
void findWordFromDictionary(map<string, string> dictionary, string path)
{
	ifstream myfile;
	myfile.open(path);
	string line;
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
}
=======
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
>>>>>>> 02a2bd6865d691f248192f21a43041cc72f71b62
