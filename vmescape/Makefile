exploit: mmu.c device.c exploit.c
	gcc -Wall -ggdb -std=gnu99 -DVBOX_WITH_HGCM -o exploit mmu.c device.c exploit.c -lpciaccess
clean:
	rm exploit

