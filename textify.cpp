#include <iostream>
#include <string>
#include "CImg.h"
#include <fstream>
#include <istream>
#include <vector>
#include <dirent.h>
//#define cimg_use_jpeg 1
#define cimg_use_png 1
using namespace cimg_library;
using std::cout;
using std::cin;
using std::endl;
/*
REQUIRED INSTALLS:
sudo apt-get install imagemagick
sudo apt-get intall cimg-dev

HOW TO COMPILE:
g++ -o textify.exe textify.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11

HOW TO USE:
./textify.exe
*/

//GLOBALVARIABLES
//Initiated in init(w, h)
int width;
int height;
unsigned long length = width * height;

int hLim;

int byteWidth;
int byteHeight;
unsigned long byteLength;

const unsigned char isFile = 0x8;

const int bitTable[] = {	0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 
					1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
					1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
					2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
					1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
					2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
					2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
					3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
					1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
					2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
					2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
					3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
					2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
					3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
					3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
					4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8};

const unsigned char chars[][16] = {
{ 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0}, //  (32) 
{ 0x0,  0x0,  0x0, 0x1c, 0x1c, 0x1c, 0x1c, 0x18, 0x18, 0x18,  0x0, 0x1c, 0x1c,  0x0,  0x0,  0x0}, //! (33) 
{ 0x0,  0x0, 0x36, 0x36, 0x36, 0x36,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0}, //" (34) 
{ 0x0,  0x0,  0x0, 0x1b, 0x37, 0xff, 0x36, 0x36, 0x76, 0xff, 0x6e, 0x6c, 0x6c,  0x0,  0x0,  0x0}, //# (35) 
{ 0x0,  0x0, 0x18, 0x3e, 0x72, 0x60, 0x70, 0x7c, 0x1e,  0x7,  0x3, 0x67, 0x7e, 0x18, 0x18,  0x0}, //$ (36) 
{ 0x0,  0x0,  0x0, 0x73, 0xf3, 0xde, 0xfc, 0x7c, 0x1f, 0x3d, 0x3d, 0x6d, 0xc7,  0x0,  0x0,  0x0}, //% (37) 
{ 0x0,  0x0,  0x0, 0x3c, 0x7e, 0x6e, 0x6c, 0x38, 0x7b, 0xff, 0xce, 0xef, 0x7f,  0x0,  0x0,  0x0}, //& (38) 
{ 0x0,  0x0, 0x1c, 0x1c, 0x18, 0x18,  0x8,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0}, //' (39) 
{ 0x0,  0x6,  0xe, 0x1c, 0x38, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x38, 0x18, 0x1c,  0xe,  0x0}, //( (40) 
{ 0x0,  0x0, 0x38, 0x18,  0xc,  0xe,  0x6,  0x6,  0x6,  0x6,  0x6,  0xe,  0xc, 0x1c, 0x38, 0x30}, //) (41) 
{ 0x0,  0x0,  0x0, 0x1c, 0x18, 0x7f, 0x1c, 0x3e, 0x36,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0}, //* (42) 
{ 0x0,  0x0,  0x0,  0x0,  0x0, 0x18, 0x18, 0x18, 0xff, 0x1c, 0x18, 0x18,  0x8,  0x0,  0x0,  0x0}, //+ (43) 
{ 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0, 0x1c, 0x1c,  0xc, 0x1c, 0x38}, //, (44) 
{ 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0, 0x3e,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0}, //- (45) 
{ 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0, 0x18, 0x1c, 0x1c,  0x0,  0x0,  0x0}, //. (46) 
{ 0x0,  0x0,  0x7,  0x6,  0x6,  0xe,  0xc,  0xc, 0x1c, 0x18, 0x18, 0x38, 0x30, 0x30, 0x70, 0x60}, /// (47) 
{ 0x0,  0x0,  0x0, 0x3e, 0x77, 0x63, 0xff, 0xff, 0xe3, 0xe3, 0x63, 0x77, 0x3e,  0x0,  0x0,  0x0}, //0 (48) 
{ 0x0,  0x0,  0x0,  0xc, 0x3c, 0x7c,  0xc,  0xc,  0xc,  0xc,  0xc,  0xc, 0x7f,  0x0,  0x0,  0x0}, //1 (49) 
{ 0x0,  0x0,  0x0, 0x3e, 0x7e,  0x7,  0x7,  0x6, 0x1c, 0x38, 0x30, 0x70, 0x7f,  0x0,  0x0,  0x0}, //2 (50) 
{ 0x0,  0x0,  0x0, 0x7e, 0x6e,  0x7,  0x6, 0x1e,  0x7,  0x3,  0x3, 0x67, 0x7e,  0x0,  0x0,  0x0}, //3 (51) 
{ 0x0,  0x0,  0x0,  0xe, 0x1e, 0x1e, 0x36, 0x66, 0x66, 0xff,  0x6,  0x6,  0x6,  0x0,  0x0,  0x0}, //4 (52) 
{ 0x0,  0x0,  0x0, 0x3f, 0x30, 0x30, 0x30, 0x7e,  0x7,  0x3,  0x3, 0x67, 0x7e,  0x0,  0x0,  0x0}, //5 (53) 
{ 0x0,  0x0,  0x0,  0xf, 0x3c, 0x70, 0x7e, 0x67, 0x63, 0x63, 0x63, 0x77, 0x3e,  0x0,  0x0,  0x0}, //6 (54) 
{ 0x0,  0x0,  0x0, 0x7f,  0x7,  0x6,  0xe,  0xc, 0x1c, 0x18, 0x18, 0x38, 0x38,  0x0,  0x0,  0x0}, //7 (55) 
{ 0x0,  0x0,  0x0, 0x3e, 0x77, 0x63, 0x67, 0x3e, 0x7e, 0x63, 0xe3, 0x77, 0x3e,  0x0,  0x0,  0x0}, //8 (56) 
{ 0x0,  0x0,  0x0, 0x3e, 0x77, 0x63, 0xe3, 0x63, 0x7f,  0x7,  0x6, 0x1e, 0x78,  0x0,  0x0,  0x0}, //9 (57) 
{ 0x0,  0x0,  0x0,  0x0,  0x0, 0x1c, 0x1c, 0x18,  0x0,  0x0, 0x18, 0x1c, 0x1c,  0x0,  0x0,  0x0}, //: (58) 
{ 0x0,  0x0,  0x0,  0x0,  0x0, 0x1c, 0x1c, 0x18,  0x0,  0x0,  0x0, 0x1c, 0x1c, 0x1c, 0x38,  0x0}, //; (59) 
{ 0x0,  0x0,  0x0,  0x0,  0x0,  0x1,  0xf, 0x3c, 0xf0, 0x78, 0x1f,  0x3,  0x0,  0x0,  0x0,  0x0}, //< (60) 
{ 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0, 0xff,  0x0,  0x0, 0xff,  0x0,  0x0,  0x0,  0x0,  0x0}, //= (61) 
{ 0x0,  0x0,  0x0,  0x0,  0x0,  0x0, 0x78, 0x1e,  0x7,  0xf, 0x7c, 0x60,  0x0,  0x0,  0x0,  0x0}, //> (62) 
{ 0x0,  0x0,  0x0, 0x7e, 0x26,  0x7,  0x6,  0xc, 0x18, 0x10,  0x0, 0x38, 0x18,  0x0,  0x0,  0x0}, //? (63) 
{ 0x0,  0x0,  0x0, 0x1e, 0x33, 0x63, 0x6f, 0xff, 0xfb, 0xfb, 0xfb, 0xff, 0x6f, 0x70, 0x38, 0x1f}, //@ (64) 
{ 0x0,  0x0,  0x0, 0x1c, 0x3c, 0x3e, 0x36, 0x76, 0x67, 0x7f, 0xe3, 0xc3, 0xc3,  0x0,  0x0,  0x0}, //A (65) 
{ 0x0,  0x0,  0x0, 0x7e, 0x67, 0x67, 0x66, 0x7e, 0x67, 0x63, 0x63, 0x67, 0x7e,  0x0,  0x0,  0x0}, //B (66) 
{ 0x0,  0x0,  0x0, 0x1f, 0x7b, 0x60, 0x60, 0xe0, 0xe0, 0x60, 0x60, 0x7b, 0x1f,  0x0,  0x0,  0x0}, //C (67) 
{ 0x0,  0x0,  0x0, 0x7c, 0x6e, 0x67, 0x63, 0x63, 0x63, 0x63, 0x67, 0x6e, 0x7c,  0x0,  0x0,  0x0}, //D (68) 
{ 0x0,  0x0,  0x0, 0x7f, 0x70, 0x60, 0x60, 0x7e, 0x70, 0x60, 0x60, 0x70, 0x7f,  0x0,  0x0,  0x0}, //E (69) 
{ 0x0,  0x0,  0x0, 0x7f, 0x70, 0x60, 0x60, 0x7f, 0x70, 0x60, 0x60, 0x60, 0x60,  0x0,  0x0,  0x0}, //F (70) 
{ 0x0,  0x0,  0x0, 0x1f, 0x7b, 0x60, 0x60, 0xe0, 0xe3, 0x63, 0x63, 0x7b, 0x1f,  0x0,  0x0,  0x0}, //G (71) 
{ 0x0,  0x0,  0x0, 0xe3, 0xe3, 0xe3, 0xe3, 0xff, 0xe3, 0xe3, 0xe3, 0xe3, 0xe3,  0x0,  0x0,  0x0}, //H (72) 
{ 0x0,  0x0,  0x0, 0x7f, 0x1c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1c, 0x7f,  0x0,  0x0,  0x0}, //I (73) 
{ 0x0,  0x0,  0x0, 0x3f,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7, 0x6e, 0x7c,  0x0,  0x0,  0x0}, //J (74) 
{ 0x0,  0x0,  0x0, 0x63, 0x66, 0x6e, 0x7c, 0x78, 0x7c, 0x6e, 0x66, 0x67, 0x63,  0x0,  0x0,  0x0}, //K (75) 
{ 0x0,  0x0,  0x0, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x70, 0x7f,  0x0,  0x0,  0x0}, //L (76) 
{ 0x0,  0x0,  0x0, 0x67, 0x77, 0x77, 0x7f, 0xff, 0xdf, 0xc3, 0xc3, 0xc3, 0xc3,  0x0,  0x0,  0x0}, //M (77) 
{ 0x0,  0x0,  0x0, 0x63, 0x73, 0x73, 0x7b, 0x7b, 0x6f, 0x6f, 0x67, 0x67, 0x63,  0x0,  0x0,  0x0}, //N (78) 
{ 0x0,  0x0,  0x0, 0x3e, 0x77, 0xe3, 0xe3, 0xc3, 0xc3, 0xe3, 0xe3, 0x77, 0x3e,  0x0,  0x0,  0x0}, //O (79) 
{ 0x0,  0x0,  0x0, 0x7e, 0x67, 0x63, 0x63, 0x67, 0x7e, 0x60, 0x60, 0x60, 0x60,  0x0,  0x0,  0x0}, //P (80) 
{ 0x0,  0x0,  0x0, 0x3e, 0x77, 0x63, 0xe3, 0xc3, 0xc3, 0xc3, 0xe3, 0x63, 0x7f, 0x3e, 0x1c,  0xf}, //Q (81) 
{ 0x0,  0x0,  0x0, 0x7e, 0x6f, 0x67, 0x63, 0x67, 0x7e, 0x6c, 0x66, 0x67, 0x63,  0x0,  0x0,  0x0}, //R (82) 
{ 0x0,  0x0,  0x0, 0x3e, 0x72, 0x60, 0x70, 0x3c,  0xe,  0x3,  0x3, 0x67, 0x7e,  0x0,  0x0,  0x0}, //S (83) 
{ 0x0,  0x0,  0x0, 0xff, 0x1c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,  0x0,  0x0,  0x0}, //T (84) 
{ 0x0,  0x0,  0x0, 0xe3, 0xe3, 0xe3, 0xe3, 0xe3, 0xe3, 0xe3, 0x63, 0x77, 0x3e,  0x0,  0x0,  0x0}, //U (85) 
{ 0x0,  0x0,  0x0, 0xc3, 0xe3, 0xe3, 0x63, 0x67, 0x76, 0x36, 0x3e, 0x3c, 0x1c,  0x0,  0x0,  0x0}, //V (86) 
{ 0x0,  0x0,  0x0, 0xc3, 0xc3, 0xc3, 0xc3, 0xdf, 0xff, 0x7f, 0x77, 0x77, 0x67,  0x0,  0x0,  0x0}, //W (87) 
{ 0x0,  0x0,  0x0, 0xe3, 0x67, 0x36, 0x3c, 0x1c, 0x3c, 0x3e, 0x76, 0x63, 0xe3,  0x0,  0x0,  0x0}, //X (88) 
{ 0x0,  0x0,  0x0, 0xc3, 0xe3, 0x67, 0x76, 0x3e, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c,  0x0,  0x0,  0x0}, //Y (89) 
{ 0x0,  0x0,  0x0, 0x7f,  0x7,  0x6,  0xc, 0x1c, 0x18, 0x30, 0x70, 0x60, 0x7f,  0x0,  0x0,  0x0}, //Z (90) 
{ 0x0,  0x0, 0x3e, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3e}, //[ (91) 
{ 0x0,  0x0, 0x60, 0x70, 0x30, 0x30, 0x38, 0x18, 0x18, 0x1c,  0xc,  0xc,  0xe,  0x6,  0x6,  0x7}, //\ (92) 
{ 0x0,  0x0, 0x3e,  0xe,  0xe,  0xe,  0xe,  0xe,  0xe,  0xe,  0xe,  0xe,  0xe,  0xe,  0xe, 0x3e}, //] (93) 
{ 0x0,  0x0,  0x0, 0x1c, 0x3c, 0x3e, 0x76, 0x63, 0x43,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0}, //^ (94) 
{ 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0, 0xff}, //_ (95) 
{ 0x0,  0x0, 0x18,  0xc,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0}, //` (96) 
{ 0x0,  0x0,  0x0,  0x0,  0x0, 0x3e,  0x7,  0x3, 0x3f, 0x77, 0x63, 0x77, 0x3f,  0x0,  0x0,  0x0}, //a (97) 
{ 0x0, 0x60, 0x60, 0x60, 0x60, 0x7e, 0x77, 0x63, 0x63, 0x63, 0x63, 0x67, 0x7e,  0x0,  0x0,  0x0}, //b (98) 
{ 0x0,  0x0,  0x0,  0x0,  0x0, 0x3f, 0x78, 0x60, 0xe0, 0xe0, 0x60, 0x79, 0x3f,  0x0,  0x0,  0x0}, //c (99) 
{ 0x0,  0x3,  0x3,  0x3,  0x3, 0x3f, 0x77, 0xe3, 0xe3, 0xe3, 0x63, 0x77, 0x3f,  0x0,  0x0,  0x0}, //d (100) 
{ 0x0,  0x0,  0x0,  0x0,  0x0, 0x3e, 0x77, 0x63, 0xff, 0xe0, 0x60, 0x72, 0x3f,  0x0,  0x0,  0x0}, //e (101) 
{ 0x0,  0x0, 0x1f, 0x18, 0x38, 0x7f, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38,  0x0,  0x0,  0x0}, //f (102) 
{ 0x0,  0x0,  0x0,  0x0,  0x0, 0x3f, 0x77, 0x63, 0xe3, 0xe3, 0xe3, 0x77, 0x3f,  0x7, 0x66, 0x7e}, //g (103) 
{ 0x0, 0x60, 0x60, 0x60, 0x60, 0x7e, 0x77, 0x67, 0x63, 0x63, 0x63, 0x63, 0x63,  0x0,  0x0,  0x0}, //h (104) 
{ 0x0,  0x0, 0x38, 0x38,  0x0, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1d,  0xf,  0x0,  0x0,  0x0}, //i (105) 
{ 0x0,  0x0,  0xe,  0xe,  0x0, 0x3e,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6, 0x6e, 0x7c}, //j (106) 
{ 0x0, 0x60, 0x60, 0x60, 0x60, 0x67, 0x6e, 0x7c, 0x78, 0x7c, 0x6e, 0x67, 0x63,  0x0,  0x0,  0x0}, //k (107) 
{ 0x0,  0x0, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1d,  0xf,  0x0,  0x0,  0x0}, //l (108) 
{ 0x0,  0x0,  0x0,  0x0,  0x0, 0xff, 0xff, 0xdb, 0xdb, 0xdb, 0xc3, 0xc3, 0xc3,  0x0,  0x0,  0x0}, //m (109) 
{ 0x0,  0x0,  0x0,  0x0,  0x0, 0x7e, 0x77, 0x67, 0x63, 0x63, 0x63, 0x63, 0x63,  0x0,  0x0,  0x0}, //n (110) 
{ 0x0,  0x0,  0x0,  0x0,  0x0, 0x3e, 0x77, 0x63, 0xe3, 0xe3, 0x63, 0x77, 0x3e,  0x0,  0x0,  0x0}, //o (111) 
{ 0x0,  0x0,  0x0,  0x0,  0x0, 0x7e, 0x67, 0x63, 0x63, 0x63, 0x63, 0x77, 0x7e, 0x60, 0x60, 0x60}, //p (112) 
{ 0x0,  0x0,  0x0,  0x0,  0x0, 0x3f, 0x77, 0x63, 0xe3, 0xe3, 0xe3, 0x77, 0x3f,  0x3,  0x3,  0x3}, //q (113) 
{ 0x0,  0x0,  0x0,  0x0,  0x0, 0x3f, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70,  0x0,  0x0,  0x0}, //r (114) 
{ 0x0,  0x0,  0x0,  0x0,  0x0, 0x3e, 0x72, 0x60, 0x3c, 0x1e,  0x7, 0x67, 0x7e,  0x0,  0x0,  0x0}, //s (115) 
{ 0x0,  0x0,  0x0, 0x38, 0x38, 0x7f, 0x38, 0x38, 0x38, 0x38, 0x38, 0x1d, 0x1f,  0x0,  0x0,  0x0}, //t (116) 
{ 0x0,  0x0,  0x0,  0x0,  0x0, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x77, 0x3f,  0x0,  0x0,  0x0}, //u (117) 
{ 0x0,  0x0,  0x0,  0x0,  0x0, 0xe3, 0x63, 0x67, 0x76, 0x36, 0x3e, 0x3c, 0x1c,  0x0,  0x0,  0x0}, //v (118) 
{ 0x0,  0x0,  0x0,  0x0,  0x0, 0xc1, 0xc3, 0xdf, 0xff, 0xff, 0x77, 0x77, 0x67,  0x0,  0x0,  0x0}, //w (119) 
{ 0x0,  0x0,  0x0,  0x0,  0x0, 0x63, 0x76, 0x3e, 0x1c, 0x1c, 0x36, 0x67, 0xe3,  0x0,  0x0,  0x0}, //x (120) 
{ 0x0,  0x0,  0x0,  0x0,  0x0, 0x63, 0x63, 0x77, 0x36, 0x36, 0x3e, 0x1c, 0x1c, 0x1c, 0x38, 0xf0}, //y (121) 
{ 0x0,  0x0,  0x0,  0x0,  0x0, 0x7f,  0x6,  0xe, 0x1c, 0x38, 0x30, 0x70, 0x7f,  0x0,  0x0,  0x0}, //z (122) 
{ 0x0,  0x0,  0xf, 0x18, 0x18, 0x18, 0x18, 0x18, 0x70, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18,  0xf}, //{ (123) 
{ 0x0,  0x0, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18}, //| (124) 
{ 0x0,  0x0, 0x78, 0x1c,  0xc,  0xc,  0xc,  0xc,  0xf,  0xc,  0xc,  0xc,  0xc,  0xc, 0x1c, 0x78}, //} (125) 
{ 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0, 0x7b, 0xde,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0}}; //~ (126) FIXED

