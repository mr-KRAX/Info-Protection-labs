#include <fstream>
#include <iostream>
#include <cmath>
#include <numeric>
#include <vector>
#include <cstring>
#include <climits>

using namespace std;

struct Key {
  int k1;
  int k2;
};

uint64_t sqr(uint64_t x) {
    return x * x;
}

uint64_t binpow(uint64_t a, uint64_t e, uint64_t mod) {
    return e == 0 ? 1 : (e & 1U ? a * binpow(a, e - 1, mod) % mod : sqr(binpow(a, e / 2, mod)) % mod);
}

vector<uint8_t> processData(const vector<uint8_t> data, Key key) {
  // printf("(%d, %d)\n", key.k1, key.k2);
  // for (auto it = data.begin(); it != data.end(); it++)
  //   printf("%d ", *it);
  // cout << endl << endl << endl; 
  vector<uint8_t> encrypted_data(data.size());
  for (int i = 0; i < data.size(); i++){
    encrypted_data[i] = binpow(data[i], key.k1, key.k2);
  }
  // for (auto it = encrypted_data.begin(); it != encrypted_data.end(); it++)
  //   printf("%d ", *it);
  // cout << endl; 
  return encrypted_data;
}

inline vector<uint8_t> decripteData(const vector<uint8_t> data, Key privateKey) {
  return processData(data, privateKey);
}

inline vector<uint8_t> encripteData(const vector<uint8_t> data, Key publicKey) {
  return processData(data, publicKey);
}

pair<Key, Key> genKeys(int p, int q) {
  int phi = (p - 1) * (q - 1);
  int n = p * q;
  int e;
  for (e = 2; e < n; e++)
    if (gcd(e, phi) == 1)
      break;

  double d = 0.1;
  int k = 0;
  while (floor(d) != d)
    d = (++k * phi + 1.0) / e;

  return {{e, n}, {(int)d, n}};
}

vector<uint8_t> loadBytes(string filename) {
  ifstream fin(filename);

  fin.seekg(0, ios::end);
  size_t len = fin.tellg();
  fin.seekg(0, ios::beg);

  char *bytes = new char[len];
  fin.read(bytes, len);
  fin.close();

  vector<uint8_t> ret(len);
  for (int i = 0; i < len; i++)
    ret[i] = (uint8_t)bytes[i];
  delete[] bytes;

  return ret;
}

void storeBytes(string filename, vector<uint8_t> data) {
  ofstream fout(filename);

  char *buf = new char[data.size()];
  for (int i = 0; i < data.size(); i++)
    buf[i] = (char)data[i];
  fout.write(buf, data.size());

  fout.close();
}

bool checkPrime(int a) {
  if (a == 0 || a == 1)
      return false;

  for (int i = 2; i <= a / 2; i++)
    if (a % i == 0) 
        return false;
  return true;
}

enum OpCode {HELP, GENKEY, ENCRIPT, DECRIPT, WRONGOPCODE};
OpCode getOpCode(const char* arg) {
  if (strcmp(arg, "--help") == 0    || strcmp(arg, "-h") == 0 ) return HELP;
  if (strcmp(arg, "--gen-key") == 0 || strcmp(arg, "-g") == 0 ) return GENKEY;
  if (strcmp(arg, "--encript") == 0 || strcmp(arg, "-e") == 0 ) return ENCRIPT;
  if (strcmp(arg, "--decript") == 0 || strcmp(arg, "-d") == 0 ) return DECRIPT;
  return WRONGOPCODE;
}

void printHelp(const char* pname){
  cout << "Usage: " << pname << " [--help | -h] [--gen-key | -g <p> <q>]  [--encript | -e <filepath> [<outfilepath>]] [--decript | -d <filepath> [<outfilepath>]]" << endl;
}


int main(int argc, char *argv[]) {
  if (argc < 2) {
      printHelp(argv[0]);
      return 1;
  }

  switch (getOpCode(argv[1]))
  {
  case HELP:
    printHelp(argv[0]);
    return 0;
  case GENKEY:{
    if (argc < 4)
      break;
    int p = atoi(argv[2]);
    int q = atoi(argv[3]);
    if (!checkPrime(p) || !checkPrime(q)) {
      cerr << "Error: numbers are not prime or even numbers" << endl;
      return 1;
    }
    auto keys = genKeys(p, q);
    Key publicKey = keys.first;
    Key privateKey = keys.second;
    cout << "Public key: " << publicKey.k1 << " " << publicKey.k2 << endl;
    cout << "Private key: " << privateKey.k1 << " " << privateKey.k2 << endl;
    return 0;
  }
  case ENCRIPT: {

    if (argc < 3)
      break;
    string filenameIn = argv[2];
    string filenameOut = (argc == 4) ? argv[3] : (filenameIn + ".out");
    Key key;
    cout << "> Enter your public key: ";
    cin >> key.k1 >> key.k2;
    auto data = loadBytes(filenameIn);
    auto encriptedData = encripteData(data, key);
    storeBytes(filenameOut, encriptedData);
    cout << "File successfully encripted!\nSee: " << filenameOut << endl;
    return 0;
  }
  case DECRIPT:{
    if (argc < 3)
        break;
    string filenameIn = argv[2];
    string filenameOut = (argc == 4) ? argv[3] : (filenameIn + ".out");      
    cout << "> Enter your private key: ";
    Key key;
    cin >> key.k1 >> key.k2;
    auto data = loadBytes(filenameIn);
    auto decriptedData = decripteData(data, key);
    storeBytes(filenameOut, decriptedData);
    cout << "File successfully decripted!\nSee: " << filenameOut << endl;
    return 0;
  }
  default:
    break;
  }
  printHelp(argv[0]);
  return 1;
}