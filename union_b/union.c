#define     _BLOCK_SIZE_ 50
#define     _IMAGES_     5
#define     _SEED_       201001

#include <math.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef void Data;

typedef struct
{
    uint8_t     _id; // 1,2,4 or 8
    uint32_t    _NumPix; // argc
    Data*       _data; // argv

}   Record;     // one record of image data

typedef struct
{
    uint32_t    _limit; 
    uint32_t    _imgC; // argc
    Record**    _imgV; // argv

}   ImageData;

int         dumpImage(const char*,Record*);
Record*     getImage();


int main(void)
{
    srand(_SEED_);
    char message[30];
    ImageData   image;
    image._imgC = 0;
    image._limit = _BLOCK_SIZE_;
    image._imgV = (Record**)malloc( image._limit * sizeof(Record*) );

    Record*      sandbox;
    for (int i = 0; i < _IMAGES_; i += 1)
    {
        // gets an image and a Record to hold it
        sprintf(message, "Sample %d", i);
        sandbox = getImage(); // gets a random image
        dumpImage(message, sandbox);


        Record* target = (Record*)malloc(sizeof(Record)); // new one
        target->_id = sandbox->_id;
        target->_NumPix = sandbox->_NumPix;
        uint32_t total = sandbox->_NumPix * sandbox->_id;
        target->_data = (void*)malloc(total);

        memcpy(target->_data, sandbox->_data, total);
        image._imgV[image._imgC] = target;

        // expands the block if there is no space left
        if (image._imgC >= image._limit)
        {   // block is full
            image._limit += _BLOCK_SIZE_;
            char* new_block = realloc(image._imgV, (image._limit * sizeof(char*)));
            printf("Block extended for a total of %d pointers\n", image._limit);
            image._imgV = (Record**)new_block;
        };  // if()

        image._imgC += 1;

    };  // for

    printf("\n\n%d Test images loaded\n", image._imgC);
    printf("now shows %d random images from list\n\n\n", _IMAGES_ / 2 );

    for (int i = 0; i < _IMAGES_ / 2; i += 1)
    {
        int j = rand() % _IMAGES_;
        sprintf(message, "From Array: image %d", j);
        sandbox = getImage(); // gets a random image
        dumpImage(message,image._imgV[j]);
    };  // for()


    // now trims the end of the block
    // allocated: _limit
    // used: argc
    printf("\t%d pointers allocated\n", image._limit);
    printf("\t%d arguments read\n", image._imgC);
    if (image._limit == image._imgC)
        printf("\tNothing to free()\n");
    else
    {
        printf("\t%d pointers to free\n", image._limit - image._imgC);
        char* new_size = realloc(image._imgV, (image._imgC * sizeof(char*)));
        printf("\tBlock size trimmed for a total of %d pointers\n", image._imgC);
        image._imgV = (Record**)new_size;
    };

    printf("\tImage array is ready for use\n");

    return 0;
};  // main()


int         dumpImage(const char* msg, Record* img)
{
    /*
        uint8_t     _id; // len
        uint32_t    _NumPix; // argc
        Data**      _data; // argv
     */
    uint32_t total = img->_NumPix * img->_id;
    uint8_t* p = (void*)img->_data;
    printf("DumpImage(%s): %d %d-pixels images [%d bytes]:\n\n", msg, img->_NumPix, img->_id, total);
    for (uint32_t i = 0; i < img->_NumPix; i += 1)
    {
        printf("%6d: ", i);
        for (int j = 0; j < img->_id; j++)
            printf("%02X ", *p);
        printf("\n");
    };  // for()
    printf("\n");
    return total;
};

//
// Image factory: build random "images"
// id is 1 to 4, NumPix is 1 to 100, 
// data is NumPix Pixels with id bytes
//
/*
    uint8_t     _id; // len
    uint32_t    _NumPix; // argc
    Data**      _data; // argv
 */
Record* getImage()
{

    const uint8_t len[4] = { 1,2,4,8 };
    Record* one = (Record*)malloc(sizeof(Record));
    one->_id = len[rand() % 4];
    one->_NumPix = 1 + rand() % 8;
    // now builds the pixels with random values under 255
    uint32_t total = one->_NumPix * one->_id;
    void* p = (Data**)malloc(total);
    uint8_t value = rand() % 255;
    memset(p, value, total);
    one->_data = (Data**)p;
    return one;
};
