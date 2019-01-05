This project was developed by Souhail Elaissaoui for the course " Programming in C++" at Ecole Polytechnique Fédérale de Lausanne.
Thanks to Prof. Jamila Sam for the instructions and supervision.
The project aims at modeling the developement of population of bees in an environment with various objects,
considering the interaction between bees, flowers, hives etc...
This program and its graphical interface was developed in C++ and constituted an introduction to object oriented programming.
The compiling is done with "scons" as explained in the below paragraph (in french)



	A. Presentation du programme:

Notre programme permet de simuler l'evolution d'abeilles et de fleurs dans un monde.

Le monde est torique, il est constitue de 3 elements : Roche, Herbe et Eau. Les zones proches de l'eau sont plus humides.

Notre monde contient des fleurs a pollen : elles sont generees aleatoirement et evoluent en fonction du niveau d'humidite de leur position.

Les abeilles partagent une ruche, et collaborent pour y stocker du nectar.
Il existe 2 types d'abeilles :
	-Les butineuses qui recoltent du pollen et le transferent a la ruche.
	-Les eclaireuses qui localisent les fleurs et communiquent leur position aux butineuses.
Ces abeilles ne peuvent survoler la roche. 
	
Nous pouvons suivre l'evolution de notre simulation en mode Debug : 
Affichage de l'etat de chaque abeille et de son energie + cercle bleu si elle se deplace avec cible, ou noir si elle se deplace aleatoirement. 
Affichage du nectar de chaque ruche.
Affichage du niveau d'humidite de la case survolee par le curseur.

Il est possible de creer une ruche a une position choisie, des abeilles y apparaitront.
Cependant elle ne peut pas apparaitre sur de la roche ou de l'eau, ni sur une zone occupee par une fleur ou une autre ruche.
Par defaut, le terrain est charge a partir d'un fichier.
Il est egalement possible de generer un nouveau terrain aleatoire, de sauvegarder le terrain actuel,
ou de charger le terrain a partir d'un terrain sauvegarde au prealable. Et ce pendant la simulation. 
Noter que toute manipulation de terrain pendant la simulation engendre la suppression de la faune et la flore.
Notre programme est configurable a l'aide d'un fichier de configuration appX.json

Extension : 

Il est possible de personnaliser le monde en mode creation : Nous pouvons ainsi mettre de la roche, de l'herbe et de l'eau comme cela nous convient.
Les abeilles rivales issues de ruches differentes s'agressent lorsqu'elles butinent la meme fleur. Celle avec le plus grand niveau d'energie gagne le duel.
Il est aussi possible de creer un ours (winnie the pooh) qui mange le miel produit par les ruches.

	B. Compilation du programme:

Pour compiler le programme, il est necessaire de lancer la commande:
			"scons application-run"
depuis le repertoire contenant sconscript.

Le fichier de configuration par defaut est app.json. 
Pour en charger un autre, la commande doit changer :
			"scons application-run --cfg=appX.json"
Nous avons 4 fichiers de configuration prets : app (plus grand monde), app2 (moins grand), app3 (moyen) ou app4 (petit).

	C. Utilisation du programme :

Commande du monde :

Reset : appuyer sur "r" pour generer un nouveau monde aleatoirement.
Chargement : appuyer sur "i" pour charger le monde a partir d'un fichier.
Sauvegarde : appuyer sur "o" pour sauvegarder le monde actuel.
Affichage des niveaux d'humidite: appuyer sur  'w'.
Activation du mode Debug: appuyer sur 'd'.
		
Commande faune et flore:
Ajout de fleur : appuyer sur "f" pour ajouter une fleur.
Ajout de ruche : appuyer sur "h" pour ajouter une ruche.

Extension : 
Appuyer sur "c" pour entrer en mode creation de monde. Un monde rocheux est genere.
La zone de creation apparait, appuyer encore sur "c" pour y placer de l'eau.
Pour passer a l'herbe, appuyer sur "Entree". 
Une fois terminé, appuyer encore sur entree pour commencer la simulation.
Les touches "+" et "-" permettent de modifier le rayon de creation.
Appuyer sur 'k' pour generer un ours.

