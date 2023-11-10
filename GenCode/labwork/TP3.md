# TP2 : Traduction en quadruplets

_Pour réaliser le TP, suivez les directives qui vont suivre et essayez de v indicatif fourni pour chaque partieous conformer au temps._

Le TP se clôture par la remise des sources que vous aurez produits sur le dépôt correspondant sur Moodle.

Pour obtenir l'archive à déposer, tapez la commande :
	`$ make tp3`

Cela crée un fichier nommé `tp3-DATE.tgz` avec _DATE_ représentant la date du jour.


## Présentation du sélecteur d'instruction

Le _sélecteur d'instruction_ est implanté dans le fichier `Inst.cpp` qui exploite les classes déclarées dans `Inst.hpp`. Deux classes y sont définies :

* Le classe `Inst` définie une instruction machine sous format d'une chaîne de caractère et d'une liste de paramètre repris dans la chaîne de caractère par des `%0`, `%1`, etc.

* La classe `Param` définit les éléments des paramètres de `Inst`.

Les paramètres sont de 2 types :

* `Param::CST` pour définir une valeur constant,
* `Param::READ`  pour un registre lu,
* `Param::WRITE` pour un registre écrit.

Ces types vont également être utilisés plus tard pour l'_allocation des registres_.

Par exemple, l'instruction `add R6, R0, R1` sera déclarée par :

	```C++
		Inst add("add %0, %1, %2", Param::write(6), Param::read(0), Param::read(1));
	```

L'instruction `mov R3, #128` sera décrite par :

	```C++
		Inst movi("mov %0, #%1", Param::write(3), Param::cst(128));
	```


## Fonctionnement du sélecteur d'instruction

Le sélecteur implante un algorithme très simple : on lui donne une liste de quadruplets et il renvoie une liste d'instructions machine. Pour ce faire, il recherche la première entrée du tableau `selector` défini dans `Inst.cpp` correspond avec le début de la liste de quadruplets (cela peut être plusieurs quadruplets). Il les supprime de la liste des quadruplets et ajoute à les instructions machines correspondantes à la fin de la liste de instructions. Il réitère jusqu'à avoir consommé tous les quadruplets de la liste.

Pour ajouter une instruction machine, il suffit alors d'ajouter une entrée au tableau `selector`. Les éléments de ce tableau, de type `select_t`, sont un couple :

* une liste de quadruplets à reconnaître,
* une liste d'instruction machine à générer.

Par exemple, on pourra reconnaître l'instruction `add` de l'ARM avec le sélecteur ci-dessous :

	```C++
	select_t
		select_add = {
			{ Quad::add(RECORD|0, RECORD|1, RECORD|2) },
			{ Inst("\tadd R%0, R%1, R%2", Param::write(COPY|0), Param::read(COPY|1), Param::read(COPY|2)), Inst::end }
		};
	```

Ainsi le simple quadruplet `v31 <- v50 + v49` génèrera l'instruction machine `add R31, R50, R49`. On notera que le modèle peut être composé de plusieurs quadruplets en séquence et qu'il peut y avoir plusieurs machines générées.

Pour réaliser l'identification des paramètres de quadruplet, les registres utilisées pour ces derniers sont décorés d'une action et d'un numéro comme `RECORD|0`. Ces actions ont la signification suivante :

* `RECORD|i` - enregistrer le numéro de registre dans la mémoire _i_.
* `EQUAL|i` - le numéro de registre doit être égal à la mémoire _i_.
* `POW2|i` - correspond si le paramètre est une constante puissance de 2 et enregistrer dans _i_.
* `ISIMM|i` - correspond si le paramètre est une constante encodable dans une instruction ARM et enregistrer dans _i_.

Les mémoires permettent de faire le lien avec les valeurs passées dans les paramètres des instructions comme `COPY|0`:

* `COPY|i` - copier la valeur de la mémoire _i_.
* `LOG2|i` - copier le logarithme de 2 de la mémoire _i_.

Ainsi, dans notre exemple, la mémoire _0_ reçoit 31, la mémoire _1_ reçoit 50 et la mémoire _2_ 49. Quand on génèrera l'instruction machine, le premier paramètre prendra 31 et %0 sera remplacé par 31, etc.

Prenons comme autre exemple le `mov` avec une constante :

	```C++
	select_t
		select_movi = {
			{ Quad::seti(RECORD|0, ISIMM|1) },
			{ Inst("\tmov R%0, #%1", Param::write(COPY|0), Param::cst(COPY|1)), Inst::end }
		};
	```

