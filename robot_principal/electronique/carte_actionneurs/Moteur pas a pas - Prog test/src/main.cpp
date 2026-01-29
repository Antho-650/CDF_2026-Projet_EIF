#include <Arduino.h>

// ==========================================================
// CONFIGURATION DU MOTEUR
// ==========================================================
// Changez cette valeur pour choisir le moteur (1 ou 2)
#define MOTOR_CHOICE 1  

// Moteur 1 : Wantai 42BYGHW609L250
// Spécifications : 1.8°/step
// Réglage Microstepping : 1/16
// Steps par tour : 200 * 16 = 3200

// Moteur 2 : 17hs3001
// Spécifications : 1.8°/step
// Réglage Microstepping : 1/8
// Steps par tour : 200 * 8 = 1600

// ==========================================================
// DEFINITION DES PINS (ESP32)
// ==========================================================
const int PIN_STEP = 18;
const int PIN_DIR  = 19;
const int PIN_EN   = 21;

// Nouveaux pins pour les Microsteps (à connecter au driver LV8729)
// MS1 -> GPIO 32
// MS2 -> GPIO 33
// MS3 -> GPIO 25
const int PIN_MS1 = 32; 
const int PIN_MS2 = 33; 
const int PIN_MS3 = 25; 

// ==========================================================
// LOGIQUE DE CONFIGURATION
// ==========================================================

#if MOTOR_CHOICE == 1
  // --- Moteur 1 (1/16) ---
  const int MICROSTEPS = 16;
  // Configuration LV8729 pour 1/16 : L, L, H
  const int MS1_STATE = LOW;
  const int MS2_STATE = LOW;
  const int MS3_STATE = HIGH;
  
#elif MOTOR_CHOICE == 2
  // --- Moteur 2 (1/8) ---
  const int MICROSTEPS = 8;
  // Configuration LV8729 pour 1/8 : H, H, L
  const int MS1_STATE = HIGH;
  const int MS2_STATE = HIGH;
  const int MS3_STATE = LOW;
  
#else
  #error "MOTOR_CHOICE doit etre 1 ou 2"
#endif

const int STEPS_PER_REV_BASE = 200; // Moteur 1.8 deg
const int STEPS_TARGET = STEPS_PER_REV_BASE * MICROSTEPS; // Nombre de pas pour un tour complet

// Vitesse : Délai entre les pas
// Avec des microsteps élevés (1/16), il faut réduire ce délai pour garder une vitesse raisonnable.
// 200 us donne une bonne vitesse pour 1/16.
const int SPEED_DELAY_US = 200; 

void setup() {
  Serial.begin(115200);
  Serial.println("=========================================");
  Serial.printf("TEST MOTEUR - CONFIGURATION: MOTEUR %d\n", MOTOR_CHOICE);
  Serial.printf("Microstepping: 1/%d\n", MICROSTEPS);
  Serial.printf("Pas par tour: %d\n", STEPS_TARGET);
  Serial.println("=========================================");

  // Configuration des pins
  pinMode(PIN_STEP, OUTPUT);
  pinMode(PIN_DIR, OUTPUT);
  pinMode(PIN_EN, OUTPUT);
  
  pinMode(PIN_MS1, OUTPUT);
  pinMode(PIN_MS2, OUTPUT);
  pinMode(PIN_MS3, OUTPUT);

  // Application de la configuration Microstepping
  digitalWrite(PIN_MS1, MS1_STATE);
  digitalWrite(PIN_MS2, MS2_STATE);
  digitalWrite(PIN_MS3, MS3_STATE);
  
  // Activation du driver (Active LOW)
  digitalWrite(PIN_EN, LOW); 
  Serial.println("Driver ACTIVE (Enable LOW)");
}

void loop() {
  // --- Sens 1 ---
  Serial.println("Rotation Sens 1 (1 tour)");
  digitalWrite(PIN_DIR, HIGH); 

  for(int i = 0; i < STEPS_TARGET; i++) {
    digitalWrite(PIN_STEP, HIGH);
    delayMicroseconds(SPEED_DELAY_US);
    digitalWrite(PIN_STEP, LOW);
    delayMicroseconds(SPEED_DELAY_US);
  }

  delay(1000); // 1 seconde de pause

  // --- Sens 2 ---
  Serial.println("Rotation Sens 2 (1 tour)");
  digitalWrite(PIN_DIR, LOW); 

  for(int i = 0; i < STEPS_TARGET; i++) {
    digitalWrite(PIN_STEP, HIGH);
    delayMicroseconds(SPEED_DELAY_US);
    digitalWrite(PIN_STEP, LOW);
    delayMicroseconds(SPEED_DELAY_US);
  }

  delay(1000); // 1 seconde de pause
}
