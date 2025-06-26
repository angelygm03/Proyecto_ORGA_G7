#include <LiquidCrystal.h>
#include <Servo.h>

// LCD: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// Servos
Servo servoEntrada;
Servo servoSalida;

// Pines de servos
const int pinServoEntrada = 9;
const int pinServoSalida = A2;

// Pines de CLK para contadores
const int clkAsc = A0;
const int clkDesc = A1;

// Pines de lectura desde el multiplexor (Q0–Q2)
const int q0Pin = A3;
const int q1Pin = A4;
const int q2Pin = A5;

// Pin de selección del multiplexor
const int selMux = 8;

// Pines de sensores simulados (DIP switches)
const int sensores[4] = {10, 11, 12, 13};
bool estadoAnterior[4] = {0, 0, 0, 0};
bool ocupacion[4] = {false, false, false, false};

void setup() {
  lcd.begin(16, 2);
  servoEntrada.attach(pinServoEntrada);
  servoSalida.attach(pinServoSalida);

  // Inicializar servos en posición cerrada
  servoEntrada.write(0);
  servoSalida.write(0);

  pinMode(clkAsc, OUTPUT);
  pinMode(clkDesc, OUTPUT);
  digitalWrite(clkAsc, LOW);
  digitalWrite(clkDesc, LOW);

  pinMode(selMux, OUTPUT);
  digitalWrite(selMux, LOW); // Inicia leyendo contador ascendente

  pinMode(q0Pin, INPUT);
  pinMode(q1Pin, INPUT);
  pinMode(q2Pin, INPUT);

  for (int i = 0; i < 4; i++) {
    pinMode(sensores[i], INPUT);
  }

  generarPulso(clkDesc);

  lcd.print("Sistema listo");
  delay(1000);
  lcd.clear();
}

void loop() {
  for (int i = 0; i < 4; i++) {
    bool estadoActual = digitalRead(sensores[i]);

    if (estadoActual != estadoAnterior[i]) {
      delay(20); // Antirrebote
      bool estadoConfirmado = digitalRead(sensores[i]);

      if (estadoConfirmado != estadoAnterior[i]) {
        if (estadoConfirmado == HIGH && !ocupacion[i]) {
          // Entrada válida
          generarPulso(clkAsc);  // Llenos++
          generarPulso(clkDesc); // Vacíos--
          ocupacion[i] = true;
          abrirBarreraEntrada();
        } else if (estadoConfirmado == LOW && ocupacion[i]) {
          // Salida válida
          generarPulso(clkDesc); // Vacíos++
          generarPulso(clkAsc);  // Llenos--
          ocupacion[i] = false;
          abrirBarreraSalida();
        }
        estadoAnterior[i] = estadoConfirmado;
      }
    }
  }

  // Leer contador ascendente (llenos)
  digitalWrite(selMux, LOW);
  delayMicroseconds(10);
  int llenos = leerContador();

  // Leer contador descendente (vacíos)
  digitalWrite(selMux, HIGH);
  delayMicroseconds(10);
  int vacios = leerContador();

  // Mostrar en LCD
  lcd.setCursor(0, 0);
  lcd.print("Llenos: ");
  lcd.print(llenos);
  lcd.print("   ");

  lcd.setCursor(0, 1);
  lcd.print("Vacíos: ");
  lcd.print(vacios);
  lcd.print("   ");

  delay(200);
}

int leerContador() {
  int b0 = digitalRead(q0Pin);
  int b1 = digitalRead(q1Pin);
  int b2 = digitalRead(q2Pin);
  return (b2 << 2) | (b1 << 1) | b0;
}

void generarPulso(int pin) {
  digitalWrite(pin, HIGH);
  delay(50);
  digitalWrite(pin, LOW);
}

void abrirBarreraEntrada() {
  servoEntrada.write(90);
  delay(20000); // 20 segundos
  servoEntrada.write(0);
}

void abrirBarreraSalida() {
  servoSalida.write(90);
  delay(30000); // 30 segundos
  servoSalida.write(0);
}
