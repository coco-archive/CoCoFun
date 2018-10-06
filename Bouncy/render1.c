// cmoc render.c && writecocofile --verbose ../emulators/mame/bouncy.dsk render.bin && coco3 bouncy.dsk render
#ifndef _COCO_BASIC_
#error This program must be compiled for a CoCo Disk Basic environment.
#endif

#define NULL ((void *) 0)

//#define KILL_IRQ 1
//#define LARGE_BLOCKS 1

#define PLAYER_DELAY    1
#define FPS_DELAY       60  //60 cycles in a second

#define DISP_ADDR   0x400
#define DISP_SIZE   (32*16)
#define DISP_WIDTH  32
#define DISP_HEIGHT 16

#define BLOCK_EMPTY             0
#define BLOCK_PELLET            1
#define BLOCK_WALL              2
#define BLOCK_WALL_INVISIBLE    3
#define BLOCK_WALL_DESTRUCTABLE 4
#define BLOCK_WALL_SLOW         7
#define BLOCK_WALL_DEATH        8
#define BLOCK_EXIT_OPEN         5
#define BLOCK_EXIT_CLOSED       6

#define BLOCK_BLACK     0x80
#define BLOCK_GREEN     0x8F
#define BLOCK_YELLOW    0x9F
#define BLOCK_BLUE      0xAF
#define BLOCK_RED       0xBF
#define BLOCK_BUFF      0xCF
#define BLOCK_CYAN      0xDF
#define BLOCK_MAGENTA   0xEF
#define BLOCK_ORANGE    0xFF

//#define NDEBUG  /* disable the asserts */
#include "coco.h"
#include "stdarg.h"

