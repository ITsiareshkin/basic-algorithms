/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
    *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    bst_node_t *iterator = tree;

    while (iterator) {
        if (tree->key == key) {
            *value = iterator->value;
            return true;
        }
        else
        if (iterator->key > key) {
            iterator = iterator->left;
        }
        else {
            iterator = iterator->right;
        }
    }
    return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    bst_node_t *iterator = *tree;
    bst_node_t *tmp = NULL;

    if (!*tree) {
        bst_node_t *insert = malloc(sizeof(struct bst_node));
        if (!insert) {
            return;
        }
        insert->key = key;
        insert->value = value;
        insert->left = NULL;
        insert->right = NULL;
        *tree = insert;
    }
    else {
        while (iterator) {
            tmp = iterator;
            if (iterator->key > key) {
                iterator = iterator->left;
                continue;
            }
            if (iterator->key < key) {
                iterator = iterator->right;
                continue;
            }
            else
            if (iterator->key == key) {
                iterator->value = value;
                return;
            }
        }
    }

    if (tmp) {
        bst_node_t *insert = malloc(sizeof(struct bst_node));
        if (!insert) {
            return;
        }
        insert->key = key;
        insert->value = value;
        insert->left = NULL;
        insert->right = NULL;

        if (insert->key < tmp->key) {
            tmp->left = insert;
        }
        else
        if (insert->key > tmp->key) {
            tmp->right = insert;
        }
        else {
            free(insert);
        }
    }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    bst_node_t *iterator = *tree;
    if (!iterator) {
        return;
    }

    bst_node_t *rightmost = NULL;

    while (iterator)
    {
        if(!iterator->right)
        {
            rightmost = iterator;
            target->key = rightmost->key;
            target->value = rightmost->value;
            target->left = NULL;

            if (rightmost->left)
            {
                target->left = rightmost->left;
            }

            free(rightmost);
            return;
        }
        else
        if (!iterator->right->right)
        {
            rightmost = iterator->right;
            target->key = rightmost->key;
            target->value = rightmost->value;
            iterator->right = NULL;

            if(rightmost->left)
            {
                iterator->right = rightmost->left;
            }

            free(rightmost);
            return;
        }
        iterator = iterator->right;
    }
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
    bst_node_t *iterator = *tree;
    if (!iterator) {
        return;
    }

    bst_node_t *tmp = NULL;
    bst_node_t *next = NULL;

    while (iterator) {
        tmp = iterator;
        if (iterator->key > key) {
            iterator = iterator->left;
        }
        else
        if (iterator->key < key) {
            iterator = iterator->right;
        }

        if (!iterator) {
            return;
        }

        if (iterator->key == key) {
            if (iterator->left && iterator->right) {
                bst_replace_by_rightmost(iterator, &iterator->left);
                return;
            }
            else
            if (!iterator->left && !iterator->right) {
                if (iterator != *tree) {
                    if (tmp->left == iterator) {
                        tmp->left = NULL;
                        free(iterator);
                    }
                    else {
                        tmp->right = NULL;
                        free(iterator);
                    }
                }
                else {
                    *tree = NULL;
                    free(iterator);
                }
            }
            else
            if (iterator->left && !iterator->right) {
                next = iterator->left;

                if (iterator != *tree) {
                    if (iterator == tmp->left) {
                        tmp->left = next;
                    }
                    else {
                        tmp->right = next;
                    }
                }
                else {
                    *tree = next;
                }
                free(iterator);
            }
            else
            if (!iterator->left && iterator->right) {
                next = iterator->right;

                if (iterator != *tree) {
                    if (iterator == tmp->left) {
                        tmp->left = next;
                    }
                    else {
                        tmp->right = next;
                    }
                }
                else {
                    *tree = next;
                }
                free(iterator);
            }
            return;
        }
    }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
    if(!*tree) {
        return;
    }

    stack_bst_t stack;
    stack_bst_init(&stack);
    stack_bst_push(&stack, *tree);

    while (!stack_bst_empty(&stack)) {
        bst_node_t *tmp = stack_bst_top(&stack);
        stack_bst_pop(&stack);

        if (tmp->left != NULL) {
            stack_bst_push(&stack, tmp->left);
        }
        else
        if (tmp->right != NULL) {
            stack_bst_push(&stack, tmp->right);
        }
        free(tmp);
    }
    bst_init(tree);
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
    while (tree) {
        stack_bst_push(to_visit, tree);
        bst_print_node(tree);
        tree = tree->left;
    }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
    stack_bst_t stack;
    stack_bst_init(&stack);

    bst_leftmost_preorder(tree, &stack);

    while (!stack_bst_empty(&stack)) {
        tree = stack_bst_top(&stack);
        stack_bst_pop(&stack);
        bst_leftmost_preorder(tree->right, &stack);
    }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
    while (tree) {
        stack_bst_push(to_visit, tree);
        tree = tree->left;
    }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
    stack_bst_t stack;
    stack_bst_init(&stack);

    bst_leftmost_inorder(tree, &stack);

    while (!stack_bst_empty(&stack)) {
        tree = stack_bst_top(&stack);
        stack_bst_pop(&stack);
        bst_print_node(tree);
        bst_leftmost_inorder(tree->right, &stack);
    }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
    while (tree) {
        stack_bst_push(to_visit, tree);
        stack_bool_push(first_visit, true);
        tree = tree->left;
    }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
    bool tmp;
    stack_bst_t stack;
    stack_bst_init(&stack);

    stack_bool_t stacktmp;
    stack_bool_init(&stacktmp);

    bst_leftmost_postorder(tree, &stack, &stacktmp);
    while (!stack_bst_empty(&stack)) {
        tree = stack_bst_top(&stack);
        tmp = stack_bool_top(&stacktmp);
        stack_bst_pop(&stack);
        stack_bool_pop(&stacktmp);
        if (tmp) {
            stack_bool_push(&stacktmp, false);
            stack_bst_push(&stack, tree);
            bst_leftmost_postorder(tree->right, &stack, &stacktmp);
        }
        else {
            bst_print_node(tree);
        }
    }
}
