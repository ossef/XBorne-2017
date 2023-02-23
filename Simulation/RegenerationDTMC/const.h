/*constantes a modifier pour chaque modele*/

#define load 0.8
#define burst 5
#define l2  0.2


#define l3  (load-l2)/burst		
#define l1  1.0 - l2 -l3		

/* Ne pas TOUCHER  pour le moment */ 

#define NEt               1   /*nombre de composantes du vecteur d'etat*/
#define NbEvtsPossibles   3  /*nombre de vecteurs d'arrivees possibles*/
#define Compact 1 /* type of output */ 
