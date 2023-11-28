/// StatMerge - merge any models or files of any quantity, based on Byte occurrence.
/// Nikolay Valentinovich Repnitskiy - License: WTFPLv2+ (wtfpl.net)


/* Version 2.0.0
#########*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*##########
#####'`                                                                  `'#####
###'                                                                        '###
##                                                                            ##
#,                Takes a folder with any files of equal size.                ,#
#'               Chop massive files equally for faster merging.               '#
##                                                                            ##
###,                                                                        ,###
#####,.                                                                  .,#####
##########*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#######*/

#include <fstream>
#include <iostream>
using namespace std;

int main()
{	//                               user knobs
	
	/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//////////////////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  /////////////////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\    ////////////////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\      ///////////////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\        //////////////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\            ////////////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\              ///////////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\                  /////////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\\\\\                      ///////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\                              ///////////////////////
	\\\\\\\\\\\\\\\\\\                                        ////////////////*/
	
	//                                                                                                                |
	bool file_size_checking = true;   //DEFAULT = TRUE. Disable only for speed!                ~fatal if broken >     |
	//                                                                                                                |
	
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
	
	/* Top-occurring Bytes are final, else the smallest top-occurring Bytes.
	Recommended: at least 3 models.
	Recommended for low-end computers: 100MB models.
	
	Gets distribution of 250kB segments from each file. So it keeps rereading
	files. It would be nice to use longer segments to lessen rereadind but this
	way, low-end devices can run it (eats ~512MB RAM.) ...For speed of course.
	Also, static long long distribution[250000][256]; needed to be so small
	because it's of type long long--supporting 10^18 files of size 10^18.
	...Just because I don't like setting any real limits. */
	
	ifstream in_stream;
	ofstream out_stream;
	
	//Gets path to FOLDER from user.
	cout << "\nHave a FOLDER ready with n models of equal size."
	     << "\nDrag & drop FOLDER into terminal or enter path:\n";
	char  path_to_file[10000];
	for(int a = 0; a < 10000; a++) {path_to_file[a] = '\0';} //Fills path_to_file[] with null.
	cin.getline(path_to_file, 10000);
	if(path_to_file[0] == '\0') {cout << "\nNo path given.\n"; return 0;}
	
	//Fixes path to file if drag & dropped (removes single quotes for ex:)   '/home/nikolay/my documents/'
	if(path_to_file[0] == '\'')
	{	for(int a = 0; a < 10000; a++)
		{	path_to_file[a] = path_to_file[a + 1];
			if(path_to_file[a] == '\'')
			{	path_to_file[a    ] = '\0';
				path_to_file[a + 1] = '\0';
				path_to_file[a + 2] = '\0';
				break;
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
	for(int a = 0; path_to_file[a] != '\0'; a++) {ls[a + 3] = path_to_file[a];} //Appends given path.
	ls[path_to_file_null_bookmark + 3] = ' '; //Appends commands.
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
	
	if(temp_garbage_byte != '\n') {number_of_files++;}
	
	//Checks if all files are of equal size. This entire if() can be removed.
	if((file_size_checking == true) && (number_of_files > 1))
	{	cout << "\nChecking file sizes...\n";
		
		//..........Copies a few things.
		char COPY_path_to_file[10000];
		for(int a = 0; a < 10000; a++) {COPY_path_to_file[a] = path_to_file[a];}
		
		int COPY_path_to_file_null_bookmark = path_to_file_null_bookmark;
		COPY_path_to_file[COPY_path_to_file_null_bookmark] = '/';
		COPY_path_to_file_null_bookmark++;
		
		//..........Runs through all files.
		long long COPY_file_name_bytes_read_bookmark = -1;
		long long size_of_first_file;
		bool ran_once = false;
		for(long long a = 0; a < number_of_files; a++)
		{	//..........Loads COPY_path_to_file[] with file name.
			in_stream.open("f");
			COPY_file_name_bytes_read_bookmark++;
			for(long long b = 0; b < COPY_file_name_bytes_read_bookmark; b++) {in_stream.get(garbage_byte);} //..........Skips name Bytes that have been read.
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
			long long total_bytes = 0;
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
	}
	
	
	
	
	
	//Begins.
	if(file_size_checking == false) {cout << "\n";}
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
			for(long long b = 0; b < file_name_bytes_read_bookmark; b++) {in_stream.get(garbage_byte);} //..........Skips name Bytes that have been read.
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
			
			//..........Loads distribution[][]. 250kB are read from each file, and Byte occurrences are set. Next round is the next unread 250kB from each file.
			int garbage_byte_normal;
			in_stream.open(path_to_file);
			for(long long b = 0; b < bytes_to_skip; b++) {in_stream.get(garbage_byte);}
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
		{	long long byte_differences_discovered = 0;
			long long most_tallies = 0;
			int final_byte;
			for(int b = 0; b < 256; b++)
			{	if(distribution[a][b] !=            0) {byte_differences_discovered++                    ;}
				if(distribution[a][b] >  most_tallies) {most_tallies = distribution[a][b]; final_byte = b;}
			}
			
			if(byte_differences_discovered > 1) {byte_differences_announced += (byte_differences_discovered - 1);}
			
			if(most_tallies == 0)
			{	out_stream.close();
				cout << "Done.\n\n";
				
				if(byte_differences_announced == 1) {cout <<                               "1 Byte difference reconciled.\n";}
				else                                {cout << byte_differences_announced << " Byte differences reconciled.\n";}
				
				remove("f");
				return 0;
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





/*#######*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*##########
#####'`                                                                  `'#####
###'                              TLDR: run it                              '###
##                                                                            ##
#,                                                                            ,#
#'                              apt install g++                               '#
##                           g++ /path/this_file.cpp                          ##
###,                          /path/resulting_file                          ,###
#####,.                                                                  .,#####
##########*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#######*/

/*How to alter this code - Software package repositories for GNU+Linux operating
systems have all the tools you can imagine. Open a terminal and use this command
as root to install Geany and g++ on your computer: apt install geany g++   Geany
is a fast & lightweight text editor and Integrated Development Environment where
you can write and run code. g++ is the GNU compiler for C++  (industry standard)
which creates an executable file out of written code. It also displays errors in
code as you will see in the lower Geany box. Make a new folder somewhere on your
machine. Paste this code into Geany. For clarity in auditing, enable indentation
guides: go to View >> Show Indentation Guides. Save the document as anything.cpp
within the newly-created folder. Use these shortcuts to run the program: F9, F5.
Once  F9  is used, it needs not be used again unless you had modified this code.
You may paste over this code with other  .cpp files, or open a new tab & repeat.
Additionally, executables created by the  F9  command can be drag & dropped into
terminals (lazy directory pasting) so the executable can run.  exe is in folder.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
How to make an executable with g++  -  Save this program as anything.cpp, open a
terminal, and type g++ then space. Drag & drop this saved file into the terminal
and its directory will be  appended to your pending entry. Click on the terminal
and press enter.   a.out now resides in the user directory, you may rename it to
anything.  To run that executable, simply drag and drop it into a terminal, then
click on the terminal and press enter.  Reminder:  executable's effect-directory
is /home/user or where you put the executable. Opening  .cpp  files in Geany and
hitting F9 creates the same executable--movable, renameable, drag & droppable.*/
