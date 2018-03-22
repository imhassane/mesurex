/*===============================================================
// Nom du fichier : mesurex
// Auteur : HASSANE & TANGUY
// Date de cr�ation : 1 Mars 2018
// Version : V4
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Description :
// Le v�hicule, il roule en ligne droite.
// Il tourne � droite ou � gauche suivant la direction qui lui est
// indiqu�e et calcule la distance qu'il a parcourue lorsque le
// capteur de contact est appuy�.
// -----------------------------------------------------------------
// A noter :
// - les moteurs sont branch�s sur les entr�es A et B
// - les capteurs pour diriger le v�hicule sont sur les sorties 3 et 4
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
void write(long *fileHandle, float mesure){

	char valeur[10];
	fileWriteData(*fileHandle,"Mesure : ",strlen("Mesure : "));
	sprintf(valeur,"%f\n", mesure);
	fileWriteData(*fileHandle,valeur,strlen(valeur));

}

task main()
{
	bool arret = true, avant = false, tourne = false;

	// Variable qui va permettre de finir la session actuelle.
	bool finSession = false;

	// La puissance du moteur;
	int puissance = 40;

	// La distance parcourue et l'angle �ffectu�.
	float distance = 0.0, angle = 0.0;
	float distance_2 = 0.0, angle_2 = 0.0;

	// On r�initialise les angles des moteurs au d�but.
	resetMotorEncoder(motorA);
	resetMotorEncoder(motorB);

	// Initialisation de l'angular sensor.
  	tHTANG angleSensor;
  	initSensor(&angleSensor, S2);
  	resetAccmulatedAngle(&angleSensor);

  // On ouvre le fichier dans lequel on met les mesures.
	long fileHandle;
	fileHandle = fileOpenWrite("mesures.txt");

	while(finSession == false){

		// Tant que le capteur d'arret n'est pas cliqu�, on roule.
		while(SensorValue[S1] == 0){

				// On roule en ligne droite.
				if(arret == false)
					rouler_ligne_droite(puissance);

				// Si le bouton droit est cliqu�.
				if(getButtonPress(buttonUp) == 1){

					// Si on n'allait pas en avant on change la puissance.
					if(!avant){
						puissance = -puissance;
						avant = true;
					}

				}else if(getButtonPress(buttonEnter) == 1){
					puissance = abso(puissance);
					avant = false;
					arret = false;

				}else if(getButtonPress(buttonDown) == 1){
					// On r�cule si le bouton bas est cliqu�.
					puissance = -puissance;
					avant = false;
				}
				else if(getButtonPress(buttonLeft)){
					// Si le bouton gauche est cliqu�, on sort de la session.
					// lors du prochain arr�t.
					finSession = true;

				}else if(SensorValue[S3] == 1){
					// On diminue la puissance de la roue de gauche
					// pour pouvoir tourner � gauche.
					setMotorSpeed(motorA, 10);
					sleep(200);

				}else if(SensorValue[S4] == 1){
					// On diminue la puissance de la route droite
					// pour pouvoir tourner � droite.
					setMotorSpeed(motorB, 10);
					sleep(200);
				}

				// On calcule la distance parcourue.
				distance += calcul_distance_parcourue(angle);

				// On lit la valeur de l'angular sensor.
				readSensor(&angleSensor);
			}
				arret = true;
				// On arrete le mouvement du v�hicule lorsqu'on clique
				// sur le capteur est action�.
				stopMultipleMotors(motorA, motorB);

				// On r�cup�re l'angle �ffectu� par les deux roues.
				angle = getMotorEncoder(motorA);
				angle_2 = getMotorEncoder(motorB);

				// On enregistre la distance �ffectu�e par les deux roues.
				distance += calcul_distance_parcourue(angle);
				distance_2 += calcul_distance_parcourue(angle_2);

				distance = -(distance + 2);
				distance_2 = -(distance_2 + 2);

				// On affiche la distance.
				distance = (distance + distance_2) / 2;

				displayTextLine(3, "La distance parcourue a une incertitude de 1%%");
				displayTextLine(5, "Distance parcourue: %g cm", distance);
				displayTextLine(8, "Distance parcourue: %g cm", -calcul_distance_parcourue(angleSensor.accumlatedAngle));

				// On la met dans un fichier.
				write(&fileHandle, distance);

				distance = 0;
				distance_2 = 0;
		}

			// On ferme le fichier.
			fileClose(fileHandle);

			sleep(10000);
}