struct BOUNCY_LEVEL
{
    char name[15];
    int width;
    int height;
    word compressedSize;
    word keysRequired;      //if 0, this is a bonus level
    word keysPresent;
    word maxTime;
    word bestScore;         //the best time for a challenge level, the best score for a regular level
    unsigned char *dataColorLookup;
    unsigned char *data;
};
unsigned short playerTimer = 0;
unsigned short frameRate = 0;
byte testData[] =
{
    0x00,0x04,0x03,0x01,0x02,0x01,0x03,0x0C,0x02,0x04,0x03,0x08,0x02,0x09,0x08,0x01,0x02,0x01,0x03,0x05,0x02,0x05,0x03,0x08,0x02,0x10,0x03,0x06,0x00,0x04,0x03,0x01,0x02,0x01,0x00,0x06,0x02,0x02,0x00,0x03,
    0x02,0x01,0x00,0x04,0x02,0x01,0x00,0x06,0x02,0x01,0x00,0x06,0x02,0x01,0x00,0x04,0x02,0x02,0x00,0x08,0x02,0x01,0x00,0x06,0x02,0x01,0x01,0x01,0x02,0x01,0x00,0x04,0x02,0x01,0x01,0x01,0x00,0x07,0x02,0x01,
    0x00,0x0A,0x03,0x01,0x02,0x01,0x00,0x05,0x02,0x01,0x00,0x02,0x02,0x01,0x00,0x02,0x02,0x01,0x00,0x01,0x02,0x01,0x00,0x03,0x02,0x01,0x00,0x04,0x02,0x01,0x00,0x03,0x02,0x01,0x00,0x03,0x02,0x01,0x00,0x06,
    0x02,0x01,0x00,0x04,0x02,0x01,0x00,0x03,0x02,0x01,0x00,0x04,0x02,0x01,0x00,0x02,0x02,0x01,0x00,0x04,0x02,0x01,0x00,0x08,0x02,0x01,0x00,0x0A,0x02,0x03,0x00,0x03,0x02,0x01,0x00,0x04,0x02,0x01,0x00,0x01,
    0x02,0x01,0x00,0x02,0x02,0x01,0x00,0x03,0x02,0x01,0x00,0x02,0x02,0x01,0x00,0x03,0x02,0x01,0x00,0x01,0x02,0x01,0x00,0x01,0x01,0x01,0x02,0x01,0x01,0x01,0x00,0x06,0x02,0x01,0x00,0x04,0x02,0x01,0x00,0x03,
    0x02,0x01,0x00,0x03,0x02,0x01,0x00,0x02,0x02,0x01,0x00,0x01,0x02,0x02,0x00,0x01,0x02,0x01,0x00,0x08,0x02,0x01,0x00,0x0A,0x03,0x01,0x00,0x02,0x02,0x01,0x00,0x01,0x02,0x01,0x00,0x06,0x02,0x01,0x00,0x04,
    0x02,0x01,0x00,0x05,0x02,0x01,0x00,0x02,0x02,0x01,0x00,0x03,0x02,0x05,0x00,0x01,0x02,0x01,0x00,0x04,0x02,0x01,0x00,0x04,0x02,0x01,0x00,0x03,0x02,0x01,0x00,0x02,0x02,0x01,0x00,0x02,0x02,0x01,0x00,0x01,
    0x02,0x02,0x00,0x01,0x02,0x01,0x08,0x01,0x02,0x01,0x08,0x01,0x02,0x01,0x08,0x01,0x02,0x01,0x08,0x01,0x02,0x04,0x00,0x08,0x03,0x01,0x00,0x03,0x02,0x01,0x00,0x03,0x02,0x02,0x00,0x03,0x02,0x01,0x00,0x04,
    0x02,0x01,0x00,0x03,0x02,0x01,0x00,0x02,0x02,0x01,0x00,0x0B,0x02,0x02,0x00,0x03,0x02,0x01,0x00,0x04,0x02,0x01,0x00,0x03,0x02,0x01,0x00,0x01,0x02,0x01,0x00,0x02,0x02,0x01,0x04,0x02,0x02,0x01,0x00,0x01,
    0x02,0x01,0x00,0x09,0x01,0x01,0x02,0x01,0x00,0x08,0x03,0x01,0x00,0x06,0x02,0x01,0x00,0x04,0x02,0x01,0x00,0x01,0x02,0x01,0x00,0x04,0x02,0x01,0x00,0x04,0x02,0x01,0x00,0x0E,0x02,0x01,0x00,0x03,0x01,0x01,
    0x00,0x04,0x02,0x01,0x00,0x03,0x02,0x01,0x00,0x03,0x02,0x01,0x04,0x02,0x02,0x01,0x00,0x01,0x02,0x01,0x00,0x0A,0x02,0x01,0x00,0x08,0x03,0x01,0x00,0x05,0x02,0x01,0x00,0x04,0x02,0x01,0x00,0x03,0x02,0x01,
    0x00,0x04,0x02,0x01,0x00,0x02,0x02,0x01,0x00,0x03,0x02,0x01,0x00,0x0C,0x02,0x01,0x00,0x03,0x02,0x01,0x00,0x04,0x02,0x01,0x00,0x05,0x02,0x01,0x04,0x03,0x02,0x01,0x00,0x01,0x02,0x01,0x00,0x08,0x05,0x01,
    0x00,0x01,0x02,0x01,0x00,0x08,0x03,0x01,0x00,0x04,0x02,0x01,0x00,0x04,0x02,0x01,0x00,0x05,0x02,0x01,0x00,0x04,0x02,0x02,0x00,0x03,0x02,0x01,0x00,0x0E,0x02,0x01,0x00,0x03,0x02,0x01,0x00,0x04,0x02,0x01,
    0x00,0x03,0x02,0x01,0x00,0x04,0x02,0x01,0x00,0x01,0x02,0x01,0x00,0x0A,0x02,0x01,0x00,0x08,0x03,0x01,0x00,0x03,0x02,0x01,0x00,0x04,0x02,0x01,0x00,0x03,0x02,0x01,0x00,0x03,0x02,0x01,0x00,0x03,0x04,0x01,
    0x00,0x03,0x02,0x01,0x00,0x03,0x02,0x01,0x00,0x06,0x02,0x01,0x00,0x05,0x02,0x01,0x00,0x03,0x02,0x01,0x00,0x04,0x02,0x01,0x00,0x01,0x02,0x01,0x00,0x05,0x02,0x01,0x00,0x01,0x02,0x01,0x00,0x0A,0x02,0x01,
    0x00,0x08,0x03,0x01,0x00,0x02,0x02,0x01,0x00,0x04,0x02,0x01,0x00,0x03,0x02,0x01,0x00,0x01,0x02,0x01,0x00,0x03,0x07,0x03,0x00,0x03,0x02,0x01,0x00,0x03,0x02,0x01,0x00,0x01,0x02,0x05,0x00,0x02,0x02,0x01,
    0x00,0x03,0x02,0x01,0x01,0x02,0x00,0x03,0x02,0x01,0x00,0x01,0x02,0x01,0x00,0x02,0x02,0x01,0x00,0x06,0x02,0x01,0x00,0x01,0x02,0x01,0x00,0x01,0x02,0x0A,0x00,0x08,0x02,0x03,0x00,0x04,0x02,0x01,0x00,0x03,
    0x02,0x01,0x00,0x03,0x02,0x01,0x00,0x07,0x02,0x01,0x00,0x03,0x02,0x01,0x00,0x04,0x02,0x01,0x00,0x05,0x02,0x01,0x01,0x01,0x02,0x01,0x00,0x02,0x02,0x02,0x00,0x03,0x02,0x01,0x00,0x0A,0x02,0x01,0x00,0x01,
    0x02,0x01,0x00,0x07,0x01,0x01,0x02,0x01,0x00,0x0A,0x03,0x01,0x02,0x01,0x00,0x08,0x02,0x01,0x00,0x05,0x02,0x01,0x00,0x05,0x02,0x01,0x00,0x03,0x02,0x01,0x00,0x05,0x02,0x01,0x00,0x06,0x02,0x01,0x00,0x05,
    0x02,0x01,0x00,0x0D,0x02,0x01,0x00,0x01,0x02,0x07,0x00,0x02,0x02,0x01,0x00,0x0A,0x03,0x01,0x02,0x01,0x00,0x07,0x02,0x01,0x00,0x07,0x02,0x01,0x00,0x03,0x02,0x01,0x00,0x09,0x01,0x01,0x02,0x01,0x01,0x01,
    0x00,0x04,0x02,0x01,0x00,0x07,0x02,0x01,0x00,0x0C,0x02,0x01,0x00,0x0A,0x02,0x01,0x00,0x0A,0x03,0x01,0x02,0x01,0x03,0x05,0x02,0x02,0x03,0x09,0x08,0x03,0x03,0x04,0x02,0x0A,0x08,0x01,0x02,0x02,0x03,0x09,
    0x02,0x03,0x03,0x08,0x02,0x0B,0x03,0x01,0x02,0x01,0x03,0x06
};
struct BOUNCY_LEVEL level_test = {
    "TEST",    //name
    81,15,    //cols & rows
    852,    //compressed size
    0,12,    //reqired keys, keys present
    0,    //max time
    0,     //best score
    (byte *)NULL,
    testData
};

