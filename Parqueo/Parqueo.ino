#include <Servo.h>
#include <LiquidCrystal.h>

// LCD: RS, E, D4, D5, D6, D7 → evitando 6–8
LiquidCrystal lcd(9, 10, 11, 12, A4, A5);

Servo Myservo;

// Pines de control
const int botonEntrada = 2;
const int sensores[4] = {A0, A1, A2, A3};
const int sentidoPin = 4;
const int pulsoPin   = 5;
const int q0 = 6, q1 = 7, q2 = 8; // Salidas Q del contador descendente
const int servoPin   = 3;

bool estadoAnteriorSensor[4] = {LOW, LOW, LOW, LOW};
bool estadoAnteriorBoton = HIGH;

int contadorActual = 4;
int contadorAnteriorLCD = -1;

void setup() {
  Serial.begin(9600);

  // Configuración de pines
  for (int i = 0; i < 4; i++) {
    pinMode(sensores[i], INPUT);
  }

  pinMode(q0, INPUT);
  pinMode(q1, INPUT);
  pinMode(q2, INPUT);
  pinMode(sentidoPin, OUTPUT);
  pinMode(pulsoPin, OUTPUT);
  pinMode(botonEntrada, INPUT);

  Myservo.attach(servoPin);
  Myservo.write(0); // Cerrar talanquera

  // Inicializar LCD
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Total: 4");

  delay(1000);
  resetearContadores();
}

void loop() {
  // Botón de entrada (ticket)
  bool estadoBoton = digitalRead(botonEntrada);
  if (estadoBoton == LOW && estadoAnteriorBoton == HIGH) {
    if (contadorActual > 0) {
      Myservo.write(90);
      delay(20000);
      Myservo.write(0);
      Serial.println("Talanquera abierta - vehículo entra");
    } else {
      Serial.println("Parqueo lleno");
    }
  }
  estadoAnteriorBoton = estadoBoton;

  // Monitoreo de sensores
  for (int i = 0; i < 4; i++) {
    bool lectura = digitalRead(sensores[i]);

    // Entrada de vehículo
    if (lectura == HIGH && estadoAnteriorSensor[i] == LOW && contadorActual > 0) {
      digitalWrite(sentidoPin, LOW);
      digitalWrite(pulsoPin, HIGH);
      delay(100);
      digitalWrite(pulsoPin, LOW);
      contadorActual--;
      Serial.print("Ocupado espacio ");
      Serial.print(i + 1);
      Serial.print(" → Libres: ");
      Serial.println(contadorActual);
    }

    // Salida de vehículo
    else if (lectura == LOW && estadoAnteriorSensor[i] == HIGH && contadorActual < 4) {
      digitalWrite(sentidoPin, HIGH);
      digitalWrite(pulsoPin, HIGH);
      delay(100);
      digitalWrite(pulsoPin, LOW);
      digitalWrite(sentidoPin, LOW);
      contadorActual++;
      Serial.print("Liberado espacio ");
      Serial.print(i + 1);
      Serial.print(" → Libres: ");
      Serial.println(contadorActual);
    }

    estadoAnteriorSensor[i] = lectura;
  }

  // Actualizar LCD solo si cambia el valor
  if (contadorActual != contadorAnteriorLCD) {
    lcd.setCursor(0, 1);
    lcd.print("Libres: ");
    lcd.print(contadorActual);
    lcd.print("   "); // Limpia posibles residuos
    contadorAnteriorLCD = contadorActual;
  }

  delay(100);
}

void resetearContadores() {
  Serial.println("Inicializando sistema...");

  // Paso 1: colocar el contador descendente en 4
  digitalWrite(sentidoPin, LOW);
  for (int i = 0; i < 11; i++) {
    digitalWrite(pulsoPin, HIGH);
    delay(50);
    digitalWrite(pulsoPin, LOW);
    delay(50);
  }

  // Paso 2: calcular entradas simuladas (4 - valor actual descendente)
  int b0 = digitalRead(q0);
  int b1 = digitalRead(q1);
  int b2 = digitalRead(q2);
  contadorActual = b0 + (b1 << 1) + (b2 << 2); // número de espacios libres
  int ocupados = 4 - contadorActual;

  // Paso 3: enviar la misma cantidad de pulsos al contador ascendente
  digitalWrite(sentidoPin, HIGH);
  for (int i = 0; i < ocupados; i++) {
    digitalWrite(pulsoPin, HIGH);
    delay(50);
    digitalWrite(pulsoPin, LOW);
    delay(50);
  }
  digitalWrite(sentidoPin, LOW);

  // Actualizar estado anterior de sensores
  for (int i = 0; i < 4; i++) {
    estadoAnteriorSensor[i] = digitalRead(sensores[i]);
  }

  Serial.print("Contador descendente = ");
  Serial.print(contadorActual);
  Serial.print(" | Ascendente sincronizado en ");
  Serial.println(ocupados);
}
