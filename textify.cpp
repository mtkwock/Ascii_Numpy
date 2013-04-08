#include <iostream>
#include <string>
#include <sstream>
#include "CImg.h"
#include <fstream> //Necessary for file input and output
#include <vector>
//using namespace std; //Main one to make cin and cout faster
#define cimg_use_jpeg 1
#define cimg_use_png 1
using namespace cimg_library;
// 
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

int bitTable[] = {	0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 
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

unsigned char chars[][16] = {
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

/*
Characters that need to be redone:
M, W,?,f,H,r,t,U,w,5,7,>,]
*/

//ENDGLOBAL//

void printCroppedArray(int array[], int top, int bot, int left, int right){
	for(int beginRow = top; beginRow < bot; beginRow++){
		for(int beginCol = left; beginCol < right; beginCol++){
			if(array[beginRow * ::width + beginCol]){
				std::cout << "##";
			}
			else{
				std::cout << "  ";
			}
		}
		std::cout << std::endl;
	}
}


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

void textify(unsigned char pix[])
{
	unsigned char byteImage[::byteLength];
	int workingIndex = 0;
	int byteIndex = 0;
	while(workingIndex < length){
		byteImage[byteIndex] = byteImage[byteIndex] * 2 + (pix[workingIndex]>>7);
		workingIndex++;
		if((workingIndex - 1) % width + 1>= hLim){
			workingIndex = (workingIndex + 7) / width * width;
			byteIndex++;
			if(workingIndex / ::width % 18 == 16){
				workingIndex+= 2*::width;
			}
		}
		else if(workingIndex % width % 9 == 8){
			byteIndex++;
			workingIndex++;
		}
	}

	byteIndex = 0;
	std::cout<< "Step 1 Finished" << std::endl;

//	int compPart[94][byteLength];
	std::vector< std::vector<int> > compPart(95, std::vector<int> (byteLength, 0));
//	int** compPart = new int[95][byteLength];
	while(byteIndex != byteLength){
		for(int charNum = 0; charNum < 95; charNum++){
			compPart[charNum][byteIndex] = byteImage[byteIndex] ^ ::chars[charNum][(byteIndex / byteWidth) & 15];
		}
		byteIndex++;
	}
//	delete byteImage;
	std::cout<<"Step 2 Finished" << std::endl;

//	unsigned char *bestChars = new unsigned char[byteLength / 16];
	unsigned char bestChars[byteLength>>4];
	byteIndex = 0;

	while(byteIndex != byteWidth){
//		std::cout << "Width Once" << std::endl;
		int vertPos = 0;
		while(vertPos != byteHeight){
//			std::cout << "Height Once" << std::endl;
			int lowest = 128;
			for(int charNum = 0x0; charNum != 0x5f; charNum++)
			{
				int charComp = 0;
				for(int vertOff = 0; vertOff != 16; vertOff++)
				{
					charComp+= bitTable[compPart[charNum][(vertPos + vertOff) * byteWidth + byteIndex]];
				}
				if(charComp < lowest)
				{
					lowest = charComp;
					bestChars[vertPos / 16 * byteWidth + byteIndex] = (char)(charNum + 0x20);
//					std::cout << charNum + 32<< " ";
				}
			}
			vertPos+=16;
//			std::cout << "Byte Height " <<byteHeight << std::endl;
		}
		byteIndex++;
	}

//	delete compPart;

	std::cout << "Step 3 Finished" << std::endl;

	int count = 0;

	std::ofstream outfile;
	outfile.open("out/result.txt");
	for(int i = 0; i < ::byteLength / 16; i++)
	{
		if(i % byteWidth == 0){
			outfile << "\n";
		}
		outfile << bestChars[i];
		count++;
	}
	outfile.close();

//	return bestChars;
}


int main()
{

//	CImg<unsigned char> img(640,400,1,3); // Define a 640x400 color image with 8 bits per color component.
//	std::string name;
//	std::cin >> name;
	CImg<unsigned char> img("in/result.jpg");
	init(img.width(), img.height());
//	unsigned char *texts = textify(img.data());
	textify(img.data());
//	std::cout << img.value_string(',', 0); //First Value is separator, second is the max string value
//	for(int i = 0; i < length; i++)
//	{
//		std::cout << (int)img.data()[i] << " ";
//	}
/*	std::ofstream outfile;
	outfile.open("out/result.txt");
	for(int i = 0; i < ::length; i++)
	{
		if(i % width == 0){
			outfile << "\n";
		}
		outfile << (char)texts[i];
	}
	outfile.close();*/
//	delete texts;
//	img.fill(0); // Set pixel values to 0 (color : black)
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