const unsigned char chBright[95] = 
{0,24,16,51,45,53,55,11,34,33,
24,22,14, 5, 8,33,56,30,37,38,
37,36,45,30,51,44,16,20,24,16,
22,24,72,46,50,36,46,37,32,42,
53,32,37,45,26,55,52,50,38,60,
49,36,27,50,44,55,44,39,33,34,
33,46,24, 8, 4,38,47,31,51,38,
38,56,45,28,37,46,28,46,37,40,
45,49,27,33,37,36,35,47,34,43,
30,34,28,36,12};
/*
Characters that need to be redone:
M, W,?,f,H,r,t,U,w,5,7,>,]
*/

//ENDGLOBAL//


void init(int w, int h){
	::width = w;
	::height = h;
	::length = width * height;


	::byteWidth = (width + 1) / 9;
	::byteHeight = ((height/18)<<4);
	::hLim = byteWidth * 9 - 1;
	if(height % 18 >= 16){
		byteHeight+= 16;
	}
	else{
		byteHeight+= height%18;
	}
	::byteLength = (long)byteWidth * (long)byteHeight;
}
/*
void textify(char *filename){
	CImg<unsigned char> img(filename);
	init(img.width(), img.height());
	textify(img.data());
}*/

void textify(unsigned char pix[], const char* filename){
	unsigned char byteImage[::byteLength];
	int workingIndex = 0;
	int byteIndex = 0;

	while(workingIndex < length){
		byteImage[byteIndex] = byteImage[byteIndex] * 2 + (pix[workingIndex]/128);
		workingIndex++;
		if((workingIndex - 1) % width + 1>= hLim){
			workingIndex = (workingIndex + 7) / width * width;
			byteIndex++;
			if(workingIndex / ::width % 18 == 16){
				workingIndex+= 2*::width;}}
		else if(workingIndex % width % 9 == 8){
			byteIndex++;
			workingIndex++;}}


	unsigned char bestChars[byteLength/16];

	for(int i = 0; i < byteLength/16; i++){  //Eliminates pixels under certain threshholds
		char brightness = 0;
		int hTrans = i / byteWidth * byteWidth * 16 + i % byteWidth;
		for(int part = 0; part < 16; part++){
			brightness += bitTable[byteImage[hTrans + part * byteWidth]];}
		if(brightness <= 4){
			bestChars[hTrans] = 0x10;}
		else{
			bestChars[hTrans] = 0x0;
		}
	}

	byteIndex = 0;
//	std::vector< std::vector<int> > compPart(95, std::vector<int> (byteLength, 0));

	std::vector< std::vector<int> > compPart;
	compPart.resize(95, std::vector<int> (byteLength, 0));


	while(byteIndex != byteLength){
		if(bestChars[byteIndex / (16 * byteWidth) * byteWidth + byteIndex % byteWidth != 32]){
			for(int charNum = 0; charNum < 95; charNum++){
				compPart[charNum][byteIndex] = byteImage[byteIndex] ^ chars[charNum][(byteIndex / byteWidth) & 15];}}
		byteIndex++;}

	byteIndex = 0;

	while(byteIndex != byteWidth){
		int vertPos = 0;
		while(vertPos != byteHeight){
			if(bestChars[vertPos/16 * byteWidth + byteIndex] !=0x10){
				int lowest = 0xff;
				for(int charNum = 0x0; charNum != 0x5f; charNum++){
					int charComp = 0;
					for(int vertOff = 0; vertOff != 16; vertOff++){
						charComp += bitTable[compPart[charNum][(vertPos + vertOff) * byteWidth + byteIndex]];}
//					charComp -= chBright[charNum] / 4;
					if(charComp < lowest){
						lowest = charComp;
						bestChars[vertPos / 16 * byteWidth + byteIndex] = (char)(charNum + 0x20);}}
			}
			else{
				bestChars[vertPos/16 * byteWidth + byteIndex] = 0x20;
			}
			vertPos+=16;}
		byteIndex++;}

//	cout<<width<<" "<<height<<" "<<length<<" "<<byteWidth<<" "<<byteHeight<<" "<<byteLength<<endl;

	std::ofstream outfile;
	outfile.open(filename);
	for(int i = 0; i < byteLength / 16; i++){
		if(i % byteWidth == 0){
			outfile << "\n";}
		outfile << bestChars[i];}
	outfile.close();
	compPart.clear();
	}

