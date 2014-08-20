/*
atdstrings - ATD strings converter.

Copyright (C) 2014 JrMasterModelBuilder

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/*
To compile:

Mac Clang:
clang++ atdstrings.cpp -o atdstrings -arch i386 -arch x86_64

Win MinGW:
g++ atdstrings.cpp -o atdstrings -static
*/

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct FileData
{
	string path;
	uint32_t size;
	uint32_t posi;
	vector<char> data;
	FileData()
	{
		size = 0;
		posi = 0;
	}
	~FileData()
	{
	}
};

struct KeyValue
{
	string key;
	vector<char> val;
	uint32_t valo;
	KeyValue()
	{
		//keyo = 0;
		valo = 0;
	}
	~KeyValue()
	{
	}
};

fstream fs;

bool read_file(FileData *file)
{
	//Open the file with read permissions.
	fs.open(file->path.c_str(), fstream::in|fstream::binary);

	//Check if file opened successfully.
	if(!fs)
	{
		//Return error.
		fs.close();
		return false;
	}

	//Find the size of the file.
	fs.seekg(0, fs.end);
	file->size = fs.tellg();
	fs.seekg(0, fs.beg);

	//Read the file into memory.
	if(file->size)
	{
		//Resize the vector to fit the file data.
		file->data.resize(file->size);
		//Read the file into the vector char data.
		fs.read(file->data.data(), file->size);
	}

	//Close the file.
	fs.close();

	//Return successful.
	return true;
}

bool write_file(FileData *file)
{
	//Open the file with write permissions.
	fs.open(file->path.c_str(), fstream::out|fstream::binary);

	//Check if file opened successfully.
	if(!fs)
	{
		//Return error.
		fs.close();
		return false;
	}

	//Write the file.
	fs.write(file->data.data(), file->size);

	//Close the file.
	fs.close();

	//Return successful.
	return true;
}

bool file_exists(FileData *file) {
	bool exists = false;
	fs.open(file->path.c_str(), fstream::in|fstream::binary);
	exists = !!fs;
	fs.close();
	return exists;
}

uint32_t readuint32(FileData *file)
{
	return (
		( ((unsigned char)file->data[file->posi++])       ) |
		( ((unsigned char)file->data[file->posi++]) << 8  ) |
		( ((unsigned char)file->data[file->posi++]) << 16 ) |
		( ((unsigned char)file->data[file->posi++]) << 24 )
	);
}

vector<char> char_to_hex(char c)
{
	vector<char> v(2);
	switch(c & 0xF0)
	{
		case 0x00: v[0] = '0'; break;
		case 0x10: v[0] = '1'; break;
		case 0x20: v[0] = '2'; break;
		case 0x30: v[0] = '3'; break;
		case 0x40: v[0] = '4'; break;
		case 0x50: v[0] = '5'; break;
		case 0x60: v[0] = '6'; break;
		case 0x70: v[0] = '7'; break;
		case 0x80: v[0] = '8'; break;
		case 0x90: v[0] = '9'; break;
		case 0xA0: v[0] = 'A'; break;
		case 0xB0: v[0] = 'B'; break;
		case 0xC0: v[0] = 'C'; break;
		case 0xD0: v[0] = 'D'; break;
		case 0xE0: v[0] = 'E'; break;
		case 0xF0: v[0] = 'F'; break;
	}
	switch(c & 0x0F)
	{
		case 0x00: v[1] = '0'; break;
		case 0x01: v[1] = '1'; break;
		case 0x02: v[1] = '2'; break;
		case 0x03: v[1] = '3'; break;
		case 0x04: v[1] = '4'; break;
		case 0x05: v[1] = '5'; break;
		case 0x06: v[1] = '6'; break;
		case 0x07: v[1] = '7'; break;
		case 0x08: v[1] = '8'; break;
		case 0x09: v[1] = '9'; break;
		case 0x0A: v[1] = 'A'; break;
		case 0x0B: v[1] = 'B'; break;
		case 0x0C: v[1] = 'C'; break;
		case 0x0D: v[1] = 'D'; break;
		case 0x0E: v[1] = 'E'; break;
		case 0x0F: v[1] = 'F'; break;
	}
	return v;
}

