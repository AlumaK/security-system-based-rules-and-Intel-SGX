#include "stdafx.h"
#include "parsefile.h"
#include <iostream>
#include <fstream>
#include <string>
#include <queue>

bool g_isComment = false;
std::list<WordData*> g_wordsList;
std::map<std::string, std::string> g_executeFilesFullPath;
std::list<std::string> g_executeFilesList;

WordData::WordData(int lineNum, int charNum, std::string word)
{
	WordData::lineNum = lineNum;
	WordData::charNum = charNum;
	WordData::word = word;
}

int WordData::getLineNum() 
{ 
	return lineNum;
}
int WordData::getCharNum()
{ 
	return charNum;
}
std::string WordData::getWord() 
{
	return word; 
}

/*
* this function
*
*/

void findWordInLine(std::string line, int lineNum, std::ofstream* outputFile)
{
	std::string str = "";
	std::string newLine = "";
	bool isSuspiciousFuncFound = false;
	int len = line.length();
	std::queue<char> q;
	bool isMain = false;
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
			if (isMain && c == '}')
			{
				q.pop();
				if (isMain && q.size() == 0)
				{
					*outputFile << "	if (SGX_SUCCESS != sgx_destroy_enclave(eid))\n";
					*outputFile << "	{\n";
					*outputFile << "		return -1; \n";
					*outputFile << "	}\n";
				}
			}
			else if (isMain && c == '{')
			{
				q.push('{');
			}

			if (g_dictionary.count(str) > 0)
			{
				int charNum = i - str.length();
				WordData* data = new WordData(lineNum, charNum, str);
				g_wordsList.push_back(data);
				replaceLineInOutputFile(line, str, charNum, outputFile);
				isSuspiciousFuncFound = true;
				if (str.compare("main") == 0)
				{
					isMain = true;
					q.push('{');
					*outputFile << line + "\n";
					*outputFile << "sgx_status_t res = SGX_SUCCESS;\n";
					*outputFile << "	// Create the Enclave with above launch token.\n";
					*outputFile << "	res = createEnclave(&eid); \n";
					*outputFile << "if (res != SGX_SUCCESS)\n{\n";
					*outputFile << "	printf(\"App: error %#x, failed to create enclave.\n\", res); \n";
					*outputFile << "	return -1; \n}\n";
				}
				return;
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
			}
			str = "";
		}
		else
		{
			if (c == '_' && i < (len - 1) && line.at(i + 1) == '_')
			{
				return;
			}
			str += line.at(i);
		}
	}
	if (!isSuspiciousFuncFound)
	{
		*outputFile << line + "\n";
	}
}

void parseFile(std::map<std::string, std::string> dictionary, std::string inputFilePath, std::string outputFilePath)
{
	std::ifstream inputFile;
	std::ofstream outputFile;
	outputFile.open(outputFilePath);
	inputFile.open(inputFilePath);
	std::string line;
	int lineNum = 0;
	std::string str = "// SecureApplication.cpp : Defines the entry point for the console application.\n";
	str += "#pragma check_stack(off)\n";
	str += "#include \"stdafx.h\"\n";
	str += "#include \"sgx_urts.h\"\n";
	str += "#include \"SecureFunctions_u.h\"\n";
	str += "#include <tchar.h>\n";
	str += "#include <stdint.h>\n";
	str += "#include <stdio.h>\n";
	str += "#include <stdlib.h>\n";
	str += "#define ENCLAVE_FILE _T(\"SecureFunctions.signed.dll\")\n";
	str += "using namespace std;\n";
	str += "/* OCall functions */\n";
	str += "void ocall_print_string(const char *str)\n{\n";
	str += "	/* Proxy/Bridge will check the length and null-terminate\n";
	str += "	* the input string to prevent buffer overflow.\n";
	str += "	*/\n";
	str += "	printf(\"%s\", str);\n}\n";
	str += "sgx_status_t createEnclave(sgx_enclave_id_t *eid) {\n";
	str += "	sgx_status_t		ret = SGX_SUCCESS;\n";
	str += "	sgx_launch_token_t	token = { 0 };\n";
	str += "	int					updated = 0; \n";
	str += "	ret = sgx_create_enclave(ENCLAVE_FILE, SGX_DEBUG_FLAG, &token, &updated, eid, NULL); \n";
	str += "	return ret;\n}\n";
	str += "sgx_enclave_id_t eid;\n\n";
	
	if (inputFile.is_open() && outputFile.is_open())
	{
		outputFile << str;
		while (getline(inputFile, line))
		{
			findWordInLine(line, lineNum, &outputFile);
			lineNum++;
		}
	}

	else
	{
		if (inputFile.is_open())
		{
			std::cout << "Unable to open output file";
		}
		else
		{
			std::cout << "Unable to open input file";
		}
	}
	inputFile.close();
	outputFile.close();
}

