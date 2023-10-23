ORG 0
BITS 16
_start:
	jmp short start
	nop
times 33 db 0

start:
	jmp 0x7c0:step2

handle_zero:
	mov ah, 0eh
	mov al, 'S'
	mov bx, 0x00
	int 0x10
	iret

handle_one:
	mov ah, 0eh
	mov al, 'I'
	mov bx, 0x00
	int 0x10
	iret

step2:
	cli			;Disable Interrupts
	mov ax, 0x7c0
	mov ds, ax
	mov ax, 0x00
	mov ss, ax
	mov sp, 0x7c00
	sti			;Enable Interrupts

	mov word[ss:0x00], handle_zero		;Set Satck segment to 00 so we can write at memroy location 0x0000
	mov word[ss:0x02], 0x7c0

	mov word[ss:0x04], handle_one		;Set Satck segment to 04 so we can write at memroy location 0x00004
	mov word[ss:0x06], 0x7c0

	;mov ax, 0x00
	;div ax

	int 1

	mov si, message
    call print
    jmp $

print:
    mov bx, 0
.loop:
    lodsb
    cmp al, 0
    je .done
    call print_char
    jmp .loop
.done:
    ret

print_char:
    mov ah, 0eh
    int 0x10
    ret

message: db 'Hello World!', 0

times 510-($ - $$) db 0
dw 0xAA55
