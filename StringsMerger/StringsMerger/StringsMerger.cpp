#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <cctype>
#include <map>
using namespace std;

pair<int, string> divide(const string& line)
{
	int id = 0;
	for (size_t i = 0; i < line.length() && i < 10; i++) {
		if (isdigit(line[i])) {
			id *= 10;
			id += line[i] - '0';
		} else if (line[i] == ',') {
			return make_pair(id, line.substr(i+1));
		} else {
			return make_pair(-1, "");  // error 
		}
	}
	return make_pair(-1, "");  // error
}

bool readFile(const string& file_path, map<int, pair<string, string> >& out, bool is_primary_lang)
{
	ifstream ifs(file_path);
	if (!ifs.is_open()) {
		cout << "Failed to open input file: " << file_path << endl;
		return false;
	}
	string line;
	int unread_lines = 0;
	while (getline(ifs, line)) {
		pair<int, string> divided = divide(line);
		if (divided.first != -1) {
			if (is_primary_lang) {
				out[divided.first].first = divided.second;
			} else {
				out[divided.first].second = divided.second;
			}
		} else {
			unread_lines++;
		}
	}
	if (unread_lines > 1) {
		cout << "Too many unread lines (" << unread_lines << ")" << endl;
		return false;
	}
	return true;
}

bool writeFile(const string& file_path, const map<int, pair<string, string> >& m)
{
	ofstream ofs(file_path);
	if (!ofs.is_open()) {
		cout << "Failed to open output file: " << file_path << endl;
		return false;
	}
	ofs << "Index,Text" << endl;
	for (map<int, pair<string, string> >::const_iterator it = m.begin(); it != m.end(); ++it) {
		int id = it->first;
		const string& primary_lang = it->second.first;
		const string& secondary_lang = it->second.second;

		// Modify this line to change output format.
		ofs << id << "," << primary_lang << " (" << secondary_lang << ")" << endl;
	}
	return true;
}

int main(int argc, char* argv[])
{
	if (argc != 4) {
		cout << "Usage: StringMerger.exe <primary language's csv file> <secondary language's csv file> <output csv file>" << endl;
		return -1;
	}
	map<int, pair<string, string>> m;
	if (!readFile(argv[1], m, true)) return -1;
	if (!readFile(argv[2], m, false)) return -1;
	if (!writeFile(argv[3], m)) return -1;
	cout << "Success! The merged file is " << argv[3] << "." << endl;
	return 0;
}