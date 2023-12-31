/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    if (!tree) {
        return false;
    }

    if (tree->key == key) {
        *value = tree->value;
        return true;
    }
    else
    if (tree->key > key) {
        return bst_search(tree->left, key, value);
    }
    else {
        return bst_search(tree->right, key, value);
    }
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    if (!*tree) {
        if (!((*tree) = malloc(sizeof(struct bst_node)))) {
            return;
        }
        (*tree)->key = key;
        (*tree)->value = value;
        (*tree)->left = NULL;
        (*tree)->right = NULL;
        return;
    }

    if ((*tree)->key == key) {
        (*tree)->value = value;
        return;
    }
    else
    if ((*tree)->key > key) {
        bst_insert(&((*tree)->left), key, value);
    }
    else {
        bst_insert(&((*tree)->right), key, value);
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    if (!*tree) {
        return;
    }

    bst_node_t *rightmost;

    if (!(*tree)->right) {
        rightmost = *tree;
        target->key = rightmost->key;
        target->value = rightmost->value;

        target->left = NULL;
        if (rightmost->left) {
            target->left = rightmost->left;
        }

        free(rightmost);
        return;
    }
    else
    if (!(*tree)->right->right) {
        rightmost = (*tree)->right;

        target->key = rightmost->key;
        target->value = rightmost->value;

        (*tree)->right = NULL;
        if (rightmost->left) {
            (*tree)->right = rightmost->left;
        }
        free(rightmost);
        return;
    }
    bst_replace_by_rightmost(target, &((*tree)->right));
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
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
    if (!*tree) {
        return;
    }

    bst_node_t *del;

    if ((*tree)->key > key) {
        if ((*tree)->left && (*tree)->left->key == key) {
            del = (*tree)->left;

            if (del->left && del->right) {
                bst_replace_by_rightmost(del, &(del->left));
                return;
            }
            else
            if (del->left && !del->right) {
                (*tree)->left = del->left;
            }
            else
            if (!del->left && del->right) {
                (*tree)->left = del->right;
            }
            else {
                (*tree)->left = NULL;
            }
            free(del);
            return;
        }
        bst_delete(&((*tree)->left),key);
        return;
    }
    else
    if ((*tree)->key < key) {
        if ((*tree)->right && (*tree)->right->key == key) {
            del = (*tree)->right;

            if (del->left && del->right) {
                bst_replace_by_rightmost(del, &(del->left));
                return;
            }
            else
            if (!del->left && del->right) {
                (*tree)->right = del->right;
            }
            else
            if (del->left && !del->right) {
                (*tree)->right = del->left;
            }
            else {
                (*tree)->right = NULL;
            }
            free(del);
            return;
        }
        bst_delete(&((*tree)->right),key);
        return;
    }
    else
    if ((*tree)->key == key) {
        del = *tree;
        if (del->left && del->right) {
            bst_replace_by_rightmost(del, &(del->left));
            return;
        }
        else
        if (del->left && !del->right) {
            (*tree) = del->left;
        }
        else
        if (!del->left && del->right) {
            (*tree) = del->right;
        }
        else {
            (*tree) = NULL;
        }
        free(del);
    }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
    if (!*tree) {
        return;
    }

    bst_dispose(&((*tree)->left));
    bst_dispose(&((*tree)->right));

    free(*tree);
    *tree = NULL;
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
    if (tree) {
        bst_print_node(tree);
        bst_preorder(tree->left);
        bst_preorder(tree->right);
    }
    else {
        return;
    }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
    if (tree) {
        bst_inorder(tree->left);
        bst_print_node(tree);
        bst_inorder(tree->right);
    }
    else {
        return;
    }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
    if (tree) {
        bst_postorder(tree->left);
        bst_postorder(tree->right);
        bst_print_node(tree);
    }
    else {
        return;
    }
}
