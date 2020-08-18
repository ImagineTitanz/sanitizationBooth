#include <Arduino.h>

#include <Adafruit_GFX.h>     //Librería para controla los Global Effects
#include <Adafruit_SSD1306.h> //Librería para controlar la pantalla OLED SSD1306
#define ANCHO 128             // reemplaza ocurrencia de ANCHO por 128
#define ALTO 64               // reemplaza ocurrencia de ALTO por 64
#define OLED_RESET 4          // necesario por la libreria y el tipo de pantalla del fabricante Adafruit

Adafruit_SSD1306 oled(ANCHO, ALTO, &Wire, OLED_RESET); // crea objeto de la pantalla oled

#include <mlx90615.h>
MLX90615 mlx = MLX90615(); // crea objeto del sensor de temperatura médico

int ledverde = 12;
int ledrojo = 11;
int ledamarillo, nebulizacion = 10;
int ventiladores = 8;
int buzzer = 4;
int buzzer_tiempo = 10;

int usuario_detectado; //Valor para almacenar el valor del sensor de presencia
int neblina_maxima = 400;
int neblina_minima = 300;

int sensor_linea = 5;
int sensor_presencia = 3;
int sensor_alcohol = A0;

int valor_alcohol;

void setup()
{
    pinMode(ledrojo, OUTPUT);
}

//valor_alcohol = analogRead(sensor_alcohol);