byte blockColors[] = {
    BLOCK_BLACK,    //1
    BLOCK_GREEN,    //1
    BLOCK_BUFF,     //2
    BLOCK_BLACK,    //3
    BLOCK_MAGENTA,  //4
    BLOCK_YELLOW,   //5
    BLOCK_RED,      //6
    BLOCK_CYAN,     //7
    BLOCK_ORANGE    //8
};

byte level_uncompressed_data[500*15];
byte level_uncompressed_data_color_lookup[500*15];

struct BOUNCY_LEVEL level_uncompressed = {"              ",0,0,0,0,0,0,0,level_uncompressed_data_color_lookup,level_uncompressed_data};
struct BOUNCY_LEVEL* level = &level_uncompressed;
byte doubleBuffer[DISP_SIZE];
char sbuffer[33];

struct BOUNCY_LEVEL *levels[] =
{
    &level_test,
    //    &level_challenge2,
    //    &level_coco3,
    //    &level_bonus1,
    //    &level_upgrade,
    //    &level_city,
    //    &level_pipes,
    //    &level_shaker,
    //    &level_upgrade,
};
byte numLevels = sizeof(levels)/sizeof(levels[0]);
byte currentLevel = 0;
int levelX = 0;
int levelDir = 1;

int playerx = 16;
int playery = 8;
int playerDir = 1;

