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
#include "hitechnic-angle.h"

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
	int puissance = 40;

	// La distance parcourue et l'angle éffectué.
	float distance = 0.0, distance_2 = 0.0, angle = 0.0, angle_2;

	resetMotorEncoder(motorA);
	resetMotorEncoder(motorB);

	// Initialisation de l'angular sensor.
  tHTANG angleSensor;
  initSensor(&angleSensor, S2);
  resetAccmulatedAngle(&angleSensor);

  //displayTextLine(8, "Distance parcourue: %g cm", angleSensor.accumlatedAngle);
  ////sleep(10000);

	while(SensorValue[S1] == 0){

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
				puissance = abso(puissance);
				arret = false;
				avant = false;

			}

			// On calcule la distance parcourue.
			distance += calcul_distance_parcourue(angle);
			distance_2 += calcul_distance_parcourue(angle_2);
			readSensor(&angleSensor);
		}

		// On arrete le mouvement du véhicule lorsqu'on clique
			// sur le bouton entré.
			stopMultipleMotors(motorA, motorB);

			// On récupère l'angle éffectué.
			angle = getMotorEncoder(motorA);
			angle_2 = getMotorEncoder(motorB);

			// On enregistre la distance.
			distance += calcul_distance_parcourue(angle);
			distance_2 += calcul_distance_parcourue(angle_2);

			// On affiche la distance parcourue.
			distance = -(distance + 5);
			distance_2 = -(distance_2 + 5);
			//float angleValue = angleSensor.accumlatedAngle;

			displayTextLine(5, "Distance parcourue: %g cm", distance);
			displayTextLine(8, "Distance parcourue: %g cm", -calcul_distance_parcourue(angleSensor.accumlatedAngle));

			sleep(5000);
}

// TODO
// Ecrire les distances parcourues dans un fichier.
