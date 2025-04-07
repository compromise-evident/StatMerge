/*StatMerge - version 3.0.0                                                     Run it: "apt install g++ geany". Open the .cpp in Geany. Hit F9 once. F5 to run.
Merge any files of any size & quantity based on most-occurring bytes.
By merging copies of a file, you retrieve the original file,
even as the copies become increasingly corrupted. */

#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
int main()
{	int raw_byte;
	char file_byte;
	ifstream in_stream;
	ofstream out_stream;
	
	//Gets path, fixes it, tries it.
	cout << "\nDrop/enter folder with any files:\n";
	char path[100000] = {'\0'}; cin.getline(path, 100000); if(path[0] == '\0') {cin.getline(path, 100000);}
	if(path[0] == '\'') {for(int bm = 0, a = 0; a < 100000; a++) {if(path[a] != '\'') {path[bm] = path[a]; if(path[bm] == '\\') {path[bm] = '\'';} bm++;}}}
	for(int a = 99999; a >= 0; a--) {if(path[a] != '\0') {if(path[a] == ' ') {path[a] = '\0';} break;}}
	in_stream.open(path); if(in_stream.fail() == true) {cout << "\nNo path " << path << "\n"; return 0;} in_stream.close();
	
	//Saves filenames to file via ls command.
	string ls = "ls "; ls += path; ls += " > f"; system(ls.c_str());
	
	//Gets number of files.
	long long number_of_files = 0;
	in_stream.open("f"); in_stream.get(file_byte);
	for(; in_stream.eof() == false; in_stream.get(file_byte)) {if(file_byte == '\n') {number_of_files++;}}
	in_stream.close();
	if(number_of_files == 0) {cout << "No files.\n"; return 0;}
	
	//Opens all files.
	vector <ifstream> in_stream_n;
	bool ok = true;
	in_stream.open("f");
	for(long long a = 0; a < number_of_files; a++)
	{	char line[100000]; in_stream.getline(line, 100000);
		string name = path; name += "/"; name += line;
		in_stream_n.push_back(ifstream(name));
		if(in_stream_n[a].fail() == true) {cout << "CAN'T OPEN " << name << "\n"; ok = false;}
	}
	in_stream.close();
	if(ok == false) {return 0;} remove("f");
	
	//Merges all files.
	out_stream.open("MERGED");
	int bytes_merged = 0; long long MB_merged = 0;
	for(;;)
	{	//Gets 1 byte from each file and tallies its occurrence.
		long long occurrence_table[256] = {0};
		for(long long a = 0; a < number_of_files; a++)
		{	in_stream_n[a].get(file_byte); if(in_stream_n[a].eof() == true) {out_stream.close(); cout << "Done.\n"; return 0;}
			raw_byte = file_byte; if(raw_byte < 0) {raw_byte += 256;} occurrence_table[raw_byte]++;
		}
		
		//Gets most-occurring byte.
		long long most_tallies = 0;
		for(int a = 0; a < 256; a++) {if(occurrence_table[a] > most_tallies) {most_tallies = occurrence_table[a]; raw_byte = a;}}
		
		//Saves most-occurring byte.
		if(raw_byte < 128) {out_stream.put(raw_byte      );}
		else               {out_stream.put(raw_byte - 256);}
		
		//Prints progress.
		bytes_merged++; if(bytes_merged == 1000000) {bytes_merged = 0; MB_merged++; cout << MB_merged << "MB merged...\n";}
	}
	out_stream.close();
}
