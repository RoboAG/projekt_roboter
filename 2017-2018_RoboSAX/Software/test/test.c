//Dies ist nur eine Testdatei mit keinerlei Bedeutung
//Ausschnitte bitte nur mit folgenden Kommentar verwenden

/*****************************************************************
* RobotikAG des Johannes-Kepler-Gymnasiums Chemnitz              *
*                                                                *
* Autoren:                                                       *
*     Janek Neugebauer                Leonhard Nagel             *
*     Felix Krüger                    Christoph Männel           *
*                                                                *
* Stand:                                                         *
*     Juni 2018                                                  *
*                                                                *
* Ausschnitte dieses Programms stammen aus dem Programm          *
* zum Endwettbewerb des RoboSAX 2018.                            *
*                                                                *
* Dokumentation und Software unter "2017-2018_RoboSAX" auf       *
*   https://github.com/RoboAG/projekt_roboter                    *
*****************************************************************/

int main(){
    set_motors(0,0);
    delay(1500);
    set_motors(50,-50);
    delay(370);
    set_motors(0,0);
}
