/// StatMerge - merge any models or files of any quantity, based on             Run it: "apt install g++ geany". Open the .cpp in Geany. Hit F9 once. F5 to run.
///             Byte occurrence/average. Or preserve any file by
///             storing copies. StatMerge can retrieve it
///             from those copies even if corrupted.


// Version 2.2.0
#include <fstream>
#include <iostream>
using namespace std;
int main()
{	/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//////////////////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  /////////////////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\    ////////////////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\      ///////////////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\        //////////////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\            ////////////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\              ///////////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\       your       /////////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\\\\\       controls       ///////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\                              ///////////////////////
	\\\\\\\\\\\\\\\\\\                                        ////////////////*/
	
	bool average_instead_of_occurrence = false; //DEFAULT = false.  (false means
	//occurrence-based merging;  for retrieving original from corrupted copies.)
	
	/*////////////////                                        \\\\\\\\\\\\\\\\\\
	///////////////////////                              \\\\\\\\\\\\\\\\\\\\\\\
	///////////////////////////                      \\\\\\\\\\\\\\\\\\\\\\\\\\\
	/////////////////////////////                  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	///////////////////////////////              \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	////////////////////////////////            \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	//////////////////////////////////        \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	///////////////////////////////////      \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	////////////////////////////////////    \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	/////////////////////////////////////  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	//////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
	
	ifstream in_stream;
	ofstream out_stream;
	
	//Gets path to FOLDER from user.
	cout << "\nDrop/enter FOLDER with files of equal size:\n";
	
	//..........Gets path then fixes it if drag-n-dropped, regardless of single-quote presence and "enter"
	//..........not being cleared, meaning you can have options before this, where the user presses enter.
	char path_to_file[10000] = {'\0'};
	{	for(int a = 0; a < 10000; a++) {path_to_file[a] = '\0';}
		cin.getline(path_to_file, 10000);
		if(path_to_file[0] == '\0')
		{	for(int a = 0; a < 10000; a++) {path_to_file[a] = '\0';}
			cin.getline(path_to_file, 10000);
		}
		if(path_to_file[0] == '\0') {cout << "\nNo path given.\n"; return 0;}
		
		//..........Removes last space in path_to_file[].
		bool existence_of_last_space = false;
		for(int a = 9999; a > 0; a--)
		{	if(path_to_file[a] != '\0')
			{	if(path_to_file[a] == ' ') {path_to_file[a] = '\0'; existence_of_last_space = true;}
				break;
			}
		}
		
		//..........Removes encapsulating single-quotes in path_to_file[].
		bool existence_of_encapsulating_single_quotes = false;
		if(path_to_file[0] == '\'')
		{	for(int a = 0; a < 9999; a++)
			{	path_to_file[a] = path_to_file[a + 1];
				if(path_to_file[a] == '\0') 
				{	if(path_to_file[a - 1] != '\'') {cout << "\nBad path.\n"; return 0;}
					path_to_file[a - 1] = '\0';
					existence_of_encapsulating_single_quotes = true;
					break;
				}
			}
		}
		
		//..........Replaces all "'\''" with "'" in path_to_file[].
		int single_quote_quantity = 0;
		for(int a = 0; a < 10000; a++)
		{	if(path_to_file[a] == '\'') {single_quote_quantity++;}
		}
		
		if((single_quote_quantity                     >    0)
		&& (existence_of_last_space                  == true)
		&& (existence_of_encapsulating_single_quotes == true))
		{	if((single_quote_quantity % 3) != 0) {cout << "\nBad path.\n"; return 0;}
			
			for(int a = 0; a < 9997; a++)
			{	if(path_to_file[a] == '\'')
				{	int temp = (a + 1);
					for(; temp < 9997; temp++)
					{	path_to_file[temp] = path_to_file[temp + 3];
					}
				}
			}
		}
	}
	
	//Checks if FOLDER exists.
	in_stream.open(path_to_file);
	if(in_stream.fail() == true) {in_stream.close(); cout << "\n\nNo such directory.\n"; return 0;}
	in_stream.close();
	
	//Gets location of the first encountered end-null coming from the left in path_to_file[].
	int path_to_file_null_bookmark;
	for(int a = 0; a < 10000; a++) {if(path_to_file[a] == '\0') {path_to_file_null_bookmark = a; break;}}
	
	//Gets list of file names from given directory.
	char ls[10000] = {"ls "};
	for(int a = 0; path_to_file[a] != '\0'; a++) {ls[a + 3] = path_to_file[a];} //..........Appends given path.
	ls[path_to_file_null_bookmark + 3] = ' '; //..........Appends commands.
	ls[path_to_file_null_bookmark + 4] = '>';
	ls[path_to_file_null_bookmark + 5] = ' ';
	ls[path_to_file_null_bookmark + 6] = 'f';
	system(ls);
	
	//Checks if FOLDER is empty.
	in_stream.open("f");
	char sniffed_one_file_character;
	in_stream.get(sniffed_one_file_character);
	if(in_stream.eof() == true) {in_stream.close(); cout << "\n\nNothing to process, the FOLDER is empty.\n"; return 0;}
	in_stream.close();
	
	//Counts number of files based on ls.
	long long number_of_files = 0;
	char garbage_byte;
	char temp_garbage_byte;
	in_stream.open("f");
	in_stream.get(garbage_byte);
	for(; in_stream.eof() == false;)
	{	if(garbage_byte == '\n') {number_of_files++;}
		temp_garbage_byte = garbage_byte;
		in_stream.get(garbage_byte);
	}
	in_stream.close();
	
	if(temp_garbage_byte != '\n') {number_of_files++                                                                      ;}
	if(number_of_files < 2      ) {cout << "\nMust have 2 or more files to merge, preferably 3+.\n"; remove("f"); return 0;}
	
	
	
	
	
	//This entire section can be removed.
	//Checks if all files are of equal size.
	cout << "\nChecking file sizes...\n";
	char COPY_path_to_file[10000];
	for(int a = 0; a < 10000; a++) {COPY_path_to_file[a] = path_to_file[a];}
	
	int COPY_path_to_file_null_bookmark = path_to_file_null_bookmark;
	COPY_path_to_file[COPY_path_to_file_null_bookmark] = '/';
	COPY_path_to_file_null_bookmark++;
	
	//..........Runs through all files.
	long long COPY_file_name_bytes_read_bookmark = -1;
	long long size_of_first_file;
	long long total_bytes = 0;
	bool ran_once = false;
	for(long long a = 0; a < number_of_files; a++)
	{	//..........Loads COPY_path_to_file[] with file name.
		in_stream.open("f");
		COPY_file_name_bytes_read_bookmark++;
		in_stream.seekg(COPY_file_name_bytes_read_bookmark, std::ios::beg); //..........Skips name Bytes that have been read.
		
		int COPY_path_to_file_write_bookmark = COPY_path_to_file_null_bookmark;
		
		in_stream.get(garbage_byte);
		for(; garbage_byte != '\n';)
		{	COPY_path_to_file[COPY_path_to_file_write_bookmark] = garbage_byte;
			COPY_path_to_file_write_bookmark++;
			COPY_file_name_bytes_read_bookmark++;
			in_stream.get(garbage_byte);
			
			if(in_stream.eof() == true) {cout << "\nError 1\n"; return 0;}
		}
		in_stream.close();
		
		COPY_path_to_file[COPY_path_to_file_write_bookmark] = '\0';
		
		//..........Gets file size.
		total_bytes = 0;
		in_stream.open(COPY_path_to_file);
		in_stream.get(garbage_byte);
		for(; in_stream.eof() == false;)
		{	in_stream.get(garbage_byte);
			total_bytes++;
		}
		in_stream.close();
		
		//..........Compares file size.
		if(ran_once == false) {size_of_first_file = total_bytes; ran_once = true;}
		if(size_of_first_file != total_bytes)
		{	cout << "\nFAILED! The following file is the first to be of\n"
			     << "a different size compared to ALL files before it:\n" ;
			
			for(int a = 0; COPY_path_to_file[a] != '\0'; a++) {cout << COPY_path_to_file[a];}
			
			cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
			     << "\nSize of that file = " << total_bytes
			     << "\nSizes prior to it = " << size_of_first_file
			     << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
			
			remove("f");
			return 0;
		}
	}
	
	cout << "Merging " << number_of_files << " files, each " << total_bytes << " Bytes...\n";
	
	
	
	
	
	/*####*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*######
	##'                                         '##
	#                   Begins.                   #
	#                                             #
	##,                                         ,##
	####*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*##*/
	path_to_file[path_to_file_null_bookmark] = '/';
	path_to_file_null_bookmark++;
	bool looped_at_least_once = false;
	long long bytes_to_skip = 0;
	long long megabytes_processed_announcement = 0;
	long long megabytes_processed_counter      = 0;
	long long byte_differences_announced       = 0;
	static long long distribution[250000][256]; //..........Can consume 512MB RAM.
	for(;;)
	{	//..........Clears distribution[][].
		for(int a = 0; a < 250000; a++)
		{	for(int b = 0; b < 256; b++) {distribution[a][b] = 0;}
		}
		
		//..........Runs through all files.
		long long file_name_bytes_read_bookmark = -1;
		for(long long a = 0; a < number_of_files; a++)
		{	//..........Loads path_to_file[] with file name.
			in_stream.open("f");
			file_name_bytes_read_bookmark++;
			in_stream.seekg(file_name_bytes_read_bookmark, std::ios::beg); //..........Skips name Bytes that have been read.
			int path_to_file_write_bookmark = path_to_file_null_bookmark;
			
			in_stream.get(garbage_byte);
			for(; garbage_byte != '\n';)
			{	path_to_file[path_to_file_write_bookmark] = garbage_byte;
				path_to_file_write_bookmark++;
				file_name_bytes_read_bookmark++;
				in_stream.get(garbage_byte);
				
				if(in_stream.eof() == true) {cout << "\nError 1\n"; return 0;}
			}
			in_stream.close();
			
			path_to_file[path_to_file_write_bookmark] = '\0';
			
			//..........Loads distribution[][]. 250kB are read from each file, and Byte occurrences are set. Next round is the next unread 250kB. From each file...
			int garbage_byte_normal;
			in_stream.open(path_to_file);
			in_stream.seekg(bytes_to_skip, std::ios::beg); //..........Skips FILE Bytes that have been read.
			in_stream.get(garbage_byte);
			for(int b = 0; in_stream.eof() == false; b++)
			{	if(b == 250000) {break;}
				garbage_byte_normal = garbage_byte;
				if(garbage_byte_normal < 0) {garbage_byte_normal += 256;}
				
				distribution[b][garbage_byte_normal]++;
				in_stream.get(garbage_byte);
			}
			in_stream.close();
		}
		
		//..........Append-writes 250kB portion of file.
		if(looped_at_least_once == false) {out_stream.open("MERGED"          );}
		else                              {out_stream.open("MERGED", ios::app);}
		
		for(int a = 0; a < 250000; a++)
		{	long long final_byte = 0;
			
			if(average_instead_of_occurrence == true)
			{	long long number_of_items = 0;
				for(int b = 0; b < 256; b++)
				{	if(distribution[a][b] != 0)
					{	number_of_items += distribution[a][b];
						final_byte += ((b + 1) * distribution[a][b]);
					}
				}
				
				if(number_of_items == 0) {out_stream.close(); remove("f"); cout << "\nAveraging done!\n\n\n"; return 0;}
				
				final_byte /= number_of_items;
				final_byte--;
			}
			else
			{	long long byte_differences_discovered = 0;
				long long most_tallies = 0;
				
				for(int b = 0; b < 256; b++)
				{	if(distribution[a][b] !=            0) {byte_differences_discovered++                    ;}
					if(distribution[a][b] >  most_tallies) {most_tallies = distribution[a][b]; final_byte = b;}
				}
				
				if(byte_differences_discovered > 1) {byte_differences_announced += (byte_differences_discovered - 1);}
				
				if(most_tallies == 0)
				{	if     (byte_differences_announced == 0) {cout << "\nDone! No Byte differences to reconcile.\n\n\n"                                ;}
					else if(byte_differences_announced == 1) {cout << "\nDone! 1 Byte difference reconciled.\n\n\n"                                    ;}
					else                                     {cout << "\nDone! " << byte_differences_announced << " Byte differences reconciled.\n\n\n";}
					
					out_stream.close(); remove("f"); return 0;
				}
			}
			
			if(final_byte < 128) {out_stream.put(final_byte      );}
			else                 {out_stream.put(final_byte - 256);}
		}
		out_stream.close();
		
		bytes_to_skip += 250000;
		looped_at_least_once = true;
		
		megabytes_processed_announcement++;
		if(megabytes_processed_announcement == 4)
		{	megabytes_processed_announcement = 0;
			megabytes_processed_counter++;
			cout << megabytes_processed_counter << "MB merged...\n";
		}
	}
}
