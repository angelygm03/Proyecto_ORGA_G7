#include <Servo.h>
#include "NewPing.h"

// ----- CONFIGURACION -----
#define TRIG_ENT 12
#define ECHO_ENT 11
#define TRIG_SAL 7
#define ECHO_SAL 6
#define BUZZER 8
#define MAX_DISTANCIA 100
#define UMBRAL_DETECCION 20

Servo servoEntrada;
Servo servoSalida;
NewPing sonarEntrada(TRIG_ENT, ECHO_ENT, MAX_DISTANCIA);
NewPing sonarSalida(TRIG_SAL, ECHO_SAL, MAX_DISTANCIA);

unsigned long ultimoMovimientoEntrada = 0;
unsigned long ultimoMovimientoSalida = 0;
unsigned long tiempoEspera = 2000;
bool barreraEntradaLevantada = false;
bool barreraSalidaLevantada = false;

// Estados de control
bool modoPanico = false;
bool modoNoche = false;
bool modoEvacuacion = false;
bool modoMantenimiento = false;

void setup() {
  Serial.begin(9600);
  servoEntrada.attach(9);
  servoSalida.attach(10);
  pinMode(BUZZER, OUTPUT);

  servoEntrada.write(0);
  servoSalida.write(0);
  digitalWrite(BUZZER, LOW);
  Serial.println("Sistema listo.");
}

void loop() {
  delay(100);

  int distanciaEnt = sonarEntrada.ping_cm();
  int distanciaSal = sonarSalida.ping_cm();

  if (!modoPanico && !modoNoche && !modoEvacuacion) {
    if (distanciaEnt > 0 && distanciaEnt < UMBRAL_DETECCION && !barreraEntradaLevantada && !modoMantenimiento) {
      levantarEntrada();
    }
    if (distanciaSal > 0 && distanciaSal < UMBRAL_DETECCION && !barreraSalidaLevantada) {
      levantarSalida();
    }
  }

  if (barreraEntradaLevantada && millis() - ultimoMovimientoEntrada > tiempoEspera) {
    bajarEntrada();
  }
  if (barreraSalidaLevantada && millis() - ultimoMovimientoSalida > tiempoEspera) {
    bajarSalida();
  }

  // Leer comandos
  if (Serial.available()) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();

    if (comando == "panico") activarPanico();
    else if (comando == "noche") activarNoche();
    else if (comando == "evacuacion") activarEvacuacion();
    else if (comando == "mantenimiento") activarMantenimiento();
    else if (comando == "amb") desactivarModos();
    else Serial.println("Comando desconocido");

    while (Serial.available()) Serial.read(); // limpiar buffer
  }

  // Sonido en modos con buzzer
  if (modoPanico || modoEvacuacion) {
    digitalWrite(BUZZER, millis() % 1000 < 500 ? HIGH : LOW); // bip intermitente
  } else {
    digitalWrite(BUZZER, LOW);
  }
}

// ----- FUNCIONES DE MODOS -----
void activarPanico() {
  modoPanico = true;
  modoNoche = false;
  modoEvacuacion = false;
  modoMantenimiento = false;
  bajarEntrada();
  bajarSalida();
  digitalWrite(BUZZER, HIGH);
  Serial.println("Modo PANICO activado");
}

void activarNoche() {
  modoPanico = false;
  modoNoche = true;
  modoEvacuacion = false;
  modoMantenimiento = false;
  bajarEntrada();
  bajarSalida();
  digitalWrite(BUZZER, LOW);
  Serial.println("Modo NOCHE activado");
}

void activarEvacuacion() {
  modoPanico = false;
  modoNoche = false;
  modoEvacuacion = true;
  modoMantenimiento = false;
  levantarEntrada();
  levantarSalida();
  digitalWrite(BUZZER, HIGH);
  Serial.println("Modo EVACUACION activado");
}

void activarMantenimiento() {
  modoPanico = false;
  modoNoche = false;
  modoEvacuacion = false;
  modoMantenimiento = true;
  bajarEntrada();
  bajarSalida();
  digitalWrite(BUZZER, LOW);
  Serial.println("Modo MANTENIMIENTO activado");
}

void desactivarModos() {
  modoPanico = false;
  modoNoche = false;
  modoEvacuacion = false;
  modoMantenimiento = false;
  bajarEntrada();
  bajarSalida();
  digitalWrite(BUZZER, LOW);
  Serial.println("Modo NORMAL activado (contraseña correcta)");
}

// ----- FUNCIONES DE CONTROL -----
void levantarEntrada() {
  servoEntrada.write(90);
  barreraEntradaLevantada = true;
  ultimoMovimientoEntrada = millis();
  Serial.println("🚗 Entrada abierta.");
}

void bajarEntrada() {
  servoEntrada.write(180);
  barreraEntradaLevantada = false;
  Serial.println("✅ Entrada cerrada.");
}

void levantarSalida() {
  servoSalida.write(90);
  barreraSalidaLevantada = true;
  ultimoMovimientoSalida = millis();
  Serial.println("🚗 Salida abierta.");
}

void bajarSalida() {
  servoSalida.write(180);
  barreraSalidaLevantada = false;
  Serial.println("✅ Salida cerrada.");
}
