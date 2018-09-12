/* Control de relés y motores paso a paso con interfáz MIDI.
   Autor: Javier Vargas. El Hormiguero.
   https://creativecommons.org/licenses/by/4.0/
*/

/////////AJUSTES DEL PROGRAMA///////////

//CONFIGURACION
#define Nrele 8 //Numero de reles por conexion
#define Nmotor 8 //Numero de motores (Funcinan 4 interrupciones simultáneas)
#define ReleON LOW //Nivel de tensión para el activado del rele

//Notas de entrada MIDI
const int NotaRele1[Nrele] = {0, 1, 2, 3, 4, 5, 6, 7}; //Channel 0
const int NotaRele2[Nrele] = {8, 9, 10, 11, 12, 13, 14, 15}; //Channel 0
//Motores --> //Channel 1, 2, 3, 4, 5, 6, 7, 8

int Tmotors[109]; //Periodos del motor que corresponde a cada nota
const float tonos[12] = {253.1, 268.1, 284.1, 301.0, 318.9, 337.8, 358.0, 379.2, 401.8, 425.7, 451.0, 477.8}; //B7 -> C7 (x100) Octava más aguda

//PINES
const int pinRele1[Nrele] = {0, 1, 2, 3, 4, 5, 6, 7};
const int pinRele2[Nrele] = {25, 26, 27, 28, 29, 30, 31, 32};
const int pinMotorSTEP[Nmotor] = {23, 22, 21, 20, 19, 18, 17, 16};
const int pinMotorENABLE[Nmotor] = {14, 39, 38, 37, 36, 35, 34, 33};
#define ledPin 13

IntervalTimer t[8];

//////////////////////////////
////////////SETUP/////////////
//////////////////////////////

void setup() {

  //PinMode
  pinMode(ledPin, OUTPUT);
  for (int i = 0; i < Nrele; i++) {
    pinMode(pinRele1[i], OUTPUT);
    pinMode(pinRele2[i], OUTPUT);
    //Inicio apagado
    digitalWrite(pinRele1[i], !ReleON);
    digitalWrite(pinRele2[i], !ReleON);
  }
  for (int i = 0; i < Nmotor; i++) {
    pinMode(pinMotorSTEP[i], OUTPUT);
    pinMode(pinMotorENABLE[i], OUTPUT);
    //Inicio apagado
    digitalWrite(pinMotorENABLE[i], HIGH);
  }

  //Inicio MIDI
  usbMIDI.setHandleNoteOn(myNoteOn);
  usbMIDI.setHandleNoteOff(myNoteOff);
  usbMIDI.setHandleControlChange(myControlChange);

  //Asignacion de notas
  for (int Oct = 0; Oct <= 6; Oct++) { //7 Octavas --> C1 - B7
    for (int N = 1; N <= 12; N++) { //12 Notas por octava
      int nota = 108 - (N  + Oct * 12);
      int periodo = tonos[N - 1] * pow(2, Oct);
      Tmotors[nota] = periodo;
    }
  }

  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
}

//////////////////////////////
/////////////LOOP/////////////
//////////////////////////////

void loop() {
  usbMIDI.read();
}

//////////////////////////////
//////////////////////////////
//////////////////////////////

//MIDI
void myNoteOn(byte channel, byte note, byte velocity) {
  // When using MIDIx4 or MIDIx16, usbMIDI.getCable() can be used
  // to read which of the virtual MIDI cables received this message.

  digitalWriteFast(ledPin, HIGH);

  //Notas de rele
  if (channel == 10) {
    for (int i = 0; i < Nrele; i++) {
      if (note == NotaRele1[i]) {
        digitalWriteFast(pinRele1[i], ReleON);
        break;
      }
      else if (note == NotaRele2[i]) {
        digitalWriteFast(pinRele2[i], ReleON);
        break;
      }
    }
  }

  //Notas de motores
  if (channel < 10) {
    SetMotor(channel, Tmotors[note]);
  }

}

void myNoteOff(byte channel, byte note, byte velocity) {

  digitalWriteFast(ledPin, LOW);

  //Notas de rele
  if (channel == 10) {
    for (int i = 0; i < Nrele; i++) {
      if (note == NotaRele1[i]) {
        digitalWriteFast(pinRele1[i], !ReleON);
        break;
      }
      else if (note == NotaRele2[i]) {
        digitalWriteFast(pinRele2[i], !ReleON);
        break;
      }
    }
  }

  //Notas de motores
  if (channel < 10) {
    SetMotor(channel, -1); //Apagado
  }

}

void myControlChange(byte channel, byte control, byte value) {

}

//CONTROL DEL MOTOR
void SetMotor(byte motor, int T) {
  //Encender motor (activa la interrupcion)
  if (T > 0) {
    digitalWriteFast(pinMotorENABLE[motor - 1], LOW);
    if (motor == 1) t[0].begin(step0, T);
    else if (motor == 2) t[1].begin(step1, T);
    else if (motor == 3) t[2].begin(step2, T);
    else if (motor == 4) t[3].begin(step3, T);
    else if (motor == 5) t[4].begin(step4, T);
    else if (motor == 6) t[5].begin(step5, T);
    else if (motor == 7) t[6].begin(step6, T);
    else if (motor == 8) t[7].begin(step7, T);
  }
  //Apagar motor, desactiva la interrupcion
  else {
    digitalWriteFast(pinMotorENABLE[motor - 1], HIGH);
    t[motor - 1].end();
  }

}

//STEPS
void step0() {
  stepPulse(0);
}
void step1() {
  stepPulse(1);
}
void step2() {
  stepPulse(2);
}
void step3() {
  stepPulse(3);
}
void step4() {
  stepPulse(4);
}
void step5() {
  stepPulse(5);
}
void step6() {
  stepPulse(6);
}
void step7() {
  stepPulse(7);
}


void stepPulse(int out) {
  static volatile boolean e[Nmotor];
  if (e[out]) digitalWriteFast(pinMotorSTEP[out], LOW), e[out] = 0;
  else digitalWriteFast(pinMotorSTEP[out], HIGH), e[out] = 1;

}