void vsync()
{
    asm {
    loopvsync:
        lda $ff03
        bpl loopvsync
        lda $ff02
        
        // increment the timer
        //ldx  $0112
        //leax 1,x
        //stx  $0112
    }
}

void flip()
{
#ifdef KILL_IRQ
    vsync();
#endif
    unsigned *ptr = (unsigned *)doubleBuffer;
    unsigned *endPtr = (unsigned *)(doubleBuffer + 512);
    unsigned *dst = (unsigned *)DISP_ADDR;
    while(ptr < endPtr) {
      *dst++ = *ptr++;
      *dst++ = *ptr++;
      *dst++ = *ptr++;
      *dst++ = *ptr++;
      *dst++ = *ptr++;
      *dst++ = *ptr++;
      *dst++ = *ptr++;
      *dst++ = *ptr++;
      *dst++ = *ptr++;
      *dst++ = *ptr++;
      *dst++ = *ptr++;
      *dst++ = *ptr++;
      *dst++ = *ptr++;
      *dst++ = *ptr++;
      *dst++ = *ptr++;
      *dst++ = *ptr++;
    }
}


unsigned char hasTimerElapsed(unsigned short currentTime,unsigned short waitTime)
{
    if((short)(currentTime-waitTime)>=0 )
        return TRUE;
    return FALSE;
}

int pset(word x,word y,byte color)
{
    byte bit[] = {0x88,0x84,0x82,0x81};
    byte mask[] = {0x87,0x8B,0x8D,0x8E};
    byte shiftx = (byte)x%2;
    byte shifty = (byte)y%2;
    byte z = shifty*2+shiftx;
    x >>= 1;
    y >>= 1;
    byte* disp = doubleBuffer+y*32+x;
    
    //      printf("SX=%02d SY=%02d X=%02d Y=%02d DISP %X Z=%d BIT=%d MASK=%X\n",shiftx,shifty,x,y,disp,z,bit[z],mask[z]);
    byte b = *(disp) & mask[z];
    *disp = b | bit[z] | color;
    return b;
}


void speedup()
{
    if(isCoCo3)
    {
        byte* fast = 0xFFD9;
        *fast = 0;
    }
}

void slowdown()
{
    if(isCoCo3)
    {
        byte* slow = 0xFFD8;
        *slow = 0;
    }
}

void printxy(byte x,byte y,char* s)
{
    byte* display = doubleBuffer+(int)y*DISP_WIDTH+(int)x;
    for(int i=0; i<strlen(s); i++)
    {
        char c=*(s+i);
        //        if(COLOR_SET_BLACK == colorset)
        //        {
        //            if(c >= '@' && c <= ']')
        //                c -= 64;
        //        }
        //        else if(COLOR_SET_GREEN == colorset)
        {
            if(c >= ' ' && c <= '?')
                c += 64;
        }
        
        *(display++) = c;
        x++;
        if(x>DISP_WIDTH)
        {
            x = 0;
            y++;
        }
    }
}

void uncompress(byte* dest,byte* src,word srcSize)
{
    for(word i=0; i<srcSize; i+=2)
    {
        byte key = *(src++);
        byte count = *(src++);
        for(byte j=0; j<count; j++)
        {
            //3 blocks uncompressed to make map bigger
            *(dest++) = key;
#ifdef LARGE_BLOCKS
            *(dest++) = key;
            *(dest++) = key;
#endif
        }
    }
}

