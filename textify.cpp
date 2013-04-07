//GLOBALVARIABLES//
const byte chars[][] = new byte[95][16];

const int width;
const int height;
const int length = width * height;

const byteWidth = (width + 1) / 9;
const byteHeight = ((height + 2)<<3) / 9;
const int byteLength = byteWidth * byteHeight;

const int bitTable[] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8}
//ENDGLOBAL//

int[] textify(int image[])
{
	byte byteImage[] = new byte[byteLength];
	int workingIndex = 0;
	int byte_index = 0;
	const int horLimit = byte_width * 9;

	while(workingIndex <= length){
		if(workingIndex % width >= horLimit){
			workingIndex = (workingIndex + 8) / width * width
			byteIndex++;
			if(workingIndex / width % 18 == 16){
				workingIndex+= 2*width;
			}
		}
		else if(workingIndex % width % 9 == 8){
			byteIndex++;
			workingIndex++;
		}
		byteImage[byteIndex] = byteImage[byteIndex]<<1 + (pix[workingIndex]>>7);
		workingIndex++;
	}

	byteIndex = 0

	int compPart[][] = new int[95][byte_length];

	while(byteIndex != byteLength){
		for(int charNum = 0; charNum < 95; charNum++){
			compPart[charNum][byteIndex] = byteImage[byteIndex] ^ chars[charNum][(byteIndex / byteWidth) & 15];
		}
		byteIndex++;
	}

	int bestChars[] = new int[byteLength>>4];
	byteIndex = 0;

	while(byteIndex != byteWidth){
		int vertPos = 0;
		while(vertPos != byteHeight){
			int lowest = 128;
			for(int charNum = 0; charNum != 95; charNum++){
				int charComp = 0;
				for(int vertOff = 0; vertOff != 16; vertOff++){
					charComp+= bitTable[compPart[(vertPos + vertOff) * byteWidth + byteIndex]];
				}
				if(charComp < lowest){
					lowest = charComp;
					bestChars[vertPos>>4 * byteWidth + byteIndex] = charNum + 32;
				}
			}
			vertPos+=16;
		}
		byteIndex++;
	}
	return bestChars;
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

const byteWidth = (width + 1) / 9  //Accounts for the elimination of every 9th byte (index 8), assuming the last value is left unaccounted for at all times  Rounds down in total number of bytes  (61 pixels wide translates to a width of 6 bytes)

const byteHeight = ((height + 2)<<3) / 9 //The 17th and 18th row of every 18 is being eliminated.  This is the equivalent of multiplying by 16 and dividing by 18.  [indices 16 and 17 are not accounted for]  Rounds down in total number of bytes

const int byteLength = byteWidth * byteHeight;

byte byteImage[] = new byte[byteLength]; //The bytes which represent the image
int workingIndex = 0;
int byte_index = 0; //Which byte it's working on
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
const byte chars[][] = new byte[95][16]

const byte a[] = new byte[16]
a[ 0] = byte = [0][0][0][0][0][0][0][0] = 00000000 =   0
a[ 1] = byte = [0][0][0][0][0][0][0][0] = 00000000 =   0
a[ 2] = byte = [0][0][0][0][0][0][0][0] = 00000000 =   0
a[ 3] = byte = [0][0][0][0][0][0][0][0] = 00000000 =   0
a[ 4] = byte = [0][0][0][0][0][0][0][0] = 00000000 =   0
a[ 5] = byte = [0][0][1][1][1][1][1][0] = 00111110 =  62
a[ 6] = byte = [0][0][0][0][0][1][1][1] = 00000111 =   7
a[ 7] = byte = [0][0][0][0][0][0][1][1] = 00111111 =  63
a[ 8] = byte = [0][0][1][1][1][1][1][1] = 01110111 = 120
a[ 9] = byte = [0][1][1][1][0][1][1][1] = 01100011 = 102
a[10] = byte = [0][1][1][0][0][0][1][1] = 01110111 = 120
a[11] = byte = [0][1][1][1][0][1][1][1] = 00111111 =  63
a[12] = byte = [0][0][1][1][1][1][1][1] = 00000000 =   0
a[13] = byte = [0][0][0][0][0][0][0][0] = 00000000 =   0
a[14] = byte = [0][0][0][0][0][0][0][0] = 00000000 =   0
a[15] = byte = [0][0][0][0][0][0][0][0] = 00000000 =   0


*/