char hex_to_char(char c1, char c2)
{
	switch(c1)
	{
		case 'F': case 'f': c1 = 0xF0; break;
		case 'E': case 'e': c1 = 0xE0; break;
		case 'D': case 'd': c1 = 0xD0; break;
		case 'C': case 'c': c1 = 0xC0; break;
		case 'B': case 'b': c1 = 0xB0; break;
		case 'A': case 'a': c1 = 0xA0; break;
		case '9':           c1 = 0x90; break;
		case '8':           c1 = 0x80; break;
		case '7':           c1 = 0x70; break;
		case '6':           c1 = 0x60; break;
		case '5':           c1 = 0x50; break;
		case '4':           c1 = 0x40; break;
		case '3':           c1 = 0x30; break;
		case '2':           c1 = 0x20; break;
		case '1':           c1 = 0x10; break;
		case '0': default:  c1 = 0x00; break;
	}
	switch(c2)
	{
		case 'F': case 'f': c2 = 0x0F; break;
		case 'E': case 'e': c2 = 0x0E; break;
		case 'D': case 'd': c2 = 0x0D; break;
		case 'C': case 'c': c2 = 0x0C; break;
		case 'B': case 'b': c2 = 0x0B; break;
		case 'A': case 'a': c2 = 0x0A; break;
		case '9':           c2 = 0x09; break;
		case '8':           c2 = 0x08; break;
		case '7':           c2 = 0x07; break;
		case '6':           c2 = 0x06; break;
		case '5':           c2 = 0x05; break;
		case '4':           c2 = 0x04; break;
		case '3':           c2 = 0x03; break;
		case '2':           c2 = 0x02; break;
		case '1':           c2 = 0x01; break;
		case '0': default:  c2 = 0x00; break;
	}
	return c1 | c2;
}

