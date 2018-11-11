/*
 * Code from virtunoid exploit for QEMU
 * https://github.com/nelhage/virtunoid/blob/master/virtunoid.c
 */

#include <assert.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>

#define PAGE_SHIFT  12
#define PAGE_SIZE   (1 << PAGE_SHIFT)
#define PFN_PRESENT (1ull << 63)
#define PFN_PFN     ((1ull << 55) - 1)

typedef uint64_t gpa_t;
typedef uint64_t gfn_t;
typedef void    *gva_t;

uint32_t page_offset(unsigned long addr) {
	return addr & ((1 << PAGE_SHIFT) - 1);
}

gfn_t gva_to_gfn(gva_t addr) {
	static int fd = -1;
	size_t off;
	uint64_t pte, pfn;


	if (fd < 0)
		fd = open("/proc/self/pagemap", O_RDONLY);
	if (fd < 0)
		errx(EXIT_FAILURE, "open");
	off = ((uintptr_t)addr >> 9) & ~7;

	if (lseek(fd, off, SEEK_SET) != off)
		errx(EXIT_FAILURE, "lseek");

	if (read(fd, &pte, 8) != 8)
		errx(EXIT_FAILURE, "read");
	if (!(pte & PFN_PRESENT))
		return (gfn_t)-1;

	pfn = pte & PFN_PFN;
	return pfn;
}

uint64_t gva_to_gpa(uint64_t addr) {
	gfn_t gfn = gva_to_gfn((gva_t)addr);
	assert(gfn != (gfn_t)-1);
	return (gfn << PAGE_SHIFT) | page_offset((unsigned long)addr);
}

