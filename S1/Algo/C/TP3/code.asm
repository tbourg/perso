section .data    ; Variables initialis�es
        Buffer:           db 'Bonjour', 10    ; En ascii, 10 = '\n'. La virgule sert � concat�ner les chaines
        BufferSize:     equ $-Buffer    ; Taille de la chaine

section .text    ; Le code source est �crit dans cette section
        global _start     ; D�finition de l'entr�e du programme

_start:    ; Entr�e du programme

        mov eax, 4    ; Appel de sys_write
        mov ebx, 1    ; Sortie standard STDOUT
        mov ecx, Buffer    ; Chaine � afficher
        mov edx, BufferSize    ; Taille de la chaine
        int 80h    ; Interruption du kernel

        mov eax, 1    ; Appel de sys_exit
        mov ebx, 0    ; Code de retour
        int 80h    ; Interruption du kernel
