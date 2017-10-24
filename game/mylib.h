//
//Fardad Hajirostami
//

#ifndef _MYLIB_H_
#define _MYLIB_H_

typedef unsigned int u32;
typedef unsigned short u16;

typedef struct 
{
	const volatile void *src;
	volatile void *dst;
	volatile u32 cnt;
} DMAREC;

#define SCANLINECOUNTER *(volatile unsigned short *)0x4000006

#define DMA ((volatile DMAREC *)0x040000B0)

#define REG_DMA0SAD         *(u32*)0x40000B0  
#define REG_DMA0DAD         *(u32*)0x40000B4  
#define REG_DMA0CNT         *(u32*)0x40000B8  

/* DMA channel 1 register definitions */
#define REG_DMA1SAD         *(u32*)0x40000BC 
#define REG_DMA1DAD         *(u32*)0x40000C0  
#define REG_DMA1CNT         *(u32*)0x40000C4  

/* DMA channel 2 register definitions */
#define REG_DMA2SAD         *(u32*)0x40000C8  
#define REG_DMA2DAD         *(u32*)0x40000CC  
#define REG_DMA2CNT         *(u32*)0x40000D0  

/* DMA channel 3 register definitions */
#define REG_DMA3SAD         *(u32*)0x40000D4  
#define REG_DMA3DAD         *(u32*)0x40000D8  
#define REG_DMA3CNT         *(u32*)0x40000DC  

/* Defines */
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

#define START_ON_FIFO_EMPTY 0x30000000



    // a constant definition exported by library:
//    #define MAX_FOO  20
#define REG_DISPCTL *(u16 *)0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)

#define COLOR(r,g,b) ((r) | (g)<<5 | (b)<<10)
#define BLACK   COLOR(0 , 0 , 0)
#define WHITE   COLOR(31,31,31)
#define RED     COLOR(31, 0, 0)
#define MAGENTA COLOR(31, 0 , 31)
#define GREEN   COLOR(0, 0, 31)
#define BLUE    COLOR(0 , 0 , 31)
#define YELLOW  COLOR(31, 31, 0 )
#define MAGENTA COLOR(31, 0 , 31)
#define PURPLE  COLOR(15, 0 , 15)
#define TEAL    COLOR(0 , 15, 15)
#define MAROON  COLOR(15, 0 , 0 )
#define MINT    COLOR(0, 31, 20)
#define CYAN    COLOR(0, 31, 27)
#define ORANGE  COLOR(31, 15, 0 )
#define BROWN   COLOR(18, 9 , 0 )
#define GREY    COLOR(15, 15, 15)
#define PINK    COLOR(31, 18, 19)
#define OFFSET(a,b) ((a)*240+(b))
#define OFFSET2(row, col, rowLen) ((row)*rowLen + (col))


//mylib.c Prototypes!
void waitForVblank();
void fillScreen(u16 color);
void setPixel(int r, int c, unsigned short color); 
void drawRect(int row, int col, int height, int width, unsigned short color);
void drawImage3(int r, int c, int width, int height, const u16* image);
    
#endif