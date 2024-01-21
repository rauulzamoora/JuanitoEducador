// Juanito Educador


  //Sensores y actuadores : Aquí se definen los pines para los sensores infrarrojos, el sensor ultrasónico y los motores del robot.

// Infrarrojos
const int pinSensorDerecha = 2;
const int pinSensorIzquierda = 3;

// Ultrasonico
const int triggerPin = 8;
const int echoPin = 9;

// Motores
const int motorDerechaA = 4;
const int motorDerechaB = 5;
const int motorIzquierdaA = 6;
const int motorIzquierdaB = 7;
//const int enableA = 10; // Controlar las RPM del motor IZQ
//const int enableB = 11; // Controlar las RPM del motor DER


 /* Constantes y Variables: Se establecen algunas constantes que se utilizan en el programa, como la distancia umbral 
 para cambiar entre los modos y algunas variables para almacenar lecturas de sensores y cálculos.*/

const int distanciaUmbral = 60;
const int vSonido = 58;
const int tiempoGiro = 800;

int valorSensorDerecha;
int valorSensorIzquierda;
float distancia;

int enModoBuscar = 1;  // Variable para controlar el modo del robot (que interrumpa una función para hacer otra)


 /* Configuración inicial: Aquí se configuran los pines como entradas o salidas según sea necesario. 
 También se establece la velocidad de comunicación para el puerto serial y se espera 5 segundos para permitir la conexión del puerto serial.*/

void setup() {
  Serial.begin(9600);
  pinMode(pinSensorDerecha, INPUT);
  pinMode(pinSensorIzquierda, INPUT);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(motorDerechaA, OUTPUT);
  pinMode(motorDerechaB, OUTPUT);
  pinMode(motorIzquierdaA, OUTPUT);
  pinMode(motorIzquierdaB, OUTPUT);
  //pinMode(enableA, OUTPUT);
  //pinMode(enableB, OUTPUT);
  delay(5000); //Tiempo en el que va a iniciar 
}

  /* FUnciones para el movimiento: Estas funciones controlan los motores para realizar diferentes movimientos, 
 como buscar, atacar, girar a la derecha, girar a la izquierda y retroceder. */

void buscar() {
 // analogWrite(enableA, 40);
 // analogWrite(enableB, 40);
  digitalWrite(motorDerechaA, HIGH);
  digitalWrite(motorDerechaB, LOW);
  digitalWrite(motorIzquierdaA, LOW);
  digitalWrite(motorIzquierdaB, HIGH);
}

void atacar() {
 // analogWrite(enableA, 240);
 // analogWrite(enableB, 240);
  digitalWrite(motorDerechaA, HIGH);
  digitalWrite(motorDerechaB, LOW);
  digitalWrite(motorIzquierdaA, HIGH);
  digitalWrite(motorIzquierdaB, LOW);
}

void girarDerecha() {
  //analogWrite(enableA, 40);
  //analogWrite(enableB, 40);
  digitalWrite(motorDerechaA, LOW);
  digitalWrite(motorDerechaB, HIGH);
  digitalWrite(motorIzquierdaA, HIGH);
  digitalWrite(motorIzquierdaB, LOW);
  delay(tiempoGiro);
}

void girarIzquierda() {
  //analogWrite(enableA, 40);
  //analogWrite(enableB, 40);
  digitalWrite(motorDerechaA, HIGH);
  digitalWrite(motorDerechaB, LOW);
  digitalWrite(motorIzquierdaA, LOW);
  digitalWrite(motorIzquierdaB, HIGH);
  delay(tiempoGiro);
}

void retroceder() {
  //analogWrite(enableA, 100);
  //analogWrite(enableB, 100);
  digitalWrite(motorDerechaA, LOW);
  digitalWrite(motorDerechaB, HIGH);
  digitalWrite(motorIzquierdaA, LOW);
  digitalWrite(motorIzquierdaB, HIGH);
}
 
 // Funcion para medir la distancia con el sensor ultrasónico

void medirDistancia() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  int duracion = pulseIn(echoPin, HIGH);
  distancia = duracion / vSonido;
}


 /* Logica principal:  El bucle principal del programa lee los valores de los sensores, mide la distancia, 
 muestra información en el puerto serial y controla el comportamiento del robot en función de las lecturas de los sensores. */

void loop() {
  // Leer valores de los infrarrojos
  valorSensorDerecha = digitalRead(pinSensorDerecha);
  valorSensorIzquierda = digitalRead(pinSensorIzquierda);

  // Valores del ultrasonico
  medirDistancia();

  // Monitor serial
  Serial.print(distancia);
  Serial.println("cm - ultrasonico");
  Serial.print(valorSensorDerecha);
  Serial.println(" - derecha");
  Serial.print(valorSensorIzquierda);
  Serial.println(" - izquierda");

  /* Lógica de control del robot mediante los sensores: La lógica de control de modo decide si el robot debería estar buscando o atacando. 
  También hay una lógica basada en los valores de los sensores infrarrojos para realizar acciones como girar a la derecha, girar a la izquierda o retroceder. */

  // Control de modo
  if (enModoBuscar == 1) {
    if (distancia > distanciaUmbral) {
      buscar();
    } else {
      enModoBuscar = 0;
      atacar();
    }
  } else {
    if (distancia <= distanciaUmbral) {
      enModoBuscar = 1;
      atacar();
    } else {
      buscar();
    }
  }

  // Lógica de los sensores
  if (valorSensorDerecha == 1 && valorSensorIzquierda == 0) {
    girarIzquierda();
  } else if (valorSensorIzquierda == 1 && valorSensorDerecha == 0) {
    girarDerecha();
  } else if (valorSensorIzquierda == 0 && valorSensorDerecha == 0) {
    retroceder();
  }
}
