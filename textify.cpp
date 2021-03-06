#include <iostream>
#include <string>
#include "CImg.h"
#include <fstream>
#include <istream>
#include <sstream>
#include <vector>
#include <dirent.h>
#define cimg_use_jpeg 1
#define cimg_use_jpg 1
#define cimg_use_png 1
//#define thresh 50 //Universal Threshold of 50.  CHANGE FOR DIFFERENT VALUES.  Still unsure how to define well.
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

	9020 Frames:  Kiss_Me.mp4
	real	22m59.587s
	user	18m15.048s
	sys	3m40.850s
*/

//GLOBALVARIABLES Initiated in init(w, h)
int thresh = 50;
int width;
int height;
unsigned long length = width * height;

int hLim;

int byteWidth;
int byteHeight;
unsigned long byteLength;

bool toTerminal;

//Value returned if an item in a directory is a FILE, used to CHECK
const unsigned char isFile = 0x8;

//Counts the BRIGHTNESS of bytes.  Each one is linked with the equivalent value in binary, storing the NUMBER OF 1'S in each binary representation
//Ranges from 0 to 255 (2^0 -1 to 2^8 -1 AKA 00000000 to 11111111)
const int bitTable[] = {
	0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 
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

//Character Byte information.  Smallest way we can store the values of the pixels of characters.
//Some still need to be fixed.
/*
	Characters that need to be redone:
	M, W,?,f,H,r,t,U,w,5,7,>,]
*/

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

//This brightness is currently unused.  Will be needed to be added later for BRIGHTNESS comparison
const unsigned char chBright[95] = {
	0 ,24,16,51,45,53,55,11,34,33,
	24,22,14, 5, 8,33,56,30,37,38,
	37,36,45,30,51,44,16,20,24,16,
	22,24,72,46,50,36,46,37,32,42,
	53,32,37,45,26,55,52,50,38,60,
	49,36,27,50,44,55,44,39,33,34,
	33,46,24, 8, 4,38,47,31,51,38,
	38,56,45,28,37,46,28,46,37,40,
	45,49,27,33,37,36,35,47,34,43,
	30,34,28,36,12};

std::string grads = "      ......''`^^,::!>><~~+--?||trrxnnucczmmwqqpbhao******######";

const int gradient_length = 64;

const unsigned char *gradient = (unsigned char*)grads.c_str();

CImgList<unsigned char> chPics(95);
//ENDGLOBAL//

//Initializes variables used for MASS USAGE only once.  MUST be called before working on a picture of possibly different size.
void init(int w, int h){
	::width = w;
	::height = h;
	::length = width * height;

	::byteWidth = (width + 1) / 9;
	::byteHeight = ((height/18)<<4);
	if(height % 18 >= 16){ byteHeight+= 16;}
	//else{byteHeight+= height%18;}
	::hLim = byteWidth * 9 - 1;

	::byteLength = (long)byteWidth * (long)byteHeight;}

/*
//Takes an EDGE-DETECTED IMAGE, converts it to an ASCII version and saves it in the filename given as an argument
void textify(unsigned char pix[], const char* filename){
	unsigned char byteImage[byteLength];
	int workingIndex = 0;
	int byteIndex = 0;
	
	//Converts Picture into byte format, ignoring every ninth column and every 17/18th row.  Black = 0, White = 1
	while(workingIndex < length){ 
		byteImage[byteIndex] = byteImage[byteIndex] * 2 + (pix[workingIndex]>>7);
		workingIndex++;
		if((workingIndex - 1) % width + 1>= hLim){
			workingIndex = (workingIndex + 7) / width * width;
			byteIndex++;
			if(workingIndex / width % 18 == 16){
				workingIndex+= 2*width;}}
		else if(workingIndex % width % 9 == 8){
			byteIndex++;
			workingIndex++;}}

	unsigned char bestChars[byteLength/16];

	//Eliminates pixels under certain brightness threshholds and estimates it to a space. 
	for(int i = 0; i < byteLength/16; i++){  
		char brightness = 0;
		int hTrans = i / (byteWidth) * byteWidth * 16 + i % byteWidth;
		for(int part = 0; part < 16; part++){
			brightness += bitTable[byteImage[hTrans + part * byteWidth]];}
		if((int)brightness <= 4){
			bestChars[i] = 0x10;}}

	byteIndex = 0;
	std::vector< std::vector<int> > compPart;
	compPart.resize(95, std::vector<int> (byteLength, 0));

	//Compares every byte portion to an equivalent position in a character byte set, still a set of 16 byte comparisons
	while(byteIndex != byteLength){ 
		if(bestChars[byteIndex / (16 * byteWidth) * byteWidth + byteIndex % byteWidth != 0x10]){
			for(int charNum = 0; charNum < 95; charNum++){
				compPart[charNum][byteIndex] = byteImage[byteIndex] ^ chars[charNum][(byteIndex / byteWidth) & 15];}}
		byteIndex++;}

	byteIndex = 0;

	//Uses the information gathered from the previous two step in order to find the best characters for each slot.
	while(byteIndex != byteWidth){
		int vertPos = 0;
		while(vertPos != byteHeight){
			if(bestChars[vertPos/16 * byteWidth + byteIndex] !=0x10){
				int lowest = 0xff;
				for(int charNum = 0x1; charNum != 0x5f; charNum++){
					int charComp = 0;
					for(int vertOff = 0; vertOff != 16; vertOff++){
						charComp += bitTable[compPart[charNum][(vertPos + vertOff) * byteWidth + byteIndex]];}
					if(charComp < lowest){
						lowest = charComp;
						bestChars[vertPos / 16 * byteWidth + byteIndex] = (char)(charNum + 0x20);}}}
			else{
				bestChars[vertPos/16 * byteWidth + byteIndex] = 0x20;}
			vertPos+=16;}
		byteIndex++;}

	//Takes the values and prints the equivalent version to a .txt file.
	std::ofstream outfile;
	outfile.open(filename);
	for(int i = 0; i < byteLength / 16; i++){
		if(i % byteWidth == 0){
			outfile << endl;}
		outfile << bestChars[i];}
	outfile.close();
	compPart.clear();}


//Takes an EDGE-DETECTED IMAGE, converts it to an ASCII version and saves it in the filename given as an argument
void textify2(unsigned char pix[], const char* filename){
	unsigned char byteImage[byteLength];
	int workingIndex = 0;
	int byteIndex = 0;
	
	//Converts Picture into byte format, ignoring every ninth column and every 17/18th row.  Black = 0, White = 1
	while(workingIndex < length){ 
		byteImage[byteIndex] = byteImage[byteIndex] * 2 + (pix[workingIndex]>>7);
		workingIndex++;
		if((workingIndex - 1) % width + 1>= hLim){
			workingIndex = (workingIndex + 7) / width * width;
			byteIndex++;
			if(workingIndex / width % 18 == 16){
				workingIndex+= 2*width;}}
		else if(workingIndex % width % 9 == 8){
			byteIndex++;
			workingIndex++;}}

	unsigned char bestChars[byteLength/16];

	//Eliminates pixels under certain brightness threshholds and estimates it to a space. 
	for(int i = 0; i < byteLength/16; i++){  
		char brightness = 0;
		int hTrans = i / (byteWidth) * byteWidth * 16 + i % byteWidth;
		for(int part = 0; part < 16; part++){
			brightness += bitTable[byteImage[hTrans + part * byteWidth]];}
		if((int)brightness <= 4){
			bestChars[i] = 0x10;}}

	byteIndex = 0;
	std::vector< std::vector<int> > compPart;
	compPart.resize(95, std::vector<int> (byteLength, 0));

	//Compares every byte portion to an equivalent position in a character byte set, still a set of 16 byte comparisons
	while(byteIndex != byteLength){ 
		if(bestChars[byteIndex / (16 * byteWidth) * byteWidth + byteIndex % byteWidth != 0x10]){
			for(int charNum = 0; charNum < 95; charNum++){
				compPart[charNum][byteIndex] = byteImage[byteIndex] ^ chars[charNum][(byteIndex / byteWidth) & 15];}}
		byteIndex++;}

	byteIndex = 0;

	//Uses the information gathered from the previous two step in order to find the best characters for each slot.
	while(byteIndex != byteWidth){
		int vertPos = 0;
		while(vertPos != byteHeight){
			if(bestChars[vertPos/16 * byteWidth + byteIndex] !=0x10){
				int lowest = 0xff;
				for(int charNum = 0x1; charNum != 0x5f; charNum++){
					int charComp = 0;
					for(int vertOff = 0; vertOff != 16; vertOff++){
						charComp += bitTable[compPart[charNum][(vertPos + vertOff) * byteWidth + byteIndex]];}
					if(charComp < lowest){
						lowest = charComp;
						bestChars[vertPos / 16 * byteWidth + byteIndex] = (char)(charNum + 0x20);}}}
			else{
				bestChars[vertPos/16 * byteWidth + byteIndex] = 0x20;}
			vertPos+=16;}
		byteIndex++;}

	CImg<unsigned char> endPic(width, height, 1, 1, 0);
	unsigned char white[] = {255, 255, 255};
	unsigned char black[] = {0, 0, 0};
	for(int i = 0; i < byteLength / 16; i++){
		int xVal = (i % byteWidth) * 9;
		int yVal = i / byteWidth * 18;
		endPic.draw_image(xVal, yVal, 0, 0, ::chPics[bestChars[i] - 0x20], 1);//, black, 1, 13);
	}
	endPic.save("media/textified.png");
	compPart.clear();}

//Takes an EDGE-DETECTED IMAGE and a similar SMALLER IMAGE to make a hybrid.
void textifySmart(unsigned char *grad, unsigned char *pix, const char* filename){
	unsigned char byteImage[byteLength];
	int workingIndex = 0;
	int byteIndex = 0;
	
	//Converts Picture into byte format, ignoring every ninth column and every 17/18th row.  Black = 0, White = 1
	while(workingIndex < length){ 
		byteImage[byteIndex] = byteImage[byteIndex] * 2 + (pix[workingIndex]/128);
		workingIndex++;
		if((workingIndex - 1) % width + 1>= hLim){
			workingIndex = (workingIndex + 7) / width * width;
			byteIndex++;
			if(workingIndex / width % 18 == 16){
				workingIndex+= 2*width;}}
		else if(workingIndex % width % 9 == 8){
			byteIndex++;
			workingIndex++;}}

	unsigned char bestChars[byteLength/16];
	unsigned char brightnesses[byteLength/16];

	//Eliminates pixels under certain brightness threshholds and estimates it to a space. 
	for(int i = 0; i < byteLength/16; i++){  
		char brightness = 0;
		int hTrans = i / (byteWidth) * byteWidth * 16 + i % byteWidth;
		for(int part = 0; part < 16; part++){
			brightness += bitTable[byteImage[hTrans + part * byteWidth]];}
		if((int)brightness <= 4){
			bestChars[i] = 0x10;}}

	byteIndex = 0;
	std::vector< std::vector<int> > compPart;
	compPart.resize(95, std::vector<int> (byteLength, 0));

	//Compares every byte portion to an equivalent position in a character byte set, still a set of 16 byte comparisons
	while(byteIndex != byteLength){ 
		if(bestChars[byteIndex / (16 * byteWidth) * byteWidth + byteIndex % byteWidth != 0x10]){
			for(int charNum = 0; charNum < 95; charNum++){
				compPart[charNum][byteIndex] = byteImage[byteIndex] ^ chars[charNum][(byteIndex / byteWidth) & 15];}}
		byteIndex++;}

	byteIndex = 0;

	//Uses the information gathered from the previous two step in order to find the best characters for each slot.
	//cout<<"A";
	while(byteIndex != byteWidth)
	{
		//cout<<"B";
		int vertPos = 0;
		while(vertPos != byteHeight)
		{
			//cout<<"C";
			if(bestChars[vertPos/16 * byteWidth + byteIndex] !=0x10)
			{
				int lowest = 0xff;
				for(int charNum = 0x1; charNum != 0x5f; charNum++)
				{
					//cout<<"D";
					int charComp = 0;
					for(int vertOff = 0; vertOff != 16; vertOff++)
					{
						charComp += bitTable[compPart[charNum][(vertPos + vertOff) * byteWidth + byteIndex]];
					}

					//charComp += abs(chBright[charNum] / 4 - brightnesses[vertPos / 16 * byteWidth + byteIndex]);
					//cout<<"D2";

					if(charComp < lowest)
					{
						lowest = charComp;
						bestChars[vertPos / 16 * byteWidth + byteIndex] = (char)(charNum + 0x20);
					}
					//cout<<"D3";
				}
			}
			else
			{
			//cout<<"C2";
				bestChars[vertPos/16 * byteWidth + byteIndex] = 0x20;
			}
			vertPos+=16;
		}
		byteIndex++;
	}

	//cout<<"Got Here"<<endl;

	CImg<unsigned char> endPic(width, height, 1, 1);

	//unsigned char white[] = {255, 255, 255};
	//unsigned char black[] = {0, 0, 0};
	for(int i = 0; i < byteLength / 16; i++)
	{
		int xVal = (i % byteWidth) * 9;
		int yVal = i / byteWidth * 18;
		if(bestChars[i] > 0x20)
		{
			endPic.draw_image(xVal, yVal, 0, 0, ::chPics[bestChars[i] - 0x20], 1);
		}
		else
		{
			//cout<<"4a"<<endl;
			//cout<<i<<endl;
			//cout<<(int)grad[i+1]<<" ";
			//cout<<gradient[grad.at(i)]<<" "<<endl;
			//cout<<"4b"<<endl;
			endPic.draw_image(xVal, yVal, 0, 0, ::chPics[gradient[(int)grad[i]/4] - 0x20], 0.4);
			//cout<<"4c"<<endl;
		}
	}
	endPic.save(filename);
	//cout<<"Almost"<<endl;
	~endPic;
}
*/
void textifySmartBright(unsigned char *grad, unsigned char *pix, const char* filename){
	unsigned char byteImage[byteLength];
	int workingIndex = 0;
	int byteIndex = 0;
	
	//Converts Picture into byte format, ignoring every ninth column and every 17/18th row.  Black = 0, White = 1
	while(workingIndex < length){ 
		byteImage[byteIndex] = byteImage[byteIndex] * 2 + (pix[workingIndex]/128);
		workingIndex++;
		if((workingIndex - 1) % width + 1>= hLim){
			workingIndex = (workingIndex + 7) / width * width;
			byteIndex++;
			if(workingIndex / width % 18 == 16){
				workingIndex+= 2*width;}}
		else if(workingIndex % width % 9 == 8){
			byteIndex++;
			workingIndex++;}}

	unsigned char bestChars[byteLength/16];
	unsigned char brightnesses[byteLength/16];

	//Eliminates pixels under certain brightness threshholds and estimates it to a space. 
	for(int i = 0; i < byteLength/16; i++){  
		char brightness = 0;
		int hTrans = i / (byteWidth) * byteWidth * 16 + i % byteWidth;
		for(int part = 0; part < 16; part++){
			brightness += bitTable[byteImage[hTrans + part * byteWidth]];}
		brightnesses[i] = brightness;
		if((int)brightness <= 4){
			bestChars[i] = 0x10;}}

	byteIndex = 0;
	std::vector< std::vector<int> > compPart;
	compPart.resize(95, std::vector<int> (byteLength, 0));

	//Compares every byte portion to an equivalent position in a character byte set, still a set of 16 byte comparisons
	while(byteIndex != byteLength){ 
		if(bestChars[byteIndex / (16 * byteWidth) * byteWidth + byteIndex % byteWidth != 0x10]){
			for(int charNum = 0; charNum < 95; charNum++){
				compPart[charNum][byteIndex] = byteImage[byteIndex] ^ chars[charNum][(byteIndex / byteWidth) & 15];}}
		byteIndex++;}

	byteIndex = 0;

	//Uses the information gathered from the previous two step in order to find the best characters for each slot.
	//cout<<"A";
	while(byteIndex != byteWidth)
	{
		//cout<<"B";
		int vertPos = 0;
		while(vertPos != byteHeight)
		{
			//cout<<"C";
			if(bestChars[vertPos/16 * byteWidth + byteIndex] !=0x10)
			{
				int lowest = 0xff;
				for(int charNum = 0x1; charNum != 0x5f; charNum++)
				{
					//cout<<"D";
					int charComp = 0;
					for(int vertOff = 0; vertOff != 16; vertOff++)
					{
						charComp += bitTable[compPart[charNum][(vertPos + vertOff) * byteWidth + byteIndex]];
					}

					charComp += abs(chBright[charNum] / 4 - brightnesses[vertPos / 16 * byteWidth + byteIndex])/2;
					//cout<<"D2";

					if(charComp < lowest)
					{
						lowest = charComp;
						bestChars[vertPos / 16 * byteWidth + byteIndex] = (char)(charNum + 0x20);
					}
					//cout<<"D3";
				}
			}
			else
			{
			//cout<<"C2";
				bestChars[vertPos/16 * byteWidth + byteIndex] = 0x20;
			}
			vertPos+=16;
		}
		byteIndex++;
	}

	//cout<<"Got Here"<<endl;

	CImg<unsigned char> endPic(width, height, 1, 1);

	//unsigned char white[] = {255, 255, 255};
	//unsigned char black[] = {0, 0, 0};
	for(int i = 0; i < byteLength / 16; i++)
	{
		int xVal = (i % byteWidth) * 9;
		int yVal = i / byteWidth * 18;
		if(bestChars[i] > 0x20)
		{
			endPic.draw_image(xVal, yVal, 0, 0, ::chPics[bestChars[i] - 0x20], 1);
		}
		else
		{
			//cout<<"4a"<<endl;
			//cout<<i<<endl;
			//cout<<(int)grad[i+1]<<" ";
			//cout<<gradient[grad.at(i)]<<" "<<endl;
			//cout<<"4b"<<endl;
			endPic.draw_image(xVal, yVal, 0, 0, ::chPics[gradient[(int)grad[i]/4] - 0x20], 0.4);
			//cout<<"4c"<<endl;
		}
	}
	endPic.save(filename);
	//cout<<"Almost"<<endl;
	~endPic;
}

//Takes an EDGE-DETECTED IMAGE, converts it to an ASCII version, and prints it out in the terminal.
//Mainly used for TESTING purposes.  REQUIRES AN INPUT before it outputs in order to make sure that the user can SEE THE OUTPUT
void textifyToTerminal(unsigned char pix[]){
	unsigned char byteImage[byteLength];
	int workingIndex = 0;
	int byteIndex = 0;
	
	//Converts Picture into byte format, ignoring every ninth column and every 17/18th row.  Black = 0, White = 1
	while(workingIndex < length){ 
		byteImage[byteIndex] = byteImage[byteIndex] * 2 + (pix[workingIndex]>>7);
		workingIndex++;
		if((workingIndex - 1) % width + 1>= hLim){
			workingIndex = (workingIndex + 7) / width * width;
			byteIndex++;
			if(workingIndex / width % 18 == 16){
				workingIndex+= 2*width;}}
		else if(workingIndex % width % 9 == 8){
			byteIndex++;
			workingIndex++;}}

	unsigned char bestChars[byteLength/16];

	//Eliminates pixels under certain brightness threshholds and estimates it to a space
	for(int i = 0; i < byteLength/16; i++){  
		char brightness = 0;
		int hTrans = i / (byteWidth) * byteWidth * 16 + i % byteWidth;
		for(int part = 0; part < 16; part++){
			brightness += bitTable[byteImage[hTrans + part * byteWidth]];}
		if((int)brightness <= 4){
			bestChars[i] = 0x10;}}

	byteIndex = 0;
	std::vector< std::vector<int> > compPart;
	compPart.resize(95, std::vector<int> (byteLength, 0));

	//Compares every byte portion to an equivalent position in a character byte set, still a set of 16 byte comparisons
	while(byteIndex != byteLength){ 
		if(bestChars[byteIndex / (16 * byteWidth) * byteWidth + byteIndex % byteWidth != 0x10]){
			for(int charNum = 0; charNum < 95; charNum++){
				compPart[charNum][byteIndex] = byteImage[byteIndex] ^ chars[charNum][(byteIndex / byteWidth) & 15];}}
		byteIndex++;}

	byteIndex = 0;

	//Uses the information gathered from the previous two step in order to find the best characters for each slot.
	while(byteIndex != byteWidth){
		int vertPos = 0;
		while(vertPos != byteHeight){
			if(bestChars[vertPos/16 * byteWidth + byteIndex] !=0x10){
				int lowest = 0xff;
				for(int charNum = 0x1; charNum != 0x5f; charNum++){
					int charComp = 0;
					for(int vertOff = 0; vertOff != 16; vertOff++){
						charComp += bitTable[compPart[charNum][(vertPos + vertOff) * byteWidth + byteIndex]];}
					if(charComp < lowest){
						lowest = charComp;
						bestChars[vertPos / 16 * byteWidth + byteIndex] = (char)(charNum + 0x20);}}}
			else{
				bestChars[vertPos/16 * byteWidth + byteIndex] = 0x20;}
			vertPos+=16;}
		byteIndex++;}

	//Takes the values and prints the equivalent version to a .txt file.
	std::string temp;
	std::getline(cin, temp);
	for(int i = 0; i < byteLength / 16; i++){
		if(i % byteWidth == 0){
			cout << endl;}
		cout << bestChars[i];}
	cout<<endl;
	compPart.clear();}

//Takes ALL PNG IMAGES from the IN folder and outputs text files to the OUT if options suggest.
//Testing mode calls textifyToTerminal for DEBUGGING and general TESTING
//REQUIRES Directory in/ and out/ to exist in the same folder as the .exe
void textifyDirectory(int numConvert){
	DIR *pdir = NULL;
	pdir = opendir("./in");
	struct dirent *pent = NULL;
	if(pdir == NULL){
		cout << "\nERROR! pdir could not be initilized correctly";
		exit(3);}
	int counter = 0;
	while(pent = readdir(pdir)){
		if(counter < numConvert){
			counter++;
			continue;
		}
		if(pent == NULL){
			cout << "\nERROR!  pent could not be intialized correctly";
			exit(3);}
		if(pent->d_type == isFile){
			const std::string filename = std::string(pent->d_name);
			const std::string filepath = "in/" + filename;
			//ONLY WORKS WITH .png and .jpg FILES, can be done with others, but needs to be enabled BEFOREHAND
			if(filename.substr(filename.find_last_of(".")) == ".png" || filename.substr(filename.find_last_of(".")) == ".jpg"){
				std::string outpic = "out/" + filename;
				std::string outfile = "out/" + filename.substr(0,filename.find_last_of(".")) + ".png";

				//Creates the Gray Laplacian Image to be used.  Change the threshold value in GLOBAL VARIABLES to modify the threshold.
				CImg<unsigned char> img(filepath.c_str());
				CImg<unsigned char> gray(width, height, 1, 1);
				CImg<unsigned char> scaled(width, height, 1, 1);
				for(int i = 0; i < width; i++){
					for(int j = 0; j < height; j++){
						unsigned char val = 0;
						unsigned char red = img(i, j, 0, 0);
						unsigned char grn = img(i, j, 0, 1);
						unsigned char blu = img(i, j, 0, 2);

						val = round(0.299 * (double)red + 0.587 * (double)grn + 0.114 * (double)blu);
						gray(i, j, 0, 0) = val;
						scaled(i, j, 0, 0) = val;}}
				gray.normalize(0, 255);
				gray.threshold(thresh);
				gray.laplacian();
//				gray.save(("edge" + outfile).c_str());
				scaled.resize(byteWidth, byteHeight/16);
				scaled.normalize(0, 255);

				//cout<<"Starting new File"<<endl;

				if(toTerminal){
					textifyToTerminal(gray.data());
					cout<<endl;}
				else{
//					textifySmart(scaled.data(), gray.data(), outfile.c_str());
					textifySmartBright(scaled.data(), gray.data(), ("bright" + outfile).c_str());}
				counter++;
				cout << outfile.c_str() <<" and "<< ("bright" + outfile).c_str()<< " converted, with "<< counter << " files made."<<endl;
				img.clear();}}}
	closedir(pdir);}

//Currently a basic text menu.  Has a SINGLE IMAGE version along with MODIFY DIRECTORY version
int main(){
	//Initializes the Character pictures used to draw
	for(int i = 0; i < 95; i++){
		std::string name;
		std::stringstream characters;
		characters <<"chars/"<< i << ".png";
		name = characters.str();
		CImg<unsigned char> character(name.c_str());
		::chPics.insert(character, i);
	}
//	std::string input;
//	cout<<"What do you want to do?"<<endl<<"1: Textify an image (Default)"<<endl<<"2: Textify the in/ directory"<<endl;
//	std::getline(cin, input);


	std::string initializer = "in/00000001.png";
	CImg<unsigned char> img(initializer.c_str());
	init(img.width(), img.height());
	cout<<"Threshold"<<endl;
//	std::getline(cin, input);
	cin>>::thresh;
	::toTerminal = false;
	int numConverted;
	cin >> numConverted;
	textifyDirectory(numConverted);
	return 0;}

/*
	//If "2" is the first value in the inputted string, then DIRECTORY mode is activated.
	//Not default option due to huge dependency issues on the user's setup.
	//Will need to add documentation later and a better GUI
	if(input[0] == '2'){
		std::string initializer = "in/00000001.png";
		CImg<unsigned char> img(initializer.c_str());
		init(img.width(), img.height());
		cout<<"Do you want to print to terminal?  [y|N]"<<endl;
		std::getline(cin, input);
		if(input[0] == 'y') {::toTerminal = true;}
		else {::toTerminal = false;}
		textifyDirectory();}

	else{
		std::string picName;
		cout<<"Picture Name: ";
		while(std::getline(cin,picName)){
			CImg<unsigned char> img(picName.c_str());
			init(img.width(), img.height());
			CImg<unsigned char> gray(width, height, 1, 1);
			CImg<unsigned char> scaled(width, height, 1, 1);
			for(int i = 0; i < width; i++){
				for(int j = 0; j < height; j++){
					unsigned char val = 0;
					unsigned char red = img(i, j, 0, 0);
					unsigned char grn = img(i, j, 0, 1);
					unsigned char blu = img(i, j, 0, 2);
					val = round(0.299 * (double)red + 0.587 * (double)grn + 0.114 * (double)blu);
					gray(i, j, 0, 0) = val;
					scaled(i, j, 0, 0) = val;
				}}
			gray.threshold(thresh);
			gray.laplacian();

			gray.save("media/edge.png");

			//scaled.crop(0, 0, 0, width / 9 * 9, height / 18 * 18, 1, 0);
			scaled.resize(byteWidth, byteHeight / 16);
			scaled.normalize(0, 255);
			scaled.save("media/gradient.png");
			cout<<img.width()<< " " << img.height() << " " << byteWidth << " " << byteHeight << endl;

			cout<<byteLength<<" "<<scaled.size()<<endl;

			//cout<<scaled.value_string();

			cout<<"Do you want to print to terminal?  [y|N]"<<endl;
			std::getline(cin, input);
			if(input[0] == 'y'){::toTerminal = true;}
			else{::toTerminal = false;}
			if(toTerminal) {
				textifyToTerminal(gray.data());}
			else {
				textifySmart(scaled.data(), gray.data(), "media/textified.png");
			}
			//else {textify(gray.data(), "media/textified.txt");}
			cout<<"Converted"<<endl;
			cout<<"Picture Name: ";}}*/
