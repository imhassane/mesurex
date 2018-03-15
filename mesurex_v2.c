/*===============================================================
// Nom du fichier : mesurex
// Auteur : HASSANE & TANGUY
// Date de création : 1 Mars 2018
// Version : V4
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Description :
// Le véhicule, il roule sur un parcours sinueux et calcule la
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
		setMultipleMotors(speed, motorA, motorB);
}

/* Fonction qui va calculer la distance parcourue en fonction de l'angle */
float calcul_distance_parcourue(float angle){
	return (-angle / 360) * 18;
}

/* Fonction qui va ecrire les valeurs mesurees dans un fichier */
void write(float mesure){
	long fileHandle;
	fileHandle = fileOpenWrite("mesures.txt");

	char valeur[10];
	fileWriteData(fileHandle,"Mesure : ",strlen("Mesure : "));
	sprintf(valeur,"%f", mesure);
	fileWriteData(fileHandle,valeur,strlen(valeur));

	fileClose(fileHandle);

}

task main()
{
	//
	bool arret = true, avant = false, tourne = false;
	// La puissance du moteur;
	int puissance = 40;

	// La distance parcourue et l'angle éffectué.
	float distance = 0.0, angle = 0.0;
	float distance_2 = 0.0, angle_2 = 0.0;

	resetMotorEncoder(motorA);
	resetMotorEncoder(motorB);

	// Initialisation de l'angular sensor.
  tHTANG angleSensor;
  initSensor(&angleSensor, S2);
  resetAccmulatedAngle(&angleSensor);

	while(SensorValue[S1] == 0){

			if(!arret){
				rouler_ligne_droite(puissance);
			}

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

			}else if(SensorValue[S3] == 1){
				setMotorSpeed(puissance / 5, motorB);
				sleep(200);

			}else if(SensorValue[S4] == 1){
				setMotorSpeed(puissance / 5, motorA);
				sleep(200);
			}

			// On calcule la distance parcourue.
			distance += calcul_distance_parcourue(angle);
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
			distance = -(distance + 2);
			distance_2 = -(distance_2 + 2);

			displayTextLine(5, "Distance parcourue: %g cm", distance);
			displayTextLine(8, "Distance parcourue: %g cm", -calcul_distance_parcourue(angleSensor.accumlatedAngle));

			write(distance);

			sleep(10000);
}
