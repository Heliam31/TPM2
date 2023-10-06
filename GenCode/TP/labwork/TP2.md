# TP2 : Traduction en quadruplets

_Pour réaliser le TP, suivez les directives qui vont suivre et essayez de v indicatif fourni pour chaque partieous conformer au temps._

Le TP se clôture par la remise des sources que vous aurez produits sur le dépôt correspondant sur Moodle.

Pour obtenir l'archive à déposer, tapez la commande :
	`$ make tp2`

Cela crée un fichier nommé `tp2-DATE.tgz` avec _DATE_ représentant la date du jour.


## Prise en main de la traduction (30mn)

Les quadruplets sont définis dans le fichier `Quad.hpp` où vous trouverez les types suivants :

* `lab_t` -- type identifiant le numéro des étiquettes dans le code assembleur,
* `reg_t` -- type représentant le numéro des registres virtuelles (notez que les 16 premiers numéros sont associés aux registres matériel de l'ARM, R0 à R15).
* `Quad` -- classe représentant un quadruplet.
* `QuadProgram` -- classe représentant un programme de quadruplet en cours de génération.

### Les quadruplets

Comme il se doit, la classe Quad contient plusieurs champs dont l'interprétation peut dépendre du type de quadruplet : `type` (type du quadruplet), `d`, `a`, `b`. En général, `d` représente le numéro du registre destination, `a` le numéro du registre premier opérande et `b` le numéro du registre second opérande (s'il y en a un).

La règle ci-dessus s'applique à la plupart des quadruplets réalisant un calcul. Pour les branchements (`GOTO`), `d` contient le numero du label sur lequel on branche. Pour l'affectation de constante (`SETI`), `a` contient la constante affectée. Pour les accès mémoire, `a` contient le numéro de registre représentant l'adresse accédée.

Pour vous aider à construire des quadruplets, la class `Quad` contient une liste de fonctions statiques permettant de construire des quadruplets d'un type donné et de passer les bons paramètres : `Quad::add`, `Quad::seti`, `Quad::goto`, etc.

On notera le pseudo-quadruplet de type `LAB` qui permet de définir une étiquette sur lasquelle on pourra brancher.

> On notera que les quadrplets peuvent afficher sur la sortie standard `cout`.


# La représentation du programme

La classe `QuadProgram` représente le programme sous forme d'une liste de quadruplets. Pour ajouter un quadruplet à la fin du programme actuel, on utilisera la fonction `QuadProgram::emit()`.

Elle fournit également quelques services pour aider à la génération du code :

* `QuadProgram::new Reg()` -- permet d'obtenir un nouveau numéro de registre non-utilisé,
* `QuadProgram::newLab()` -- permet d'obtenir un nouveau numéro d'étiquette non-utilisé,
* `QuadProgram::declare()` -- alloue un nouveau registre et l'associe avec un identificateur (pratique pour les variables),
* `QuadProgram::regFor()` -- permet d'obtenir le numéro de registre associé avec un identificateur (créé avec l'appel précédent).



## La traduction d'un automate

La notion d'automate n'a pas d'équivalent dans le type de machine cible que nous visons, ici, reflétés par les quadruplets. Il va donc falloir la construire de manière synthétique à partir des quadruplets dont nous disposons et de manière à imiter le comportement d'un automate.

Pour cela, il faut bien comprendre le comportement d'un automate :

1. Au démarrage les variables sont initialisées avec leur valeur d'initialisation ou 0.

2. Au démarrage, le code d'initialisation est exécuté.

3. Au démarrage, l'automate est dans son premier état tel que trouvé dans les sources.

4. Quand on rentre dans un état, le code de l'état est exécuté (y compris au démarrage de l'automate).

5. Un automate ne s'arrête jamais à moins quel'instruction `stop` ne soit appelée.

6. Pour changer d'état, l'instruction `goto` est utilisée.

7. Dans un état, l'automate teste toutes les clauses `when` sans arrêt, dans l'ordre où elles
apparaissent dans les sources.

8. Dès qu'une clause `when` devient vraie, le code de la clause `when` est exécuté.

Il existe plusieurs implantations possibles mais je propose la suivante :

	1	LABEL main
		<initialisation des variables>
		<code d'initialisation de l'automate>
		<POUR chaque état dans l'ordre de sources>
	5		LABEL <étiquette état>
			<code d'entrée dans l'état>
			LABEL <test clauses when>
			<POUR chaque clause when>
				<tester la clause>
	10			<si la clause est fausse GOTO fin clause>
					<code de la clause>
				LABEL <fin clause>
			GOTO <test clauses when>
		LABEL stop
	15	RETURN


Ce modèle est un mélange de quadruplets et de pseudoinstructions entre `<` ... `>`. On adoptera les traductions suivantes :