void textifyDirectory(){
	DIR *pdir = NULL;
	pdir = opendir("./in");
	struct dirent *pent = NULL;
	if(pdir == NULL)
	{
		cout << "\nERROR! pdir could not be initilized correctly";
		exit(3);
	}
	int counter = 0;
	while(pent = readdir(pdir))
	{
//		cout<<"Here"<<endl;
		if(pent == NULL)
		{
			cout << "\nERROR!  pent could not be intialized correctly";
			exit(3);
		}
//		cout<<"Here B"<<endl;
		if(pent->d_type == isFile)
		{
//			cout<<"Here C"<<endl;
			const std::string filename = std::string(pent->d_name);
			const std::string filepath = "in/" + filename;
//			cout << filepath<<endl;
			cout<<filename<<endl;
			if(filename.substr(filename.find_last_of(".")) == ".png")
			{
//				char fpath[filepath.size() + 1];
//				fpath[filepath.size()] = 0;
//				memcpy(fpath,filepath.c_str(), filepath.size());
//				cout<<fpath;
//				CImg<unsigned char> imag = new CImg<unsigned char>(filepath.c_str());
				CImg<unsigned char> img(filepath.c_str());
//				img.assign(width, height, 1, 3);
				img.load(filepath.c_str());
//				cout<<filepath.c_str()<<img.data()[2]<<endl;

				std::string outpic = "out/" + filename;
//				img.save(outpic.c_str());
				std::string outfile = "out/" + filename.substr(0,filename.find_last_of(".")) + ".txt";
//				char opath[outfile.size() + 1];
//				opath[outfile.size()] = 0;
//				memcpy(opath, outfile.c_str(), outfile.size());
				textify(img.data(), outfile.c_str());
				counter++;
				cout << outfile.c_str() << " converted, with "<< counter << " files made."<<endl;
				img.clear();
			}
		}
	}
	closedir(pdir);
}

