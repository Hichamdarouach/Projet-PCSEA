# PCSEA: x86

## Mode d'emploi :

Pour lancer l'OS, il n'y a pas de changement par rapport à la version de base. Le système à compiler et exécuter se trouve sur la **branche master**.

- La compilation se fait avec `make`
- L'exécution se fait avec `make run`
- Pour utiliser gdb

	* `cd kernel` 
	* `gdb kernel.bin`

## Tests  

- Les tests 1, 2, 3 et 4 **fonctionnent** en mode kernel
- Le test 5 génère une erreur lié à la gestion de l'instruction *return*. 

## Phases de développement

Le projet disponible sur la **branche master** est fonctionnel jusqu'à la **phase 4**

La phase 5 est presque opérationnelle mais nous sommes bloqué :

### Pour la pagination, le code est sur la **branche Step5_mem** : 

#### Eléments fonctionnels
	
- Allocateur de mémoire physique pour gérer les pages avec un mécanisme de type free-list (**free_list.c**)
- Création d'un page directory par processus 
- Fonctions permettant de mapper le page directory aux tables des pages, et de relier ses tables des pages à des pages physiques (**mem.c**)
- Changement de contexte ainsi que de registre CR3 stocké dans les processus (**ctx_sw.S**)

#### Eléments non fonctionnels 

- Les structures des pages sont stockées au début des pages. Pourtant, nous nous confrontons à un blue screen lors de l'appel de la fonction **init_free_list** contenu dans le fichier **free_list.c** car nous n'arrivons pas à modifier la valeur des variables, qui restent à 0 malgré des initialisations.
En effet, lorsque nous faisons un `page->champ = valeur`, gdb nous indique que la valeur n’a pas changée et est restée à 0x0.
	

### Pour la mémoire virtuelle, le code est sur la **branche master** :

#### Eléments fonctionnels

- Le mécanisme pour implémenter une bibliothèque d'appels système (récupération 
des paramètres passés par l'appelant, identification de l'appel système à 
lancer) 

- Le traitant de l'interruption 49 réagit bien, et une gestion de l'appel système waitpid telle qu'attendue dans l'étape 5 figure dans tic_PIT_SUPERUSER, située dans (**kernel_it_49.c**)

#### Eléments non fonctionnels

- Nous n'avons pas réussi à faire fonctionner complètement le côté user, nous ne nous sommes pas attelés à gérer tous les appels système, uniquement celui de waitpid pour mettre en place le fonctionnement de l'API.
De plus nous n'utilisons les appels systèmes implémentés.


## Groupe

* Alexian Serrano
* Clement Zangrilli 
* Hicham Darouach
* Jérémy Navarro
* Locif Azi 
* William Goulois 