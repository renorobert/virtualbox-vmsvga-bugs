#include <err.h>
#include <stdlib.h>
#include <pciaccess.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/io.h>

#include "svga.h"
#include "svga_reg.h"

extern SVGADevice gSVGA;

uint32_t
SVGA_ReadReg(uint32_t index)
{
	outl(index, gSVGA.ioBase + SVGA_INDEX_PORT);
	return inl(gSVGA.ioBase + SVGA_VALUE_PORT);
}

void
SVGA_WriteReg(uint32_t index,  uint32_t value)
{
	outl(index, gSVGA.ioBase + SVGA_INDEX_PORT);
	outl(value, gSVGA.ioBase + SVGA_VALUE_PORT);
}

void
VMwareWaitForFB(void)
{
	SVGA_WriteReg(SVGA_REG_CONFIG_DONE, true);
	SVGA_WriteReg(SVGA_REG_SYNC, SVGA_SYNC_GENERIC);
	while (SVGA_ReadReg(SVGA_REG_BUSY));
}

void
VMwareWriteWordToFIFO(uint32_t value)
{
	uint32_t *VMwareFIFO = gSVGA.fifoMem;

	if ((VMwareFIFO[SVGA_FIFO_NEXT_CMD] + sizeof(uint32_t) == VMwareFIFO[SVGA_FIFO_STOP])
			|| (VMwareFIFO[SVGA_FIFO_NEXT_CMD] == VMwareFIFO[SVGA_FIFO_MAX] - sizeof(uint32_t)
				&& VMwareFIFO[SVGA_FIFO_STOP] == VMwareFIFO[SVGA_FIFO_MIN])) {

		VMwareWaitForFB();
	}

	VMwareFIFO[VMwareFIFO[SVGA_FIFO_NEXT_CMD] / sizeof(uint32_t)] = value;

	if(VMwareFIFO[SVGA_FIFO_NEXT_CMD] == VMwareFIFO[SVGA_FIFO_MAX] - sizeof(uint32_t)) {
		VMwareFIFO[SVGA_FIFO_NEXT_CMD] = VMwareFIFO[SVGA_FIFO_MIN];
	} else {
		VMwareFIFO[SVGA_FIFO_NEXT_CMD] += sizeof(uint32_t);
	}
}

void
pci_cleanup(struct pci_device_iterator *iter)
{

	pci_iterator_destroy(iter);
	pci_system_cleanup();
}


int
conf_svga_device(void)
{

	struct pci_device *dev;
	struct pci_device_iterator *iter;
	struct pci_id_match match;
	uint16_t command;

	if (getuid() != 0 || geteuid() != 0) 
		errx(EXIT_FAILURE, "[!] Run program as root");

	iopl(3);

	if (pci_system_init())
		return -1;

	match.vendor_id = PCI_VENDOR_ID_VMWARE;
	match.device_id = PCI_DEVICE_ID_VMWARE_SVGA2;
	match.subvendor_id = PCI_MATCH_ANY;
	match.subdevice_id = PCI_MATCH_ANY;
	match.device_class = 0;
	match.device_class_mask = 0;

	iter = pci_id_match_iterator_create(&match);
	dev = pci_device_next(iter);

	if (dev == NULL) {
		pci_cleanup(iter);
		return -1;
	}

	pci_device_probe(dev);

	gSVGA.ioBase = dev->regions[0].base_addr;
	gSVGA.fbMem = (void *)dev->regions[1].base_addr;
	gSVGA.fifoMem = (void *)dev->regions[2].base_addr;

	command = pci_device_cfg_read_u16(dev, 0, 4);
	pci_device_cfg_write_u16(dev, command | 7, 4);

	SVGA_WriteReg(SVGA_REG_ID, SVGA_ID_2);
	SVGA_WriteReg(SVGA_REG_ENABLE, true);

	gSVGA.vramSize = SVGA_ReadReg(SVGA_REG_VRAM_SIZE);
	gSVGA.fbSize = SVGA_ReadReg(SVGA_REG_FB_SIZE);
	gSVGA.fifoSize = SVGA_ReadReg(SVGA_REG_MEM_SIZE);

	pci_device_map_range(dev, (pciaddr_t)gSVGA.fbMem, (pciaddr_t)gSVGA.fbSize,
			PCI_DEV_MAP_FLAG_WRITABLE, (void *)&gSVGA.fbMem);
	pci_device_map_range(dev, (pciaddr_t)gSVGA.fifoMem, (pciaddr_t)gSVGA.fifoSize,
			PCI_DEV_MAP_FLAG_WRITABLE, (void *)&gSVGA.fifoMem);

	pci_cleanup(iter);

	return 0;
}

