#include <iostream>
#include <fstream> 
#include <map>
using namespace std;


/**
 * \brief Получить размер файла в байтах
 *
 * \param filename имя файла
 * \return int размер файла в байтах или -1 если произошла ошибка
 */
int countFileSize(ifstream file) {
	if (!file)
		return -1;

	streampos begin, end;
	begin = file.tellg();
	file.seekg(0, ios::end);
	end = file.tellg();
	return end - begin;
};

int main(int argc, char* argv[]) {
	/*char filename[1024];
	cout << "Input filename: ";
	cin >> filename;*/

	const char* filename = "C:\\Users\\8180969\\source\\repos\\Project1\\doc.doc";
	ifstream file(filename, ios::in | ios::binary);
	if (!file) {
		cout << "Cannot open file.\n";
		return 1;
	}

	streampos begin, end;
	begin = file.tellg();
	file.seekg(0, ios::end);
	end = file.tellg();
	int fsize = end - begin;
	cout << "file size is: " << fsize << " bytes" << endl;

	file.seekg(0, ios::beg);
	map<unsigned int, int> bytesFreq;
	char byte;
	while (file) {
		file.read(&byte, 1);
		if (file.eof())
			break;
		if (file.fail()) {
			cout << "Input error" << endl;
			break;
		}
		unsigned int int_b = byte;
		auto b_it = bytesFreq.find(int_b);
		if (b_it == bytesFreq.end())
			bytesFreq[int_b] = 0;
		bytesFreq[int_b]++;
	}
	cout << "Byte\tCount" << endl;

	for (auto it = bytesFreq.begin(); it != bytesFreq.end(); it++) {
		cout << hex << it->first << dec << "\t" << it->second << endl;
	}

}