Si on doi générer le quadruplet `v45 <- 1024` alors la mémoire _0_ reçoit 45. La mémoire _1_ reçoit 1024 car 1024 est encodable comme une constante immédiate en ARM.


## A faire

Quand on lance le sélecteur d'instruction et qu'il ne trouve pas une instruction, il affiche un avertissement avec le quadruplets qui n'est pas supporté.

[TODO]

1. En utilisant comme test `test/blink.io` et `test/pushdown.io`, ajoutez des instructions jusqu'à ce qu'il n'y est plus d'avertissement.

**NOTE 1** On pourra utiliser l'option `-print-select` pour afficher le résultat de la sélection des instructions.

**NOTE 2** Pour réduire le travail d'écriture, on notera l'usage des fonction _inline_ `pread`, `pwrite` et `pcst` à la place de `Param::read`, `Param::write` et `Param::cst`.

**ATTENTION** Les structures `select_t` doivent être déclarées séparemment puis leur adresse ajoutée aui tableau `selectors`.

2. Ajoutez des entrées `select_t` qui permettent de remplacer les séquences `vi <- n; vj <- vk op vi` par une seule instruction machine dont le second paramètre est une valeur immédiate.

3. Utilisez le sélecteur d'instruction pour optimiser les séquences du type :

	* `goto l; label l` par `label l`
	* `goto_cc vi, vj, L; goto L'; label L` par `goto_cc\ vi, vj, L'; label L`

4. Certaines opérations peuvent également être simplifiées.

	* `vi <- 2^n; vj <- vk * vi` par `vi <- vk << n`
	* `vi <- 2^n; vj <- vk / vi` par `vi <- vk >> n`

5. Certains résultats triviaux peuvent donner à des remplacement.

	* `vi <- 0; vj <- vk + vi` par `vj <- vk`
	* `vi <- 0; vj <- vk - vi` par `vj <- vk`
	* `vi <- 0; vj <- vi - vk` par `vj <- -vk`
	* `vi <- 0; vj <- vk * vi` par `vj <- 0`
	* `vi <- 1; vj <- vk * vi` par `vj <- vk`


## Instructions ARM

La liste ci-dessous donnent les instructions ARM utiles pour notre traducteur. Les notations suivantes sont utilisées :

* _IMM_ - valeur immédiate avec un codage constant (8-bit) >>> rotation (4-bit).
* _Ri_, _Rj_, _Rk_ - un registre.
* _LAB_ - une étiquette.
* _CC_ - une condition (EQ, LE, GT, GE, LT, LE).

Les instructions arithmétiques :
* `add Ri, Rj, Rk` - Ri <- Rj + Rk
* `add Ri, Rj, #IMM` - Ri <- Rj + IMM
* `and Ri, Rj, Rk` - Ri <- Rj & Rk
* `and Ri, Rj, #IMM` - Ri <- Rj & IMM
* `div Ri, Rj, Rk` - Ri <- Rj / Rk
* `eor Ri, Rj, Rk` - Ri <- Rj ^ Rk
* `eor Ri, Rj, #IMM` - Ri <- Rj ^ IMM
* `mov Ri, Rj` - Ri <- Rj
* `mov Ri, Rj, #IMM` - Ri <- IMM
* `mov Ri, Rj, lsl Rk` - Ri <- Rj << Rk
* `mov Ri, Rj, lsl #IMM` - Ri <- Rj << IMM
* `mov Ri, Rj, lsr Rk` - Ri <- Rj >> Rk
* `mov Ri, Rj, lsr #IMM` - Ri <- Rj >> IMM
* `mul Ri, Rj, Rk` - Ri <- Rj * Rk
* `mvn Ri, Rj` - Ri <- ~Rj
* `orr Ri, Rj, Rk` - Ri <- Rj | Rk
* `orr Ri, Rj, #IMM` - Ri <- Rj | IMM
* `rsb Ri, Rj, Rk` - Ri <- Rk - Rj
* `rsb Ri, Rj, #IMM` - Ri <- IMM + Rj
* `sub Ri, Rj, Rk` - Ri <- Rj - Rk
* `sub Ri, Rj, #IMM` - Ri <- Rj - IMM

Les accès mémoire :
* `ldr Ri, [Rj]` - Ri <- Mint[Rj]
* `str Ri, [Rj]` - Mint[Rj] <- Ri

Les branchements :
* `b LAB` - PC <- LAB
* `bCC LAB` - si SR[CC] alors PC <- LAB
* `bx Ri` - PC <- Ri
* `cmp Ri, Rj` - SR <- Ri ~ Rj
* `cmp Ri, #IMM` - SR <- Ri ~ IMM
