#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* =========================================================
   CONFIGURACIÓN DE HARDWARE (PINES)
   ========================================================= */
#define PIN_ADC_BAT   3

#define PIN_I2C_SDA   4
#define PIN_I2C_SCL   5

#define PIN_TTP       2
#define PIN_HUM       6

#define PIN_CHRG      7
#define PIN_STDBY     1

/* =========================================================
   CONFIGURACIÓN DE BATERÍA
   ========================================================= */
const float BAT_MAX_VOLT = 4.05;
const float BAT_MIN_VOLT = 1.55;

/* =========================================================
   CONFIGURACIÓN OLED
   ========================================================= */
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/* =========================================================
   ENUMERACIONES
   ========================================================= */
enum EstadoSistema {
  EST_READY,
  EST_RUN,
  EST_FULL,
  EST_LOW
};

enum EstadoCarga {
  CARGA_IDLE,
  CARGA_CHARGING,
  CARGA_FULL
};

/* =========================================================
   VARIABLES GLOBALES
   ========================================================= */
bool ttpAnterior   = LOW;
bool humEncendido  = false;

float vBatFiltrada = 0;

/* =========================================================
   CAPA HARDWARE
   ========================================================= */
EstadoCarga obtenerEstadoCarga() {

  bool chrg  = digitalRead(PIN_CHRG);
  bool stdby = digitalRead(PIN_STDBY);

  if (chrg == LOW)  return CARGA_CHARGING;
  if (stdby == LOW) return CARGA_FULL;

  return CARGA_IDLE;
}

float leerBateria() {

  long suma = 0;

  for (int i = 0; i < 100; i++) {
    suma += analogRead(PIN_ADC_BAT);
    delay(2);
  }

  float adcProm = suma / 100.0;
  float voltADC = (adcProm / 4095.0) * 3.6;
  float voltBat = (voltADC * (122.0 / 22.0)) * 0.831;

  return voltBat;
}

/* =========================================================
   CAPA DE PROCESAMIENTO
   ========================================================= */
int voltPor(float v) {

  float p;

  if (v >= BAT_MAX_VOLT)      p = 100;
  else if (v <= BAT_MIN_VOLT) p = 0;
  else p = (v - BAT_MIN_VOLT) * 100.0 / (BAT_MAX_VOLT - BAT_MIN_VOLT);

  int por5 = round(p / 5.0) * 5;
  return constrain(por5, 0, 100);
}

int voltPorEstable(float v) {

  static int porcentajeActual = -1;
  static float vRef = 0;

  int nuevo = voltPor(v);

  if (porcentajeActual < 0) {
    porcentajeActual = nuevo;
    vRef = v;
    return porcentajeActual;
  }

  if (abs(v - vRef) > 0.06) {
    porcentajeActual = nuevo;
    vRef = v;
  }

  return porcentajeActual;
}

EstadoSistema obtenerEstadoSistema(int porcentaje) {

  if (porcentaje <= 30) return EST_LOW;
  if (porcentaje >= 95) return EST_FULL;

  if (humEncendido) return EST_RUN;

  return EST_READY;
}

/* =========================================================
   CAPA DE ACTUACIÓN
   ========================================================= */
void dispararHumidificador() {

  digitalWrite(PIN_HUM, LOW);
  delay(10);
  digitalWrite(PIN_HUM, HIGH);
}

/* =========================================================
   CAPA DE INTERFAZ
   ========================================================= */
void dibujarIconoBateria(int porcentaje) {

  const int x = 4;
  const int y = 6;
  const int w = 48;
  const int h = 20;

  display.drawRect(x, y, w, h, SSD1306_WHITE);
  display.fillRect(x + w, y + 6, 3, 8, SSD1306_WHITE);

  const int margen = 2;
  const int anchoUtil = w - 2 * margen;
  int fill = map(porcentaje, 0, 100, 0, anchoUtil);

  display.fillRect(
    x + margen,
    y + margen,
    fill,
    h - 2 * margen,
    SSD1306_WHITE
  );
}

void dibujarTextoEstado(EstadoSistema estado, EstadoCarga carga) {

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // ===== Estado del sistema (principal) =====
  switch (estado) {

    case EST_READY:
      display.setCursor(68, 12);
      display.print("READY");
      break;

    case EST_RUN:
      display.setCursor(68, 12);
      display.print("RUN");
      break;

    case EST_FULL:
      display.setCursor(68, 6);
      display.print("BATTERY");
      display.setCursor(68, 18);
      display.print("FULL");
      break;

    case EST_LOW:
      display.setCursor(68, 6);
      display.print("BATTERY");
      display.setCursor(68, 18);
      display.print("LOW");
      break;
  }

  // ===== Estado de carga (secundario) =====
  if (carga == CARGA_CHARGING) {
    display.setCursor(68, 0);
    display.print("CHG");
  }

  if (carga == CARGA_FULL) {
    display.setCursor(68, 0);
    display.print("FULL");
  }
}

/* =========================================================
   SETUP
   ========================================================= */
void setup() {

  analogReadResolution(12);
  analogSetPinAttenuation(PIN_ADC_BAT, ADC_11db);

  pinMode(PIN_TTP, INPUT_PULLDOWN);
  pinMode(PIN_HUM, OUTPUT);
  digitalWrite(PIN_HUM, HIGH);

  pinMode(PIN_CHRG, INPUT_PULLUP);
  pinMode(PIN_STDBY, INPUT_PULLUP);

  Serial.begin(115200);

  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
  Wire.setClock(100000);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
}

/* =========================================================
   LOOP
   ========================================================= */
void loop() {

  // ===== TTP =====
  bool ttpActual = digitalRead(PIN_TTP);

  if (ttpAnterior == HIGH && ttpActual == LOW) {
    humEncendido = !humEncendido;
    dispararHumidificador();
  }

  ttpAnterior = ttpActual;

  // ===== BATERÍA =====
  if (!humEncendido) {
    vBatFiltrada = leerBateria();
  }

  int porcentaje = voltPorEstable(vBatFiltrada);

  // ===== ESTADOS =====
  EstadoCarga carga = obtenerEstadoCarga();
  EstadoSistema estado = obtenerEstadoSistema(porcentaje);

  // ===== DISPLAY =====
  display.clearDisplay();
  dibujarIconoBateria(porcentaje);
  dibujarTextoEstado(estado, carga);
  display.display();

  delay(200);
}