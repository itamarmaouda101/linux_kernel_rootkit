cmd_/home/itamar/6/linux_kernel_rootkit/src/rootkit.ko := ld -r -m elf_x86_64  -z max-page-size=0x200000  --build-id  -T ./scripts/module-common.lds -o /home/itamar/6/linux_kernel_rootkit/src/rootkit.ko /home/itamar/6/linux_kernel_rootkit/src/rootkit.o /home/itamar/6/linux_kernel_rootkit/src/rootkit.mod.o;  true