void findAllExecuteFiles(std::string executeFilesListPath)
{
	std::ifstream myfile;
	myfile.open(executeFilesListPath);
	std::string line;
	std::string path[50];
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			std::string str = "";
			int len = line.length();
			int strNum = 0;
			for (int i = 0; i < len; i++)
			{
				char c = line.at(i);
				if (c == '\\')
				{
					path[strNum] = str;
					str = "";
					strNum++;
				}
				else
				{
					str += c;
				}
				if (i == (len - 1))
				{
					path[strNum] = str;
					str = "";
					strNum++;
				}
			}

			std::string fullPath = "";
			std::string fileName;
			for (int i = 0; i < strNum; i++)
			{
				fullPath += path[i];
				if (i < (strNum - 1))
				{
					fullPath += '\\';
				}
				else
				{
					fileName = path[strNum];
				}
			}
			g_executeFilesFullPath[fileName] = fullPath;
			g_executeFilesList.push_back(fileName);
		}
		myfile.close();
	}

	else
	{
		std::cout << "Unable to open file";
	}
	myfile.close();
}


void deleteLists()
{
	for (std::list<WordData*>::iterator list_iter = g_wordsList.begin(); list_iter != g_wordsList.end(); list_iter++)
	{
		delete *list_iter;
	}
}


std::string getNewCallFunction(std::string oldFunctionName, std::list<std::string> params)
{
	std::string newCallFunc = "";
	newCallFunc += g_dictionary[oldFunctionName];
	newCallFunc += "(eid, ";
	for (std::list<std::string>::iterator list_iter = params.begin(); list_iter != params.end(); list_iter++)
	{
		newCallFunc += (*list_iter + ",");
	}
	newCallFunc.pop_back();
	newCallFunc += ')';
	printf("%s\n", newCallFunc);
	return newCallFunc;
}


std::list<std::string> getFuncParams(std::string func)
{
	std::queue<char> q;
	std::list<std::string> params;
	std::string p;
	for (int i = 0; i < func.length(); i++)
	{
		char c = func.at(i);
		if (c == '(')
		{
			if (q.size() != 0)
			{
				p += c;
			}
			q.push(c);
		}
		else if (c == ')')
		{
			p += c;
			q.pop();
		}
		else if (c == ',')
		{
			params.push_back(p);
			printf("%s\n", p);
			p = "";
		}
		else if (q.size() != 0)	//
		{
			p += c;
		}
		else //if it is before the start of parse the params func
		{
			continue;
		}
		if (q.size() == 0)
		{
			return params;
		}
	}
	return params;
}

void replaceLineInOutputFile(std::string line, std::string str, int charNum, std::ofstream* outputFile)
{
	if (str.compare("strcpy") == 0)
	{
		std::queue<char> q;
		std::string param1 = "";
		std::string param2 = "";
		bool isParam1 = true;
		int len = line.length();
		char c;
		int i;
		for (i = charNum + 6; i < len; i++)
		{
			c = line.at(i);
			if (c == '(')
			{
				q.push(c);
				if (q.size() == 1)
				{
					continue;
				}
			}
			else if (c == ')')
			{
				q.pop();
			}

			if (q.size() == 0)
			{
				break;
			}

			if (c == ',')
			{
				isParam1 = false;
			}
			else
			{
				if (isParam1)
				{
					param1 += c;
				}
				else // is param2
				{
					param2 += c;
				}
			}
		}
		std::string newLine = line.substr(0, charNum);
		newLine += "enclaveStrcpy(eid, " + param1 + ", " + param2 + ");\n";
		*outputFile << "//" + line + "\n";
		*outputFile << newLine;
	}
	else if (str.compare("/") == 0)
	{
		for (int i = 0; i < line.length(); i++)
		{

		}
	}
	else if (str.compare("int32_t") == 0)
	{
		int i;
		char c;
		int len = line.length();
		std::string paramName = "";
		bool isAfterEqualSign = false, isFunction = false;
		for (i = charNum + 7; i < len; i++)
		{
			c = line.at(i);
			if (c == '=')
			{
				isAfterEqualSign = true;
				break;
			}
			else if (c == ';')
			{
				break;
			}
			else if (!isAfterEqualSign && c == '*')
			{
				*outputFile << line + "\n";
				return;
			}
			else if (c == '(')
			{
				isFunction = true;
			}
		}
		if (isFunction)
		{
			*outputFile << line + "\n";
		}
		else
		{
			std::string newLine = line.substr(0, i);
			newLine += " = 0;\n";
			*outputFile << newLine;
		}
	}
}