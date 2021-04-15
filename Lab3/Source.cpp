#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <windows.h>

using namespace std;

void unhide_byte_in_pixel(RGBQUAD pixel, uint8_t& hide_byte) {
  hide_byte = 0x0;
  hide_byte |= ((pixel.rgbBlue << 6) & (0xC0));
  hide_byte |= ((pixel.rgbGreen << 4) & (0x30));
  hide_byte |= ((pixel.rgbRed << 2) & (0xC));
  hide_byte |= (pixel.rgbReserved & (0x3));
}

void hide_byte_into_pixel(RGBQUAD* pixel, uint8_t hide_byte) {
  pixel->rgbBlue &= (0xFC);
  pixel->rgbBlue |= (hide_byte >> 6) & 0x3;
  pixel->rgbGreen &= (0xFC);
  pixel->rgbGreen |= (hide_byte >> 4) & 0x3;
  pixel->rgbRed &= (0xFC);
  pixel->rgbRed |= (hide_byte >> 2) & 0x3;
  pixel->rgbReserved &= (0xFC);
  pixel->rgbReserved |= (hide_byte) & 0x3;
}

enum OpCode { HELP, ENCRIPT, DECRIPT, WRONGOPCODE };
OpCode getOpCode(const char* arg) {
  if (strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0) return HELP;
  if (strcmp(arg, "--encript") == 0 || strcmp(arg, "-e") == 0) return ENCRIPT;
  if (strcmp(arg, "--decript") == 0 || strcmp(arg, "-d") == 0) return DECRIPT;
  return WRONGOPCODE;
}

void printHelp(const char* pname) {
  cout << "Usage: " << pname << " [--help | -h] [--encript | -e <file-to-encode> <image-file> [<outfilepath>]] [--decript | -d <image-file> [<outfilepath>]]" << endl;
}

int main(int argc, char* argv[]) {
  setlocale(LC_ALL, "ru_RU.UTF-8");
  if (argc < 2) {
    printHelp(argv[0]);
    return 1;
  }
  string in_file_name, out_file_name, img_file_name;
  ifstream img_file, in_file;
  ofstream out_file;
  BITMAPFILEHEADER bmpHeader;
  BITMAPINFOHEADER bmiHeader;
  RGBQUAD rgbtimed;
  bool eof_flag;

  switch (getOpCode(argv[1])) {
  case HELP:
    printHelp(argv[0]);
    return 0;
  case DECRIPT:
    if (argc < 3)
      break;
    img_file_name = argv[2];
    out_file_name = (argc == 4) ? argv[3] : (img_file_name + "_decoded.txt");


    out_file.open(out_file_name, ios::binary);

    img_file.open(img_file_name, ios::binary);
    img_file.read((char*)&bmpHeader, sizeof(BITMAPFILEHEADER));
    img_file.read((char*)&bmiHeader, sizeof(BITMAPINFOHEADER));

    uint8_t hided_byte;
    eof_flag = false;
    for (int i = 0; i < bmiHeader.biHeight; i++) {
      for (int j = 0; j < bmiHeader.biWidth; j++) {
        img_file.read((char*)&rgbtimed, sizeof(RGBQUAD));
        unhide_byte_in_pixel(rgbtimed, hided_byte);
        cout << hided_byte;
        if (hided_byte == (0xFF)) {
          eof_flag = true;
          break;
        }
        out_file.write((char*)&hided_byte, sizeof(uint8_t));
      }
      if (eof_flag == true)
        break;
    }
    out_file.close();
    img_file.close();
    return 0;
  case ENCRIPT:
    in_file_name = argv[2];
    img_file_name = argv[3];
    out_file_name = (argc == 5) ? argv[4] : ("encoded_" + img_file_name);

    out_file.open(out_file_name, ios::binary);
    in_file.open(in_file_name, ios::binary);
    img_file.open(img_file_name, ios::binary);
    img_file.read((char*)&bmpHeader, sizeof(BITMAPFILEHEADER));
    img_file.read((char*)&bmiHeader, sizeof(BITMAPINFOHEADER));

    out_file.write((char*)&bmpHeader, sizeof(BITMAPFILEHEADER));
    out_file.write((char*)&bmiHeader, sizeof(BITMAPINFOHEADER));
    uint8_t byte;
    for (int i = 0; i < bmiHeader.biHeight; i++) {
      for (int j = 0; j < bmiHeader.biWidth; j++) {
        img_file.read((char*)&rgbtimed, sizeof(RGBQUAD));
        if (in_file.eof())
          byte = 0xFF;
        else
          in_file.read((char*)&byte, sizeof(char));
        hide_byte_into_pixel(&rgbtimed, byte);
        out_file.write((char*)&rgbtimed, sizeof(RGBQUAD));
        cout << byte;
      }
    }
    in_file.close();
    out_file.close();
    img_file.close();
    return 0;
  }
  printHelp(argv[0]);
  return 1;
}