;; Errors
;;  1 - No MultiBoot
;;  2 - No CPUID
;;  3 - No Long mode
;;  4 - No SSE

global start    ; Point of Entry for kernel
extern init

section .text
bits 32
; The entry point of the Omega kernel
start:
  ; Position the stack pointer in .bss
  mov esp, stack_top

  ; Run Checks
  call check_multiboot
  call check_cpuid
  call check_long_mode


  ; Init Kernel
  call init_page_tables
  call init_paging

  ; Enable SSE
  call init_SSE

  ; load the 64bit Global Descriptor Table
  lgdt [gdt64.pointer]

  ;update selectors
  mov ax, gdt64.data
  mov ss, ax
  mov ds, ax
  mov es, ax

  ; set cs register and initialize long mode
  jmp gdt64.code:init

; Ensure that multiboot 2 was used
check_multiboot:
  cmp eax, 0x36d76289
  jne .no_multiboot
  ret
.no_multiboot:
  mov al, "1"
  jmp error

; Check if CPUID is supported by attempting to flip the ID bit (bit 21)
; in the FLAGS register. If we can flip it, CPUID is available.
check_cpuid:

  ; Copy FLAGS in to EAX via stack
  pushfd
  pop eax

  ; Copy to ECX as well for comparing later on
  mov ecx, eax

  ; Flip the ID bit
  xor eax, 1 << 21

  ; Copy EAX to FLAGS via the stack
  push eax
  popfd

  ; Copy FLAGS back to EAX (with the flipped bit if CPUID is supported)
  pushfd
  pop eax

  ; Restore FLAGS from the old version stored in ECX (i.e. flipping the
  ; ID bit back if it was ever flipped).
  push ecx
  popfd

  ; Compare EAX and ECX. If they are equal then that means the bit
  ; wasn't flipped, and CPUID isn't supported.
  cmp eax, ecx
  je .no_cpuid
  ret
.no_cpuid:
  mov al, "2"
  jmp error


; Check for LM and error if not
check_long_mode:
    ; test if extended processor info in available
    mov eax, 0x80000000    ; implicit argument for cpuid
    cpuid                  ; get highest supported argument
    cmp eax, 0x80000001    ; it needs to be at least 0x80000001
    jb .no_long_mode       ; if it's less, the CPU is too old for long mode

    ; use extended info to test if long mode is available
    mov eax, 0x80000001    ; argument for extended processor info
    cpuid                  ; returns various feature bits in ecx and edx
    test edx, 1 << 29      ; test if the LM-bit is set in the D-register
    jz .no_long_mode       ; If it's not set, there is no long mode
    ret
.no_long_mode:
    mov al, "3"
    jmp error

; Check for SSE and enable it. If it's not supported throw error "a".
init_SSE:
    ; check for SSE
    mov eax, 0x1
    cpuid
    test edx, 1<<25
    jz .no_SSE

    ; enable SSE
    mov eax, cr0
    and ax, 0xFFFB      ; clear coprocessor emulation CR0.EM
    or ax, 0x2          ; set coprocessor monitoring  CR0.MP
    mov cr0, eax
    mov eax, cr4
    or ax, 3 << 9       ; set CR4.OSFXSR and CR4.OSXMMEXCPT at the same time
    mov cr4, eax

    ret
.no_SSE:
    mov al, "4"
    jmp error


; Initialize the page table
init_page_tables:
  ; map PDP to PML4
  mov eax, PDP_table
  or  eax, 0b11             ; present and writable
  mov [PML4_table], eax

  ; map PD[0] to PDP[0]
  mov eax, PD_table
  or eax, 0b11                   ; present and writable
  mov [PDP_table], eax

  ; map each PD entry to a huge 2MiB
  mov ecx, 0                ; reset counter

.map_PD:
  mov eax, 0x200000               ; 2MiB
  mul ecx                         ; start ecx at 2MiB
  or eax, 0b10000011              ; huge, writable, present
  mov [PD_table + ecx * 8], eax   ; map every ecx'th byte

  inc ecx                    ; increment counter
  cmp ecx, 512               ; if ecx == 512 the PD is mapped
  jne .map_PD

  ret


init_paging:
  ; load PML4 into the CR3 register
  mov eax, PML4_table
  mov cr3, eax

  ; Enable the PAE flag in cr4 for long mode
  mov eax, cr4
  or  eax, 1 << 5
  mov cr4, eax

  ; set long mode bit in EFER MSR
  mov ecx, 0xC0000080     ;The address of the intel MSR
  rdmsr                   ; Read MSR into eax
  or  eax, 1 << 8          ; set long mode bit
  wrmsr                   ; write to the MSR

  ;enable paging in cr0
  mov eax, cr0
  or  eax, 1 << 31
  mov cr0, eax

  ret

  ; Prints an error (ERR: al) in white text with a red background
  error:
        mov dword [0xb8000], 0x4f524f45
        mov dword [0xb8004], 0x4f3a4f52
        mov dword [0xb8008], 0x4f204f20
        mov byte  [0xb800a], al
        hlt

section .rodata
gdt64:
  dq 0 ; zero entry
.code: equ $ - gdt64                                  ; get the offset of use
  dq (1<<44) | (1<<47) | (1<<41) | (1<<43) | (1<<53) ; code segment
.data: equ $ - gdt64             ; get the offset for use later
  dq (1<<44) | (1<<47) | (1<<41) ; data segment
.pointer:
  dw $ - gdt64 -1
  dq gdt64

.bss:
align 4096                  ; Align and reserve for paging
PML4_table:                   ; Page-Map Level-4 Table (PML4)
  resb 4096
PDP_table:                   ; Page-Directory Pointer Table (PDP)
  resb 4096
PD_table:                   ; Page-Directory Table (PD)
  resb 4096

; Reserve space for the stack
stack_bottom:
  resb 64
stack_top: