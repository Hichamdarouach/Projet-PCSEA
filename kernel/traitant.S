    .text
# cette directive sert a rendre l'etiquette publique

    .globl traitant_IT_32 
#Function for the clock
    .globl traitant_IT_33
#Function for the keyboard
    .globl traitant_IT_49
#Function for the superuser syscall


traitant_IT_32:
# sauvegarde des registres importants
    pushl %eax
    pushl %edx
    pushl %ecx
# appel a la fonction C realisant le traitant
# a decommenter quand vous l'aurez implantee
    call tic_PIT_CLOCK
# restauration des registres importants
    popl %ecx
    popl %edx
    popl %eax
# fin du traitant
    iret


traitant_IT_33:
# sauvegarde des registres importants
    pushl %eax
    pushl %edx
    pushl %ecx
# appel a la fonction C realisant le traitant
# a decommenter quand vous l'aurez implantee
    call tic_PIT_KEYBOARD
# restauration des registres importants
    popl %ecx
    popl %edx
    popl %eax
# fin du traitant
    iret

traitant_IT_49:
# sauvegarde des registres importants
    pushl %eax
    pushl %edx
    pushl %ecx
# appel a la fonction C realisant le traitant
# a decommenter quand vous l'aurez implantee
    call tic_PIT_SUPERUSER
# restauration des registres importants
    popl %ecx
    popl %edx
    popl %eax
# fin du traitant
    iret