bool txt2bin(FileData *filein, FileData *fileout)
{
	uint32_t i;
	uint32_t j;
	uint32_t l;
	uint32_t charset_size;
	uint32_t charset_last;
	uint32_t kvps_size;
	uint32_t offset;
	uint32_t hex_char_count;
	char c;
	char hex1;
	char hex2;
	char char_hex;
	char char_prev;
	bool do_break = false;
	bool in_key = false;
	bool in_value = false;
	bool is_after_key = false;
	bool is_ctrl_char = false;
	bool charset[65536] = {false};
	vector<KeyValue *> kvps;
	KeyValue * kv;
	
	//Loop over the caracters.
	for(; filein->posi < filein->size;)
	{
		//The character.
		c = filein->data[filein->posi++];
		//If the character is a control character.
		is_ctrl_char = false;
		switch(c)
		{
			case '[':
				//Opening brackets not in values define keys.
				if(!in_value)
				{
					in_key = true;
					is_ctrl_char = true;
					//Create a new key value pair, and add it to the list.
					kv = new KeyValue();
					kvps.push_back(kv);
				}
			break;
			case ']':
				//Closing brackets in open key declarations.
				if(in_key)
				{
					in_key = false;
					is_ctrl_char = true;
					is_after_key = true;
				}
			break;
			case '\r':
				//Check for \r\n line feed.
				if(filein->posi < filein->size && filein->data[filein->posi] == '\n')
				{
					//Read past the character.
					filein->posi++;
					//Assume \r\n is a control character, even if a litteral.
					is_ctrl_char = true;
					//If in open value.
					if(in_value)
					{
						//Double \r\n followed by not \r or EFO ends value, otherwise append the characters.
						if(
							   filein->posi+1 < filein->size
							&& filein->data[filein->posi  ] == '\r'
							&& filein->data[filein->posi+1] == '\n'
							&& (filein->posi+2 >= filein->size || filein->data[filein->posi+2] != '\r')
						)
						{
							filein->posi += 2;
							in_value = false;
						}
						else
						{
							kv->val.push_back('\r');
							kv->val.push_back(0x00);
							kv->val.push_back('\n');
							kv->val.push_back(0x00);
						}
					}
					//If just after a key, enter a value.
					if(is_after_key)
					{
						in_value = true;
						is_after_key = false;
					}
				}
			break;
		}
		//Non-control characters need to be put in list of chars.
		if(!is_ctrl_char)
		{
			if(in_key)
			{
				//Add the caracter to the key.
				kv->key += c;
			}
			else if(in_value)
			{
				//Check if hex escape character.
				if(c == '|')
				{
					//Check that more can be read.
					if(filein->posi < filein->size)
					{
						//Check if double escape (meaning just the character).
						if(filein->data[filein->posi] == '|')
						{
							filein->posi++;
							kv->val.push_back(c);
							kv->val.push_back(0x00);
							charset[(unsigned char)c] = true;
						}
						else
						{
							//Read forward over the hex characters.
							hex_char_count = 0;
							do_break = false;
							while(!do_break && filein->posi < filein->size)
							{
								//Read the next hex character.
								hex1 = filein->data[filein->posi++];
								//Break if end character.
								if(hex1 == '|')
								{
									break;
								}
								//Check if next hex character can be read.
								if(filein->posi < filein->size)
								{
									hex2 = filein->data[filein->posi++];
								}
								else
								{
									//File ended before hex string terninated.
									char_hex = hex_to_char(hex1, 0);
									do_break = true;
								}
								//If end character.
								if(hex2 == '|')
								{
									//Hex string ended with uneven characters.
									char_hex = hex_to_char(hex1, 0);
									do_break = true;
								}
								else
								{
									//Full hex character read.
									char_hex = hex_to_char(hex1, hex2);
								}
								//Append the character and increment counter.
								kv->val.push_back(char_hex);
								++hex_char_count;
								//Check if high or low character.
								if(hex_char_count % 2)
								{
									//Remember this character to add to the charset.
									char_prev = char_hex;
								}
								else
								{
									//Add the character to the charset.
									charset[(((unsigned char)char_prev) << 8) | ((unsigned char)char_hex)] = true;
								}
							}
							//Check if an uneven number of characters were read.
							if(hex_char_count % 2)
							{
								//Balance the binary data.
								kv->val.push_back(0x00);
								//Add the character to the charset.
								charset[(((unsigned char)char_prev) << 8)] = true;
							}
						}
					}
				}
				else
				{
					//Add the character to the value.
					kv->val.push_back(c);
					kv->val.push_back(0x00);
					//Set this character in the charset if it belongs there.
					switch((unsigned char)c)
					{
						case ' ':
						case '\t'://Guessing here.
							//Ignored character.
						break;
						default:
							charset[(unsigned char)c] = true;
						break;
					}
				}
			}
		}
	}
	//Key/value pairs are all created.
	kvps_size = kvps.size();
	
	//Placeholder for size of character set.
	fileout->data.push_back(0x00);
	fileout->data.push_back(0x00);
	fileout->data.push_back(0x00);
	fileout->data.push_back(0x00);
	
	//Version info?
	fileout->data.push_back(0x20);
	fileout->data.push_back(0x00);
	fileout->data.push_back(0x00);
	fileout->data.push_back(0x00);
	
	//Placeholder for last charcter in set.
	fileout->data.push_back(0x00);
	fileout->data.push_back(0x00);
	fileout->data.push_back(0x00);
	fileout->data.push_back(0x00);
	
	//Version info?
	fileout->data.push_back(0x20);
	fileout->data.push_back(0x00);
	fileout->data.push_back(0x00);
	fileout->data.push_back(0x00);
	
	//Loop over the character list, adding the ones that are in the values, and getting a count of how many.
	charset_size = 0;
	for(i = 0; i < 65536; ++i)
	{
		if(charset[i])
		{
			if(i < 256)
			{
				fileout->data.push_back((char)i);
				fileout->data.push_back(0x00);
			}
			else
			{
				fileout->data.push_back((char)((i >> 8) & 0xFF));
				fileout->data.push_back((char)((i     ) & 0xFF));
			}
			fileout->data.push_back(0x00 + (++charset_size));
			fileout->data.push_back(0x00);
			charset_last = i;
		}
	}
	
	//Set the last character.
	if(charset_last < 256)
	{
		fileout->data[8] = (char)charset_last;
	}
	else
	{
		fileout->data[8] = (char)((charset_last >> 8) & 0xFF);
		fileout->data[9] = (char)((charset_last     ) & 0xFF);
	}
	
	//Update the character size.
	charset_size++;
	fileout->data[0] = charset_size;
	fileout->data[1] = charset_size >> 8;
	fileout->data[2] = charset_size >> 16;
	fileout->data[3] = charset_size >> 24;
	
	//Key/value pair count.
	fileout->data.push_back(kvps_size);
	fileout->data.push_back(kvps_size >> 8);
	fileout->data.push_back(kvps_size >> 16);
	fileout->data.push_back(kvps_size >> 24);
	
	//Append the keys to the file and remember their positions.
	for(i = 0; i < kvps_size;)
	{
		kv = kvps[i++];
		//Add the key to the file, null terminated.
		for(j = 0, l = kv->key.size(); j < 12; ++j)
		{
			fileout->data.push_back((j < l && j < 11) ? kv->key[j] : 0x00);
		}
		//Remember where this value offset is.
		kv->valo = fileout->data.size();
		//Placeholder to offset to appended string.
		fileout->data.push_back(0x00);
		fileout->data.push_back(0x00);
		fileout->data.push_back(0x00);
		fileout->data.push_back(0x00);
	}
	
	//Append the values for the keys and set the offsets.
	for(i = 0; i < kvps_size;)
	{
		kv = kvps[i++];
		//Calculate the relative offset and update the placeholder.
		offset = fileout->data.size() - kv->valo;
		fileout->data[kv->valo  ] = offset;
		fileout->data[kv->valo+1] = offset >> 8;
		fileout->data[kv->valo+2] = offset >> 16;
		fileout->data[kv->valo+3] = offset >> 24;
		//Add the value to the file.
		fileout->data.insert(fileout->data.end(), kv->val.begin(), kv->val.end());
		//Null terminate.
		fileout->data.push_back(0x00);
		fileout->data.push_back(0x00);
	}
	
	//Update the file size.
	fileout->size = fileout->data.size();
	
	return true;
}

