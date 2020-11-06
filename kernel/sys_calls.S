    .text
# cette directive sert a rendre l'etiquette publique

    .globl starte
    .globl getpide
    .globl getprioe
    .globl chprioe
    .globl kille
    .globl waitpide
    .globl exite
    .globl clock_settingse
    .globl current_clocke
    .globl wait_clocke



starte:
    mov $1, %eax

    int $49
    ret

getpide:
    mov $2, %eax

    int $49
    ret

getprioe:
    mov $3, %eax

    int $49
    ret

chprioe:
    mov $4, %eax

    int $49
    ret

kille:
    movl -12(%ebp), %ecx
    mov $5, %eax
    int $49
    ret

waitpide:
    push   %ebp
    mov    %esp,%ebp
    mov    $6, %ecx
    mov    0x8(%ebp),%edx
    mov    0xc(%ebp),%esi
    int    $49
    leave
    ret


exite:
    mov $7, %eax

    int $49
    ret

clock_settingse:
    mov $24, %eax

    int $49
    ret

current_clocke:
    mov $25, %eax

    int $49
    ret

wait_clocke:
    mov $26, %eax

    int $49
    ret