void printDirectoryFiles(){
	DIR *pdir = NULL;
	pdir = opendir("./in");
	struct dirent *pent = NULL;
	if(pdir == NULL)
	{
		cout << "\nERROR! pdir could not be initilized correctly";
		exit(3);
	}
	while(pent = readdir(pdir))
	{
		if(pent == NULL)
		{
			cout << "\nERROR!  pent could not be intialized correctly";
			exit(3);
		}
		if(pent->d_type == isFile)
		{
			std::string filename = std::string(pent->d_name);
			std::string filepath = "./in/" + filename;

			cout << filepath<< "was finshed" <<endl;
//			if(pent->d_name.substr(pent->d_name.find_last_of(".")) == ".jpg"){
//				cout<<" Is a Picture File" << endl;
//			}
		}
	}
}

int main()
{

//	CImg<unsigned char> img(640,400,1,3); // Define a 640x400 color image with 8 bits per color component.
//	unsigned char *texts = textify(img.data());
//	for(int i = 0; i < 100; i++){
//	std::string name;
//	cout << "File: ";
//	std::getline(cin, name);
//	cout<<"Here!";
	std::string name = "in/00000001.png";
	CImg<unsigned char> img(name.c_str());
	init(img.width(), img.height());
//	textify(img.data(), "out/test4.txt");
//	img.clear();
//	img.load("in/00000264.png");
//	textify(img.data(), "out/test_3.txt");
//	cout<<"./in/00000001.png"<<img.data();
//	cout<<"Here!";
//	~img;
	textifyDirectory();
//	}
//	textifyDirectory();
//	unsigned char purple[] = { 255,0,255 }; // Define a purple color
//	img.draw_text(100,100,"Hello World",purple); // Draw a purple "Hello world" at coordinates (100,100).
//	img.display("My first CImg code"); // Display the image in a display window.
	return 0;
}

