/*--------------------------------------------------------*/
/*                                                        */
/*partie de Code specifique au modéle "Naissance et Mort" */
/*             Cas d'une file M/M/C/B                     */
/*--------------------------------------------------------*/

#define C        5       // Nombre de serveurs dans la file  
#define B        20      // Capacité de la file  (C <= B) sinon certains serveurs ne seront jamais utilisés 
#define Lambda   10      // Taux d'arrivée		
#define Mu       2  	 // Taux de service
#define DEBUG    0       // 1 pour la version debug + calcul de certaines rewards
                         // 0 pour la version non-debug