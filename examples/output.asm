.model large
.stack 1024
.data
	tmp dw 0
	readBuf db 6, 6 dup(0)
	id_a dw 0
	id_b dw 0
.code
print proc
	mov bx,ax
	cmp bx,0
	je pZero
	jg pPos
	not bx
	add bx,1
	xor ax,ax
	mov ah,02h
	mov dl,2Dh
	int 21h
pPos:
	mov ax,bx
	mov bx,10
	mov si,0
itoa:
	cmp ax,0
	je startPrint
	xor dx,dx
	div bx
	add dl,30h
	push dx
	inc si
	jmp itoa
startPrint:
	cmp si,0
	je donePrint
	pop dx
	dec si
	mov ah,02h
	int 21h
	jmp startPrint
pZero:
	mov dl,30h
	mov ah,02h
	int 21h
donePrint:
	mov ah,02h
	mov dl,13
	int 21h
	mov dl,10
	int 21h
	ret
print endp
scan proc
	mov dx, offset readBuf
	mov ah,0ah
	int 21h
	xor cx,cx
	mov cl,readBuf+1
	mov si,0
	mov bx,10
	mov ax,0
atoi:
	cmp si,cx
	je donescan
	mul bx
	xor dx,dx
	mov dl,readBuf[si+2]
	sub dx,30h
	add ax,dx
	inc si
	jmp atoi
donescan:
	mov bx,ax
	mov ah,02h
	mov dl,13
	int 21h
	mov dl,10
	int 21h
	mov ax,bx
	ret
scan endp
.startup
	push ax
	push bx
	push cx
	push dx
	push si
	push di
	pushf
	call scan
	mov id_a, ax
	popf
	pop di
	pop si
	pop dx
	pop cx
	pop bx
	pop ax
	push ax
	push bx
	push cx
	push dx
	push si
	push di
	pushf
	call scan
	mov id_b, ax
	popf
	pop di
	pop si
	pop dx
	pop cx
	pop bx
	pop ax
	mov ax,id_a
	imul word ptr id_b
	mov id_a,ax
	push ax
	push bx
	push cx
	push dx
	push si
	push di
	pushf
	mov ax, id_a
	call print
	popf
	pop di
	pop si
	pop dx
	pop cx
	pop bx
	pop ax
.exit
end
