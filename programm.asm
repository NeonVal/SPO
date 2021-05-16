;MyCode
.8086
.model small
.stack 100h
.data
; support data
@buffer db 6
blength db (?)
@buf db 256 DUP (?)
err_msg db "Input error, try again", 0Dh, 0Ah, "$" 
@true db "true"
@false db "false"
@@true db "true$"
@@false db "false$"
clrf db 0Dh, 0Ah,"$"
output db 6 DUP (?), "$"
; user data
a dw  (?)
b dw  (?)
d dw  (?)
c dw 4 DUP (?)
x db  (?)
y db  (?)
z db  (?)
flag db  (?)
@M0 db "Enter number, number, char, bool$"
@M18 db "a=$"
@M21 db "b=$"
@M24 db "d=$"
@M27 db "c[0]=$"
@M30 db "c[1]=$"
@M33 db "c[2]=$"
@M36 db "c[3]=$"
@M39 db "x=$"
@M40 db "y=$"
@M41 db "z=$"
@M42 db "flag=$"
@M50 db "a=$"
@M58 db "a=$"
@M65 db "a=$"
@M72 db "a=$"
@M75 db "d=$"
.code
main:
mov ax, @data
mov ds, ax
mov es, ax
xor ax, ax
; Write()
lea dx, clrf
mov ah, 9
int 21h
lea dx, @M0
mov ah, 9
int 21h
push 0
@M1:
mov ah, 0Ah
lea dx, @buffer
int 21h
mov ax, 0
mov cx, 0
mov cl, byte ptr[blength]
mov bx, cx
@M2:
dec bx
mov al, @buf[bx]
cmp al, "9"
ja @M4
cmp al, "0"
jb @M4
loop @M2
mov cl, byte ptr[blength]
mov di, 0
mov ax, 0
@M5:
mov bl, @buf[di]
inc di
sub bl, 30h
add ax, bx
mov si, ax
mov bx, 10
mul bx
loop @M5
mov ax, si
pop di
shl di, 1
mov a[di], ax
jmp @M3
@M4:
lea dx, err_msg
mov ah, 9
int 21h
jmp @M1
@M3:
lea dx, clrf
mov ah, 9
int 21h
mov ax, 0
push ax
@M6:
mov ah, 0Ah
lea dx, @buffer
int 21h
mov ax, 0
mov cx, 0
mov cl, byte ptr[blength]
mov bx, cx
@M7:
dec bx
mov al, @buf[bx]
cmp al, "9"
ja @M9
cmp al, "0"
jb @M9
loop @M7
mov cl, byte ptr[blength]
mov di, 0
mov ax, 0
@M10:
mov bl, @buf[di]
inc di
sub bl, 30h
add ax, bx
mov si, ax
mov bx, 10
mul bx
loop @M10
mov ax, si
pop di
shl di, 1
mov c[di], ax
jmp @M8
@M9:
lea dx, err_msg
mov ah, 9
int 21h
jmp @M6
@M8:
lea dx, clrf
mov ah, 9
int 21h
push 0
mov ah, 0Ah
lea dx, @buffer
int 21h
xor dx, dx
mov dl, @buf[0]
pop di
mov z[di], dl
lea dx, clrf
mov ah, 9
int 21h
push 0
@M11:
mov ah, 0Ah
lea dx, @buffer
int 21h
cmp blength, 4
je @M12
cmp blength, 5
je @M13
jmp @M17
@M12:
lea si, @true
lea di, @buf
mov cx, 4
repe cmpsb
jz @M14
jmp @M17
@M13:
lea si, @false
lea di, @buf
mov cx, 5
repe cmpsb
jz @M14
jmp @M17
@M14:
cmp @buf[0], "t"
je @M15
push 0
jmp @M16
@M15:
push 1
jmp @M16
@M17:
lea dx, err_msg
mov ah, 9
int 21h
jmp @M11
@M16:
pop ax
pop di
mov flag[di], al
lea dx, clrf
mov ah, 9
int 21h
; Appr
mov ax, 0
push ax
pop di
shl di, 1
mov ax, c[di]
push ax
pop ax
mov b, ax
; Appr
push "Y"
pop ax
mov y, al
; Appr
mov di, 0
mov al, y[di]
push ax
pop ax
mov x, al
; Appr
mov di, 0
shl di, 1
mov ax, a[di]
push ax
mov di, 0
shl di, 1
mov ax, b[di]
push ax
pop bx
pop ax
add ax, bx
push ax
pop ax
mov d, ax
; Appr
mov ax, 1
push ax
mov di, 0
shl di, 1
mov ax, b[di]
push ax
mov di, 0
shl di, 1
mov ax, a[di]
push ax
pop bx
pop ax
sub ax, bx
push ax
pop ax
pop di
shl di, 1
mov c[di], ax
; Appr
mov ax, 2
push ax
mov di, 0
shl di, 1
mov ax, b[di]
push ax
mov di, 0
shl di, 1
mov ax, a[di]
push ax
pop bx
pop ax
xor dx, dx
mul bx
push ax
pop ax
pop di
shl di, 1
mov c[di], ax
; Appr
mov ax, 3
push ax
mov di, 0
shl di, 1
mov ax, b[di]
push ax
mov di, 0
shl di, 1
mov ax, a[di]
push ax
pop bx
pop ax
xor dx, dx
div bx
push ax
pop ax
pop di
shl di, 1
mov c[di], ax
; Write()
lea dx, clrf
mov ah, 9
int 21h
lea dx, @M18
mov ah, 9
int 21h
mov di, 0
shl di, 1
mov ax, a[di]
push ax
pop ax
mov bx, 10
mov di, 0
mov si, ax
cmp ax, 0
jns @M19
neg si
mov ah, 2
mov dl, "-"
int 21h
mov ax, si
@M19:
mov dx, 0
div bx
add dl, 30h
mov output[di], dl
inc di
cmp al, 0
jnz @M19
mov cx, di
dec di
mov ah, 2
@M20:
mov dl, output[di]
dec di
int 21h
loop @M20
; Write()
lea dx, clrf
mov ah, 9
int 21h
lea dx, @M21
mov ah, 9
int 21h
mov di, 0
shl di, 1
mov ax, b[di]
push ax
pop ax
mov bx, 10
mov di, 0
mov si, ax
cmp ax, 0
jns @M22
neg si
mov ah, 2
mov dl, "-"
int 21h
mov ax, si
@M22:
mov dx, 0
div bx
add dl, 30h
mov output[di], dl
inc di
cmp al, 0
jnz @M22
mov cx, di
dec di
mov ah, 2
@M23:
mov dl, output[di]
dec di
int 21h
loop @M23
; Write()
lea dx, clrf
mov ah, 9
int 21h
lea dx, @M24
mov ah, 9
int 21h
mov di, 0
shl di, 1
mov ax, d[di]
push ax
pop ax
mov bx, 10
mov di, 0
mov si, ax
cmp ax, 0
jns @M25
neg si
mov ah, 2
mov dl, "-"
int 21h
mov ax, si
@M25:
mov dx, 0
div bx
add dl, 30h
mov output[di], dl
inc di
cmp al, 0
jnz @M25
mov cx, di
dec di
mov ah, 2
@M26:
mov dl, output[di]
dec di
int 21h
loop @M26
; Write()
lea dx, clrf
mov ah, 9
int 21h
lea dx, @M27
mov ah, 9
int 21h
mov ax, 0
push ax
pop di
shl di, 1
mov ax, c[di]
push ax
pop ax
mov bx, 10
mov di, 0
mov si, ax
cmp ax, 0
jns @M28
neg si
mov ah, 2
mov dl, "-"
int 21h
mov ax, si
@M28:
mov dx, 0
div bx
add dl, 30h
mov output[di], dl
inc di
cmp al, 0
jnz @M28
mov cx, di
dec di
mov ah, 2
@M29:
mov dl, output[di]
dec di
int 21h
loop @M29
; Write()
lea dx, clrf
mov ah, 9
int 21h
lea dx, @M30
mov ah, 9
int 21h
mov ax, 1
push ax
pop di
shl di, 1
mov ax, c[di]
push ax
pop ax
mov bx, 10
mov di, 0
mov si, ax
cmp ax, 0
jns @M31
neg si
mov ah, 2
mov dl, "-"
int 21h
mov ax, si
@M31:
mov dx, 0
div bx
add dl, 30h
mov output[di], dl
inc di
cmp al, 0
jnz @M31
mov cx, di
dec di
mov ah, 2
@M32:
mov dl, output[di]
dec di
int 21h
loop @M32
; Write()
lea dx, clrf
mov ah, 9
int 21h
lea dx, @M33
mov ah, 9
int 21h
mov ax, 2
push ax
pop di
shl di, 1
mov ax, c[di]
push ax
pop ax
mov bx, 10
mov di, 0
mov si, ax
cmp ax, 0
jns @M34
neg si
mov ah, 2
mov dl, "-"
int 21h
mov ax, si
@M34:
mov dx, 0
div bx
add dl, 30h
mov output[di], dl
inc di
cmp al, 0
jnz @M34
mov cx, di
dec di
mov ah, 2
@M35:
mov dl, output[di]
dec di
int 21h
loop @M35
; Write()
lea dx, clrf
mov ah, 9
int 21h
lea dx, @M36
mov ah, 9
int 21h
mov ax, 3
push ax
pop di
shl di, 1
mov ax, c[di]
push ax
pop ax
mov bx, 10
mov di, 0
mov si, ax
cmp ax, 0
jns @M37
neg si
mov ah, 2
mov dl, "-"
int 21h
mov ax, si
@M37:
mov dx, 0
div bx
add dl, 30h
mov output[di], dl
inc di
cmp al, 0
jnz @M37
mov cx, di
dec di
mov ah, 2
@M38:
mov dl, output[di]
dec di
int 21h
loop @M38
; Write()
lea dx, clrf
mov ah, 9
int 21h
lea dx, @M39
mov ah, 9
int 21h
mov di, 0
mov ax, 0
mov al, x[di]
mov dl, al
mov ah, 2
int 21h
; Write()
lea dx, clrf
mov ah, 9
int 21h
lea dx, @M40
mov ah, 9
int 21h
mov di, 0
mov ax, 0
mov al, y[di]
mov dl, al
mov ah, 2
int 21h
; Write()
lea dx, clrf
mov ah, 9
int 21h
lea dx, @M41
mov ah, 9
int 21h
mov di, 0
mov ax, 0
mov al, z[di]
mov dl, al
mov ah, 2
int 21h
; Write()
lea dx, clrf
mov ah, 9
int 21h
lea dx, @M42
mov ah, 9
int 21h
mov di, 0
mov ax, 0
mov al, flag[di]
push ax
pop ax
cmp ax, 0
je @M43
lea dx, @@true
jmp @M44
@M43:
lea dx, @@false
@M44:
mov ah, 9
int 21h
; Cond()
mov di, 0
mov ax, 0
mov al, flag[di]
push ax
push 1
pop bx
pop ax
cmp ax, bx
je @M45
push 0
jmp @M46
@M45:
push 1
@M46:
pop ax
cmp ax, 0
jz @M48
jmp @M47
@M47:
; Write()
lea dx, clrf
mov ah, 9
int 21h
lea dx, @M50
mov ah, 9
int 21h
mov di, 0
shl di, 1
mov ax, a[di]
push ax
pop ax
mov bx, 10
mov di, 0
mov si, ax
cmp ax, 0
jns @M51
neg si
mov ah, 2
mov dl, "-"
int 21h
mov ax, si
@M51:
mov dx, 0
div bx
add dl, 30h
mov output[di], dl
inc di
cmp al, 0
jnz @M51
mov cx, di
dec di
mov ah, 2
@M52:
mov dl, output[di]
dec di
int 21h
loop @M52
jmp @M49
@M48:
@M49:
; Cond()
mov di, 0
shl di, 1
mov ax, a[di]
push ax
mov di, 0
shl di, 1
mov ax, b[di]
push ax
pop bx
pop ax
cmp ax, bx
ja @M53
push 0
jmp @M54
@M53:
push 1
@M54:
pop ax
cmp ax, 0
jz @M56
jmp @M55
@M55:
; Appr
mov di, 0
shl di, 1
mov ax, a[di]
push ax
mov di, 0
shl di, 1
mov ax, b[di]
push ax
pop bx
pop ax
sub ax, bx
push ax
pop ax
mov a, ax
jmp @M57
@M56:
; Appr
mov ax, 0
push ax
pop ax
mov a, ax
@M57:
; Write()
lea dx, clrf
mov ah, 9
int 21h
lea dx, @M58
mov ah, 9
int 21h
mov di, 0
shl di, 1
mov ax, a[di]
push ax
pop ax
mov bx, 10
mov di, 0
mov si, ax
cmp ax, 0
jns @M59
neg si
mov ah, 2
mov dl, "-"
int 21h
mov ax, si
@M59:
mov dx, 0
div bx
add dl, 30h
mov output[di], dl
inc di
cmp al, 0
jnz @M59
mov cx, di
dec di
mov ah, 2
@M60:
mov dl, output[di]
dec di
int 21h
loop @M60
; Cycle()
@M61:
mov di, 0
shl di, 1
mov ax, a[di]
push ax
mov di, 0
shl di, 1
mov ax, b[di]
push ax
pop bx
pop ax
cmp ax, bx
jb @M63
push 0
jmp @M64
@M63:
push 1
@M64:
pop ax
cmp ax, 0
jz @M62
; Appr
mov di, 0
shl di, 1
mov ax, a[di]
push ax
mov ax, 1
push ax
pop bx
pop ax
add ax, bx
push ax
pop ax
mov a, ax
jmp @M61
@M62:
; Write()
lea dx, clrf
mov ah, 9
int 21h
lea dx, @M65
mov ah, 9
int 21h
mov di, 0
shl di, 1
mov ax, a[di]
push ax
pop ax
mov bx, 10
mov di, 0
mov si, ax
cmp ax, 0
jns @M66
neg si
mov ah, 2
mov dl, "-"
int 21h
mov ax, si
@M66:
mov dx, 0
div bx
add dl, 30h
mov output[di], dl
inc di
cmp al, 0
jnz @M66
mov cx, di
dec di
mov ah, 2
@M67:
mov dl, output[di]
dec di
int 21h
loop @M67
; Repeat()
@M68:
; Appr
mov di, 0
shl di, 1
mov ax, a[di]
push ax
mov ax, 2
push ax
pop bx
pop ax
add ax, bx
push ax
pop ax
mov a, ax
; Appr
mov di, 0
shl di, 1
mov ax, d[di]
push ax
mov ax, 1
push ax
pop bx
pop ax
add ax, bx
push ax
pop ax
mov d, ax
mov di, 0
shl di, 1
mov ax, a[di]
push ax
mov di, 0
shl di, 1
mov ax, d[di]
push ax
pop bx
pop ax
cmp ax, bx
ja @M70
push 0
jmp @M71
@M70:
push 1
@M71:
pop ax
cmp ax, 0
jnz @M69
jmp @M68
@M69:
; Write()
lea dx, clrf
mov ah, 9
int 21h
lea dx, @M72
mov ah, 9
int 21h
mov di, 0
shl di, 1
mov ax, a[di]
push ax
pop ax
mov bx, 10
mov di, 0
mov si, ax
cmp ax, 0
jns @M73
neg si
mov ah, 2
mov dl, "-"
int 21h
mov ax, si
@M73:
mov dx, 0
div bx
add dl, 30h
mov output[di], dl
inc di
cmp al, 0
jnz @M73
mov cx, di
dec di
mov ah, 2
@M74:
mov dl, output[di]
dec di
int 21h
loop @M74
; Write()
lea dx, clrf
mov ah, 9
int 21h
lea dx, @M75
mov ah, 9
int 21h
mov di, 0
shl di, 1
mov ax, d[di]
push ax
pop ax
mov bx, 10
mov di, 0
mov si, ax
cmp ax, 0
jns @M76
neg si
mov ah, 2
mov dl, "-"
int 21h
mov ax, si
@M76:
mov dx, 0
div bx
add dl, 30h
mov output[di], dl
inc di
cmp al, 0
jnz @M76
mov cx, di
dec di
mov ah, 2
@M77:
mov dl, output[di]
dec di
int 21h
loop @M77
mov ax, 4C00h
int 21h
end main
