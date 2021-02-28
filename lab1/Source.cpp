#include <fstream>
#include <iostream>
#include <map>
#include <string>
using namespace std;

string filename;

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

int task1() {
  // const char *filename = "C:\\Users\\8180969\\source\\repos\\Project1\\doc.doc";
  // Open file
  ifstream file(filename, ios::in | ios::binary);
  if (!file) {
    cout << "Cannot open file.\n";
    return 1;
  }

  // Calculate size
  streampos begin, end;
  begin = file.tellg();
  file.seekg(0, ios::end);
  end = file.tellg();
  int fsize = end - begin;
  cout << "File size is: " << fsize << " bytes" << endl;

  // Calculate bytes frequency
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
  return 0;
}

void printTextTable(string text, int *columns, int columnNum) {
  // | 0 | 1 | . |
  // +---+---+---+

  cout << "|";
  for (int j = 0; j < columnNum; j++)
    cout << " " << columns[j] << " |";
  cout << "\n+";
  for (int j = 0; j < columnNum; j++)
    cout << "---+";
  // | a | b | . |
  for (int i = 0; i < text.length(); i++) {
    if (i % columnNum == 0)
      cout << "\n|";
    cout << " " << text[i] << " |";
  }
  cout << endl;
}

string encriptText(string plaintext, int *key, int keyLength) {
  int *columnOrder = new int[keyLength];
  for (int i = 0; i < keyLength; i++)
    columnOrder[key[i]] = i;

  /*Append string with z so that its length is a multiple of key length*/
  int n = keyLength - plaintext.length() % keyLength;
  for (int i = 0; i < n; i++)
    plaintext += "z";

  int lineNum = plaintext.length() / keyLength;
  string ciphertext = "";
  for (int k = 0; k < keyLength; k++)
    for (int i = 0; i < lineNum; i++)
      ciphertext += plaintext[i * keyLength + columnOrder[k]];
  return ciphertext;
}

string decriptText(string ciphertext, int *key, int keyLength) {
  string decriptedtext;
  int lineNum = ciphertext.length() / keyLength;
  for (int i = 0; i < lineNum; i++)
    for (int k = 0; k < keyLength; k++)
      decriptedtext += ciphertext[key[k] * lineNum + i];
  return decriptedtext;
}

int task2() {
  // Open text file
  ifstream textFile(filename, ios::in);
  if (!textFile) {
    cout << "Cannot open file.\n";
    return 1;
  }

  // Get text from text file
  string plaintext = "";
  getline(textFile, plaintext);

  // Get key file name
  string keyFileName;
  cout << "Key file name: ";
  getline(cin, keyFileName);
  
  // Get key
  int keyLength = 5;
  ifstream keyFile(keyFileName, ios::in);
  int *key = new int[keyLength];
  for (int i = 0; i < keyLength; i++){
    keyFile >> key[i];
  }

  cout << "Plaintext:" << endl;
  cout << plaintext << endl;
  
  cout << "Table view:" << endl;
  printTextTable(plaintext, key, keyLength);

  string ciphertext = encriptText(plaintext, key, keyLength);
  cout << endl
       << "Ciphertext:" << endl;
  cout << ciphertext << endl;

  string decriptedtext = decriptText(ciphertext, key, keyLength);
  cout << "Deciphered text:" << endl;
  cout << decriptedtext << endl;

  return 0;
}

int main(int argc, char *argv[]) {
#ifdef WIN32
  system("chcp 1251");
#endif

  cout << "1. task-1" << endl
       << "2. task-2" << endl;
  int op;
  cin >> op;
  cout << "File name: ";
  cin.ignore();
  getline(cin, filename);
  // cout << filename << endl;

  switch (op) {
  case 1:
    if (task1())
      return 1;
    break;
  case 2:
    if (task2())
      return 1;
    break;
  default:
    break;
  }
  return 0;
}