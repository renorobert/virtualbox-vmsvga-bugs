#include <stdint.h>
#include <stdbool.h>

typedef long long int64;
typedef unsigned long long uint64;

typedef int int32;
typedef unsigned int uint32;

typedef short int16;
typedef unsigned short uint16;

typedef char int8;
typedef unsigned char uint8;

typedef uint8 Bool;

typedef struct SVGADevice {
	uint32     ioBase;
	uint32    *fifoMem;
	uint8     *fbMem;
	uint32     fifoSize;
	uint32     fbSize;
	uint32     vramSize;

	uint32     deviceVersionId;
	uint32     capabilities;

	uint32     width;
	uint32     height;
	uint32     bpp;
	uint32     pitch;
} SVGADevice;

uint32_t SVGA_ReadReg(uint32_t);
void SVGA_WriteReg(uint32_t,  uint32_t);
void VMwareWaitForFB(void);
void VMwareWriteWordToFIFO(uint32_t);
int conf_svga_device(void);