/*
Notes:

ASSUMING BIG ENDIAN (Large values of bytes accounted for in the FIRST ROWS)

Decompose picture into 1 dimensional array:

Use Kernel {1, 1, 1, 1, -8, 1, 1, 1, 1} as a Laplacian against the picture's RGB values, proceeding to eliminate any colors that exist at the same time by summing and dividing by 3.

<http://stackoverflow.com/questions/7449596/implement-laplacian-3x3>

> Yields 1 dimensional array of length (width * height) filled with values between 0 and 255.

iterate through the array to make it 0's and 1's:





const int length = width * height;

Decompose picture into a single array of length w * h filled with 1's or 0's representing white or black pixels.

const byteWidth = (width + 1) / 9  //Accounts for the elimination of every 9thunsigned char(index 8), assuming the last value is left unaccounted for at all times  Rounds down in total number of bytes  (61 pixels wide translates to a width of 6 bytes)

const byteHeight = ((height + 2)<<3) / 9 //The 17th and 18th row of every 18 is being eliminated.  This is the equivalent of multiplying by 16 and dividing by 18.  [indices 16 and 17 are not accounted for]  Rounds down in total number of bytes

const int byteLength = byteWidth * byteHeight;

byte byteImage[] = new byte[byteLength]; //The bytes which represent the image
int workingIndex = 0;
int byte_index = 0; //Whichunsigned charit's working on
const horLimit = byte_width * 9;

while(workingIndex <= length){
	if(workingIndex % width >= horLimit){
		workingIndex = (workingIndex + 8) / width * width//Goes to next row
		byteIndex++;
		if(workingIndex / width % 18 == 16){ //Skips two lines <-> it is the end of the character line
			workingIndex+= 2*width;
		}
	}
	else if(workingIndex % width % 9 == 8){
		byteIndex++;
		workingIndex++;
	}
	byteImage[byteIndex]<<1+=(pix[workingIndex]>>7);
	workingIndex++;
} //The byteImage is now fully completed and

characters stored as the following:
constunsigned charchars[][] = new byte[95][16]

constunsigned chara[] = new byte[16]
a[ 0] =unsigned char= [0][0][0][0][0][0][0][0] = 00000000 =   0
a[ 1] =unsigned char= [0][0][0][0][0][0][0][0] = 00000000 =   0
a[ 2] =unsigned char= [0][0][0][0][0][0][0][0] = 00000000 =   0
a[ 3] =unsigned char= [0][0][0][0][0][0][0][0] = 00000000 =   0
a[ 4] =unsigned char= [0][0][0][0][0][0][0][0] = 00000000 =   0
a[ 5] =unsigned char= [0][0][1][1][1][1][1][0] = 00111110 =  62
a[ 6] =unsigned char= [0][0][0][0][0][1][1][1] = 00000111 =   7
a[ 7] =unsigned char= [0][0][0][0][0][0][1][1] = 00111111 =  63
a[ 8] =unsigned char= [0][0][1][1][1][1][1][1] = 01110111 = 120
a[ 9] =unsigned char= [0][1][1][1][0][1][1][1] = 01100011 = 102
a[10] =unsigned char= [0][1][1][0][0][0][1][1] = 01110111 = 120
a[11] =unsigned char= [0][1][1][1][0][1][1][1] = 00111111 =  63
a[12] =unsigned char= [0][0][1][1][1][1][1][1] = 00000000 =   0
a[13] =unsigned char= [0][0][0][0][0][0][0][0] = 00000000 =   0
a[14] =unsigned char= [0][0][0][0][0][0][0][0] = 00000000 =   0
a[15] =unsigned char= [0][0][0][0][0][0][0][0] = 00000000 =   0


Should report:
124 124 124 124
	int test[] = {	0, 0, 0,   0,   0, 0, 0, 0,0, 0, 0, 0,   0,   0, 0, 0, 0,0,
					0, 0, 0,   0,   0, 0, 0, 0,0, 0, 0, 0,   0,   0, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0,   0,   0, 0, 0, 0,0, 0, 0, 0,   0,   0, 0, 0, 0,0,
					0, 0, 0,   0,   0, 0, 0, 0,0, 0, 0, 0,   0,   0, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 255, 255, 0, 0, 0,0, 0, 0, 0, 255, 255, 0, 0, 0,0,
					0, 0, 0, 0, 0, 0,  0, 0, 0,0, 0, 0, 0, 0, 0, 0,  0, 0, 0,0,
					0, 0, 0, 0, 0, 0,  0, 0, 0,0, 0, 0, 0, 0, 0, 0,  0, 0, 0,0
					};
//	textify(test);
*/