//Uuncompress the selected level data unto the uncompressed data buffer,
//then copy the rest of the level data to the uncompressed level structure
void selectLevel(byte levelIndex)
{
    //    printf("selecting level %s\n",levels[levelIndex]->name);
    struct BOUNCY_LEVEL *level = levels[levelIndex];
    uncompress(level_uncompressed_data, level->data, level->compressedSize);

    // Pre-lookup color data
    for(int ii=0; ii<level->height; ii++) {
      byte *src = level_uncompressed_data + level->width*ii;
      byte *dst = level_uncompressed_data_color_lookup + level->width*ii;
      for(int jj=0; jj<level->width; jj++) {
        *dst++ = blockColors[*src++];
      }
    }

    strcpy(level_uncompressed.name, level->name);
#ifdef LARGE_BLOCKS
    level_uncompressed.width = level->width*3; //new block size
#else
    level_uncompressed.width = level->width; //new block size
#endif
    level_uncompressed.height = level->height;
    level_uncompressed.keysRequired = level->keysRequired;
    level_uncompressed.keysPresent = level->keysPresent;
    level_uncompressed.maxTime = level->maxTime;
}

void showLevelSection(int offset)
{
    unsigned *pdisp = (unsigned *)doubleBuffer;
    unsigned *plevel = (unsigned *)(level->dataColorLookup + offset);
    unsigned offset2 = level->width - DISP_WIDTH;

    for(byte y=15; y>0; y--)
    {
        *pdisp++ = *plevel++;
        *pdisp++ = *plevel++;
        *pdisp++ = *plevel++;
        *pdisp++ = *plevel++;
        *pdisp++ = *plevel++;
        *pdisp++ = *plevel++;
        *pdisp++ = *plevel++;
        *pdisp++ = *plevel++;
        *pdisp++ = *plevel++;
        *pdisp++ = *plevel++;
        *pdisp++ = *plevel++;
        *pdisp++ = *plevel++;
        *pdisp++ = *plevel++;
        *pdisp++ = *plevel++;
        *pdisp++ = *plevel++;
        *pdisp++ = *plevel++;
        plevel = (unsigned *)((byte *)plevel + offset2);
    }

}


void gameLoop()
{
    int maxLevelX = level->width-32;
    unsigned short now = getTimer();
    unsigned short fpsTimer = now+FPS_DELAY;
    unsigned short fpsCounter = 0;
    
    selectLevel(0);
    setTimer(0);
    playerTimer = PLAYER_DELAY;
    
    for(byte ii=0; ii<240; ii++) 
    {
        showLevelSection(levelX);
        //        pset(playerx,playery,0x40);
        flip();
        now = getTimer();
        
        levelX += levelDir;
#ifdef LARGE_BLOCKS
        if(levelX + levelDir > 211)
#else
            if(levelX + levelDir > 49)
#endif
                levelDir *= -1;
        if(levelX + levelDir < 0)
            levelDir *= -1;
        
        //if(hasTimerElapsed(now,playerTimer))
        //{
        //    playerTimer = getTimer()+PLAYER_DELAY;
        //    playery += playerDir;
        //    if(playery + playerDir > 29 || playery + playerDir < 0)
        //        playerDir *= -1;
        //}
        
        //++fpsCounter;
        //if(hasTimerElapsed(now,fpsTimer))
        //{
        //    fpsTimer = now+FPS_DELAY;
        //    frameRate = fpsCounter; //100/now via integer math instead of floating point
        //    fpsCounter = 0;
        //}
    }
}

void exitGame()
{
    slowdown();
    cls(1);
    printf("FPS: %d\n", (14400)/getTimer());
    printf("THANKS FOR PLAYING!\n\nBOUNCY BALL\n");
    printf("BY LEE PATTERSON\n");
    printf("WWW.8BITCODER.COM\n\n");
    exit(0);
}


int main()
{
    printf("WWW.8BITCODER.COM\n");
    initCoCoSupport();
    memset(doubleBuffer,128,sizeof doubleBuffer);
#ifdef KILL_IRQ
    asm
    {
        orcc    #$50    // mask interrupts
    }
#endif
    speedup();

    gameLoop();
    exitGame();

    if (0)  {
      return 1;
    } else {
      return 0;
    }
}
