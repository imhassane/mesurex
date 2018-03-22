/*===============================================================
// Nom du fichier : mesurex
// Auteur : HASSANE & TANGUY
// Date de création : 1 Mars 2018
// Version : V4
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Description :
// Le véhicule, il roule en ligne droite.
// Il tourne à droite ou à gauche suivant la direction qui lui est
// indiquée et calcule la distance qu'il a parcourue lorsque le
// capteur de contact est appuyé.
// -----------------------------------------------------------------
// A noter :
// - les moteurs sont branchés sur les entrées A et B
// - les capteurs pour diriger le véhicule sont sur les sorties 3 et 4
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
	bool arret = true, avant = false, tourne = false;
	// La puissance du moteur;
	int puissance = 40;

	// La distance parcourue et l'angle éffectué.
	float distance = 0.0, angle = 0.0;
	float distance_2 = 0.0, angle_2 = 0.0;
	
	// On réinitialise les angles des moteurs au début.
	resetMotorEncoder(motorA);
	resetMotorEncoder(motorB);

	// Initialisation de l'angular sensor.
  	tHTANG angleSensor;
  	initSensor(&angleSensor, S2);
  	resetAccmulatedAngle(&angleSensor);

	while(SensorValue[S1] == 0){
			// On roule en ligne droite.
			if(arret == false)
				rouler_ligne_droite(puissance);

			// Si le bouton droit est cliqué.
			if(getButtonPress(buttonUp)){

				// Si on n'allait pas en avant on change la puissance.
				if(!avant){
					puissance = -puissance;
					avant = true;
				}

			}else if(getButtonPress(buttonEnter)){
				puissance = abso(puissance);
				avant = false;
				arret = false;

			}else if(SensorValue[S3] == 1){
				// On diminue la puissance de la roue de gauche
				// pour pouvoir tourner à gauche.
				setMotorSpeed(puissance / 5, motorB);
				sleep(200);

			}else if(SensorValue[S4] == 1){
				// On diminue la puissance de la route droite
				// pour pouvoir tourner à droite.
				setMotorSpeed(puissance / 5, motorA);
				sleep(200);
			}

			// On calcule la distance parcourue.
			distance += calcul_distance_parcourue(angle);
			// On lit la valeur de l'angular sensor.
			readSensor(&angleSensor);
		}

			// On arrete le mouvement du véhicule lorsqu'on clique
			// sur le capteur est actioné.
			stopMultipleMotors(motorA, motorB);

			// On récupère l'angle éffectué par les deux roues.
			angle = getMotorEncoder(motorA);
			angle_2 = getMotorEncoder(motorB);

			// On enregistre la distance éffectuée par les deux roues.
			distance += calcul_distance_parcourue(angle);
			distance_2 += calcul_distance_parcourue(angle_2);

			distance = -(distance + 2);
			distance_2 = -(distance_2 + 2);
	
			// On affiche la distance.
			displayTextLine(5, "Distance parcourue: %g cm", distance);
			displayTextLine(8, "Distance parcourue: %g cm", -calcul_distance_parcourue(angleSensor.accumlatedAngle));
			
			// On la met dans un fichier.
			write(distance);

			sleep(10000);
}