* 16. `STOP` -- on branchera sur l'étiquette _stop_ (`fonction AutoDecl::stopLabel()`).
* 17. `GOTO` _s_ -- on branchera sur l'étiquette `<étiquette état>` (fonction `State::label()`).

> En bref, un changement d'état sera réalisé par un branchement vers le code implémentant l'état.
Un état est un morceau de code exécuté dès qu'il est activié et un morceau de code avec une
boucle qui teste les clauses `when`. Une clause `when` est un test de signal et si le signal est activé,
le morceau de code correspondant est exécuté.

Regardons si de manière formelle, notre implantation satisfait la description du comportement de notre automate :

* (1) est implanté par la ligne 2.
* (2) est implanté par la ligne 3.
* (3) est supporté par l'ordre de la boucle POUR (ligne 4).
* (4) est supporté par la génération de l'étiquette (ligne 5) et du code d'entrée en suivant
(ligne 6) et par l'implantation du GOTO (ligne 17).
* (5) est supporté par l'implantation du STOP (16) et l'étiquette de stop (lignes 13, 14).
* (6) supporté par (17) et le placement des étiquettes d'état (ligne 5-6).
* (7) supporté par l'étiquette de boucle (ligne 7), la génération des clauses when (lignes 8-11)
et l'instruction de rebouclage (ligne 12).
* (8) supporté par le code des clauses (lignes 9-11).

Appliqué sur notre exemple pushdown, on doit obtenir un code qui ressemble à ça :

	LABEL main
		< GPIOD_MODER [2* GREEN_LED +1 .. 2* GREEN_LED ] = GPIO_MODER_OUT >
		< GPIOA_MODER [2* USER_BUT +1 .. 2* USER_BUT ] = GPIO_MODER_IN >
		< GPIOA_OTYPER [2* USER_BUT +1 .. 2* USER_BUT ] = GPIO_PUPDR_PD >

	LABEL Lup
		< GPIOD_BSRR [ GREEN_LED + 16] = 1>
	LABEL Lup-loop
		< tester signal BUTT.ON >
		<si faux GOTO Lup-next0>
			GOTO Ldown
	LABEL Lup-next0
		GOTO Lup-loop

	LABEL Ldown
		< GPIOD_BSRR [ GREEN_LED ] = 1 >
	LABEL Ldown-loop
		< tester signal ! BUTTON >
		<si geux GOTO Ldown-next0 >
			GOTO Lup
	LABEL Ldown-next0
		GOTO Lup-loop

	LABEL Lstop :
		RETURN

Les textes en `<` ... `>` ne sont pas des quadruplets et doivent donner lieu à une traduction.

Notre modèle semble bien correspondre au comportement attendu des automates : on peut
démarrer la traduction.

> Notez que la traduction des automates, états et clauses `when` vous sont fournies. Vous aurez juste à implanter la traduction pour les ASTs `Statement`, `Expression` et `Condition`.


## Mise en oeuve

Le problème majeur, quand on fait un traducteur, est de pouvoir tester les traductions au fur et à mesure ou petit pas par petit pas. L'idée est alors de faire un minimum viable de traductions, de trouver un (des) exemple(s) minimum(s) couvert(s) par la traducteur et de tester sur ce(s) exemple(s).

Ensuite, il suffira de :

1. faire la traduction d'un AST,
2. écrire un exemple (en IOML) impliquant cet AST (et les AST traduits jusque là)
3. et de faire le test.

On procèdera ainsi jusqu'à avoir fait la traduction complète des AST.

> Comme d'habitude, on évitera de tout écrire en une fois et de peiner ensuite à (a) compiler et (b) tester. Il est donc conseillé de compiler très souvent.

Pour débuter, on utilisera l'exemple minim suivant (dans test `zero.io`) :

	auto A
		state S :

On obtient alors un magnifique automate **A** avec un seul état **S** dans lequel on va rester pour toujours.

Pour le compiler et observer les quadruplets générés, on pourra utiliser la commande :
	`$ ./ ioc -print-quads test/zero.io`

Et obtiendra :

	L2
	L3
		goto L3
	L1
		return

Maintenant nous allons pouvoir commencer à réaliser la traduction à proprement parler. Chaque étape qui suit propose une petite extension et un fichier de test pour valider la traduction.

> Les fonctions à compléter se trouvent dans le fichier `gen.cpp`.

> ATTENTION : pour simplifier les opérations suivants la traduction, on va adopter une approche **SSA** (_Static Single Assignment_) allégée. C'est à dire qu'à l'exception des registres qui représentent des variables, un registre ne peut être affecté puis utilisé qu'une seule fois. Cela alloue certes plus de registres mais on verra que cela ne pose pas de problèmes par la suite. Notre implantation **SSA* est allégée car elle ne compote pas de d'opération [phi](https://en.wikipedia.org/wiki/Static_single-assignment_form). Les AST mènent assez naturellement vers une forme **SSA**.