bool bin2txt(FileData *filein, FileData *fileout)
{
	uint32_t charsetsize;
	uint32_t strcount;
	uint32_t i;
	uint32_t j;
	char c1;
	char c2;
	vector<char> hex;
	string txt;
	string key;
	
	//Sanity check.
	if(filein->size < 4)
	{
		return false;
	}
	
	//Read the character set size.
	charsetsize = readuint32(filein);
	
	cout << "Charset size: " << charsetsize << endl;
	
	//Seek past the character set, unnecessary to decompile.
	filein->posi += (charsetsize * 4) + 8;
	
	//Sanity check.
	if(filein->posi + 4 >= filein->size)
	{
		return false;
	}
	
	//Read the string count.
	strcount = readuint32(filein);
	
	cout << "String entries: " << strcount << endl;
	
	//Loop over the entries.
	for(i = 0; i < strcount; ++i)
	{
		//Sanity check.
		if(filein->posi + 16 >= filein->size)
		{
			return false;
		}
		
		//Read the up to 11 character key.
		key = string(&filein->data[filein->posi], 0, 11);
		
		//Add the key to the list.
		fileout->data.push_back('[');
		fileout->data.insert(fileout->data.end(), key.begin(), key.end());
		fileout->data.push_back(']');
		fileout->data.push_back('\r');
		fileout->data.push_back('\n');
		
		//Move past it.
		filein->posi += 12;
		
		//Read the value offset and make it absolute.
		j = filein->posi + readuint32(filein);
		
		//Loop over the DBCS characters for the string until until null or eof.
		for(; j < filein->size-2;)
		{
			c1 = filein->data[j++];
			c2 = filein->data[j++];
			//Null terminate.
			if(c1 == 0x00)
			{
				break;
			}
			//Check if multi-byte character.
			if(c2)
			{
				//Hex delimiter.
				fileout->data.push_back('|');
				//Append the hex representation of the first character.
				hex = char_to_hex(c1);
				fileout->data.insert(fileout->data.end(), hex.begin(), hex.end());
				//Append the hex representation of the second character.
				hex = char_to_hex(c2);
				fileout->data.insert(fileout->data.end(), hex.begin(), hex.end());
				//Hex delimiter.
				fileout->data.push_back('|');
			}
			//Check if the escape character.
			else if(c1 == '|')
			{
				//Double it for the unescaped character.
				fileout->data.push_back('|');
				fileout->data.push_back('|');
			}
			else
			{
				//Add the character to the data.
				fileout->data.push_back(c1);
			}
		}
		//End the entry.
		fileout->data.push_back('\r');
		fileout->data.push_back('\n');
		fileout->data.push_back('\r');
		fileout->data.push_back('\n');
	}
	
	fileout->size = fileout->data.size();
	
	return true;
}

