//Fardad Hajirostami

#include "mylib.h"

//#include <debugging.h>

u16* videoBuffer = (u16*)0x6000000;


// set pixel at (row, col)
void setPixel(int r, int c, u16 color) { 
	videoBuffer[OFFSET(r,c)] = color;
}

// Filled Rectangle
void drawRect(int row, int col, int width, int height, u16 color){ 
	for(int r = 0; r < height; r++) {
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET(row+r, col)];
		DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
	}
} 

// Fill Screen using DMA
void fillScreen(volatile u16 color) {
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = 240*160 | DMA_ON | DMA_SOURCE_FIXED;
}

// Draw Image Using DMA
void drawImage3(int r, int c, int width, int height, const u16* image){
	for(int row = 0; row < height; row++){
		DMA[3].src = image + row * width;
		DMA[3].dst = videoBuffer + OFFSET(row+r, c);
		DMA[3].cnt = DMA_ON | width;
	}
}


void waitForVblank() {
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}



































