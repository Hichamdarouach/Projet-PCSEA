#include "cpu.h"
#include "segment.h"
#include "interrupt.h"
#include "stdint.h"


void open_IRQ(uint32_t num_IRQ, bool masque)
{
    //Etape 1 : Lire la valeur actuelle du masque sur le port de données 0x21
    uint8_t masque_complet = inb(0x21);
    //Etape 2 : Ce qu'on a récupéré est un tableau de boolean (8 bits)
    //          qui correspondent chacun à une irq, on change le bit correspondant
    //1 si l'IRQ est masquée
    //0 si l'IRQ est autorisé
    if (masque)
        masque_complet = masque_complet | (1 << num_IRQ);
    else
        masque_complet = masque_complet & ~(1 << num_IRQ);
    //Etape 3 : Envoie du masque sur le port de données 0x21
    outb(masque_complet, 0x21);
}


void init_traitant_IT(int32_t num_IT, void (*traitant)(void))
{
    // Attach function traitant at num_it 
    // 0x1000 adresse table vecteur interruption
    // traitant -> fonction à appelée à chaque interruption

    uint32_t premierMot =
        ((uint32_t)(KERNEL_CS) << 16) | ((uint32_t)(traitant)&0x0000FFFF); // Poids fort -> Constante, poids faible -> fonction gérant l'interruption
    uint32_t deuxiemeMot = ((uint32_t)(traitant)&0xFFFF0000) | 0x0008E00;

    uint32_t *ptrVecteur = (uint32_t *)0x1000; // Début table des vecteurs d'interruptions

    ptrVecteur[num_IT * 2] = premierMot; // *2 deux mots par commandes
    ptrVecteur[num_IT * 2 + 1] = deuxiemeMot;
}
