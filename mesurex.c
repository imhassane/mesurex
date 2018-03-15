/*===============================================================
// Nom du fichier : mesurex
// Auteur : HASSANE & TANGUY
// Date de création : 1 Mars 2018
// Version : V1
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Description :
// Le véhicule, il roule suivant une ligne droite et calcule la
// parcourue.
// -----------------------------------------------------------------
// A noter :
// - les moteurs sont branchés sur les entrées A et B
//===============================================================*/

/* Fonction qui va permettre qui va renvoyer la valeur absolue d'une valeur */
int abso(int n){
	if(n < 0)
		return -n;
	return n;
}

/* Fonction qui va permettre au moteur de rouler */
void rouler_ligne_droite(int speed){
		setMultipleMotors(speed, motorA, motorB, motorC);
}

/* Fonction qui va calculer la distance parcourue en fonction de l'angle */
float calcul_distance_parcourue(float angle){
	return (-angle / 360) * 18;
}

task main()
{
	//
	bool arret = true, avant = false;
	// La puissance du moteur;
	int puissance = 90;

	// La distance parcourue et l'angle éffectué.
	float distance = 0.0, angle = 0.0;

	resetMotorEncoder(motorA);

	while(true){

		if(!arret)
			rouler_ligne_droite(puissance);

		// Si le bouton droit est cliqué.
		if(getButtonPress(buttonUp)){

			// Si on n'allait pas en avant on change la puissance.
			if(!avant){
				puissance = -puissance;
				avant = true;
			}
			// On met l'arret à false.
			arret = false;

		}else if(getButtonPress(buttonEnter)){
			// On arrete le mouvement du véhicule lorsqu'on clique
			// sur le bouton entré.
			stopMultipleMotors(motorA, motorB);

			// On récupère l'angle éffectué.
			angle = getMotorEncoder(motorA);

			// On enregistre la distance.
			distance += calcul_distance_parcourue(angle);

			// On affiche la distance parcourue.
			displayTextLine(5, "Distance parcourue: %g cm", distance);
			arret = true;

			distance = 0;
			// On réinitialise les angles.
			resetMotorEncoder(motorA);


		}else if(getButtonPress(buttonDown)){
			puissance = abso(puissance);
			arret = false;
			avant = false;

		}

		// On calcule la distance parcourue.
		distance += calcul_distance_parcourue(angle);
		sleep(200);

	}

}

// TODO
// Ecrire les distances parcourues dans un fichier.
