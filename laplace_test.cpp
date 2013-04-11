#include <iostream>
#include <string>
#include "CImg.h"
#include <fstream>
#include <istream>
#include <vector>
#include <dirent.h>
#define cimg_use_jpeg 1
#define cimg_use_png 1
using namespace cimg_library;
using std::cout;
using std::cin;
using std::endl;

/*    

g++ -o laplace.exe laplace_test.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11
*/
int width;
int height;
unsigned long length = width * height;

int hLim;

int byteWidth;
int byteHeight;
unsigned long byteLength;

const unsigned char isFile = 0x8;

void init(int w, int h){
	::width = w;
	::height = h;
	::length = width * height;

	::byteWidth = (width + 1) / 9;
	::byteHeight = ((height/18)<<4);
	::hLim = byteWidth * 9 - 1;
	if(height % 18 >= 16){ byteHeight+= 16;}
	else{byteHeight+= height%18;}

	::byteLength = (long)byteWidth * (long)byteHeight;}

int main()
{
	std::string initializer;
	int thresh;
	cout<<"Picture: ";
	std::getline(cin,initializer);
	cout<<endl<<"Threshold Value: "; //50 for cartoons seems to work.  need to check more
	cin>>thresh;
	CImg<unsigned char> img(initializer.c_str());
	init(img.width(), img.height());
	CImg<unsigned char> gray(width, height, 1, 1);
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			unsigned char val = 0;
			unsigned char red = img(i, j, 0, 0);
			unsigned char grn = img(i, j, 0, 1);
			unsigned char blu = img(i, j, 0, 2);

			val = round(0.299 * (double)red + 0.587 * (double)grn + 0.114 * (double)blu);
			gray(i, j, 0, 0) = val;
		}
	}
	gray.threshold(thresh);
	gray.laplacian();
//	unsigned char purple[] = { 255,0,255 }; // Define a purple color
//	img.draw_text(100,100,"Hello World",purple); // Draw a purple "Hello world" at coordinates (100,100).
	(img,gray).display("Laplacian, Grayscale, and Threshold "); // Display the image in a display window.
//	std::string outname = "laplace_" + initializer;
	gray.save("media/gray.png");
	return 0;
}