**Affectation à une variable** (`assign.io`) Le pas que nous allons réaliser maintenant est un peu complexe mais va nous permettre ensuite d'implanter toutes les expressions. Nous allons supporter l'affectation dans une variable d'une constante :

	SET (i , CST n )

Donc il va falloir :

1. Implanter la traduction des constantes dans les expressions (fonction `ConstExpression::gen()`).

2. Implanter la traduction de l'affectation dans les instructions (fonction `SetStatement::gen()`).

Ensuite, on vérifiera si le code produit est bien ce qu'on attend.


**Traduction de la séquence** (`seq.io`) Cela va nous permettre de traduire des programmes constitués de plusieurs instructions qui se suivent. Traduire la séquence, _SEQ (s1 , s2 )_, est assez facile à réaliser dans `SeqStatement::gen()`.


**Traduction des opérations unaires ou binaires**
(`op.io`) Il s'agit ici d'implanter les opérations unaires (_NEG_, _INV_) et binaires (_ADD_, _SUB_, _MUL_, _DIV_, _MOD_, _BIT_AND_, _BIT_OR_, _XOR_, _SHL_, _SHR_, _SHA_, _ROL_, _ROR_). On notera qu'il existe un quadruplet pour chacune de ces opérations (fonctions `UnopExpression::gen()` et `BinopExpressio::gen()`).


**Traduction de l'accès mémoire** (`mem.io`) Maintenant nous allons nous intéresser à l'accès mémoire (classe `MemExpression`). Seuls les 3 symboles suivants sont supportés dans une expression :

* `Declaration::CST` -- accès à une constante nommée.
* `Declaration::VAR` -- variable. On se souviendra que les registres associés aux variables sont
accessibles avec la fonction `QuadProgram::regFor()`.
* `Declaration::VAR` -- accès à un registre d'entrée-sortie en mémoire. Il faut réaliser une lecture mémoire
(`Quad::load`) à l'adresse du registre.


**Traduction de la sélection** (`if.io`) La sélection _if_ de IOML est très proche de la sélection
des autres langages dont le C. Pour simplier l'implantation, nous allons d'abord seulement
considérer le cas où on a affaire avec une condition étant une comparaison simple.

1. Dans la fonction `CompCond::gen()`, réalisez la traduction de la comparaison. On appliquera la même
approche que pour le langage C.

2. Dans la fonction `IfStatement::gen()`, réalisez la traduction de _if_ en utilisant bien la traduction de la condition définie précédemment.


**Traduction des autres conditions** (`short.io`) En utilisant, l'évaluation en circuit court vue
en cours, réalisez la traduction des autres conditions `NotCond::gen()`, `AndCond::gen()` et `OrCond::gen()`.


**Traduction de champ de bit** (`bits.io`) Cet AST _BITFIELD(e, u, l)_, permet d'obtenir
la valeur des bits _l_ (low) à _u_ (up) bits dans le résultat de l'expression _e_. Cet opérateur n'existant pas dans nos quadruplets, on pourra la calculer, par exemple, avec l'expression C suivante :
	_(e >> l) & ((1 << (u -l + 1)) - 1)_u

1. Dans un premier temps, on pourra appeler le sous-programme `L10000` (constante `field_get_call`) qui prend comme paramètre _e_ dans R0, _u_ dans R1 et _l_ dans R2. Le résultat est dans R0.

> NOTE : Ri a pour code de registre (Quad::reg_t), i. Les 16 premiers registres virtuels sont réservés pour représenter les registres réels.

2. On peut distinguer un cas qui va beaucoup simplifier le code :
	si _u_ = _l_ alors la formule devient _(e >> l) & 1_


**Traduction de l'affectation de champ de bits** (`setbits.io`) Cet AST, _SET_BITFIELD(i, u, l, e)_, est
le pendant de _BITFIELD_ décrit précédemment. Il modifie dans la mémoire _i_ (variable ou registre)
les bits _u_ à _l_ avec la valeur produite par l'expression _e_. Bien sûr, seuls les _u − l + 1__ bits de _e_ sont affectés.

1. Dans un premier temps, on pourra appeler le sous-programme `L10001` (constante `field_set_call`) qui prend comme paramètre _e_ dans R0, _u_ dans R1, _l_ dans R2 et _e_ dans R3. Le résultat est dans R0.

2. On peut distinguer un cas qui va beaucoup simplifier le code :
	si _u_ = _l_ et que _e_ est une constante alors
		si _e_ = 1 alors la valeur affectée est _i | (1 << l)_
		si _e_ = 0 alors la valeur affectée est _i & ~(1 << l)_
	Les choses peuvent encore plus simplifiées si _l_ est une constante.


> Cela conclura la compilation en quadruplets de IOML. On va pouvoir passer à la sélection des instructions.