int main(int argc, char *argv[])
{
	FileData filein,
		fileout
	;
	size_t path_length;
	if(argc > 1)
	{
		filein.path = argv[1];
		if(read_file(&filein))
		{
			//Check if binary or text file.
			if(
				filein.size >= 24//The smallest possible size for the binary format.
				&& filein.data[4] == 0x20
				&& filein.data[5] == 0x00
				&& filein.data[6] == 0x00
				&& filein.data[7] == 0x00
			)
			{
				//Binary file, add .TXT extension.
				fileout.path = filein.path + ".TXT";
				//Check if file exists.
				if(file_exists(&fileout))
				{
					cout << "ERROR: Converted file already exists: " << fileout.path << endl;
					return 1;
				}
				else
				{
					//Convert the binary file into a text file.
					if(bin2txt(&filein, &fileout))
					{
						cout << "Converted size: " << fileout.size << endl;
						cout << "Writing file: " << fileout.path << endl;
						if(write_file(&fileout))
						{
							cout << "SUCCESS: File converson complete." << endl;
						}
						else
						{
							cout << "ERROR: Failed to write file." << endl;
							return 1;
						}
					}
					else
					{
						cout << "ERROR: Failed to parse file." << endl;
						return 1;
					}
				}
			}
			else
			{
				//Text file, remove .TXT extension, or add .BIN extension.
				path_length = filein.path.size();
				if(
					path_length > 4
					&& (filein.path[path_length-4] == '.')
					&& (filein.path[path_length-3] == 'T' || filein.path[path_length-3] == 't')
					&& (filein.path[path_length-2] == 'X' || filein.path[path_length-2] == 'x')
					&& (filein.path[path_length-1] == 'T' || filein.path[path_length-1] == 't')
				)
				{
					fileout.path = filein.path.substr(0, path_length-4);
				}
				else
				{
					fileout.path = filein.path + ".BIN";
				}
				//Check if file exists.
				if(file_exists(&fileout))
				{
					cout << "ERROR: Converted file already exists: " << fileout.path << endl;
					return 1;
				}
				else
				{
					//Convert the text file into a binary file.
					if(txt2bin(&filein, &fileout))
					{
						cout << "Converted size: " << fileout.size << endl;
						cout << "Writing file: " << fileout.path << endl;
						if(write_file(&fileout))
						{
							cout << "SUCCESS: File converson complete." << endl;
						}
						else
						{
							cout << "ERROR: Failed to write file." << endl;
							return 1;
						}
					}
				}
			}
		}
		else
		{
			cout << "ERROR: Failed to read file: " << filein.path << endl;
			return 1;
		}
	}
	else
	{
		cout << "atdstrings - ATD strings converter." << endl << endl;
		cout << "Copyright (C) 2014 JrMasterModelBuilder" << endl << endl;
		cout << "This tool will convert ATD strings files between their binary and text formats." << endl << endl;
		cout << "USAGE: atdstrings FILE_PATH" << endl << endl;
		cout << "This program is free software: you can redistribute it and/or modify" << endl
			<< "it under the terms of the GNU General Public License as published by" << endl
			<< "the Free Software Foundation, either version 3 of the License, or" << endl
			<< "(at your option) any later version." << endl << endl
		;
		cout << "This program is distributed in the hope that it will be useful," << endl
			<< "but WITHOUT ANY WARRANTY; without even the implied warranty of" << endl
			<< "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the" << endl
			<< "GNU General Public License for more details." << endl << endl
		;
		cout << "You should have received a copy of the GNU General Public License" << endl
			<< "along with this program. If not, see <http://www.gnu.org/licenses/>." << endl << endl
		;
		return 1;
	}
	return 0;
}
