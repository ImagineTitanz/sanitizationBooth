/* Mueblares Morelos
*/
#include <Arduino.h>

#include <Adafruit_GFX.h>     //Librería para controla los Global Effects
#include <Adafruit_SSD1306.h> //Librería para controlar la pantalla OLED SSD1306
#define ANCHO 128             //Reemplaza ocurrencia de ANCHO por 128
#define ALTO 64               //Reemplaza ocurrencia de ALTO por 64
#define OLED_RESET 4          //Necesario por la libreria y el tipo de pantalla del fabricante Adafruit

Adafruit_SSD1306 oled(ANCHO, ALTO, &Wire, OLED_RESET); //Crea objeto de la pantalla oled

#include <mlx90615.h>
MLX90615 mlx = MLX90615(); //Crea objeto del sensor de temperatura médico

int led_verde = 12;                  //Pin de conección al arduino del led verde
int led_rojo = 11;                   //Pin de conección al arduino del led rojo
int led_amarillo, nebulizacion = 10; //Pin de conección al arduino del led amarillo
int ventiladores = 8;                //Pin de conección al arduino de ventiladores de circulación de neblina
int sensor_linea = 5;                //Pin de conección al arduino del sensor de linea
int buzzer = 4;                      //Pin de conección al arduino del buzzer
int sensor_presencia = 3;            //Pin de conección al arduino del sensor de presencia
int sensor_alcohol = A0;             //Pin de conección al arduino del sensor de alcohol

// int neblina_maxima = 400;    //Tope de neblina en la cabina de sanitización
// int neblina_minima = 300;    //Minimo de neblino que se quiere tener en la cabina
int buzzer_tiempo = 10;      //Tiempo que durará el buzzer encendido
int usuario_detectado;       //Variable para almacenar el valor del sensor de presencia
int valor_alcohol;           //Variable para almacenar el valor del sensor de alcohol
int tempus;                  //Definir mejor, cambiada por "temperatura_leida" no era necesaria
bool finalizado = false;     //Definir mejor
bool cambio = false;         //Definir mejor
bool verifica = false;       //Definir mejor
bool activa = false;         //Definir mejor
bool persona_activa = false; //Definir mejor
float temperatura_leida;
String temperatura_mostrada;
char temperatura_enviada[6];

void nebulizacionConstante(int neblina_estandar); //Funcion de estado general, llena constantemente de sustancia el ambiente

void setup()
{
    Serial.begin(9600);
    Wire.begin();                           // inicializa bus I2C
    oled.begin(SSD1306_SWITCHCAPVCC, 0x3C); // inicializa pantalla con direccion 0x3C
    mlx.begin();                            //inicializa el sensor de temperatura

    //Declaración de salidas y entradas digitales:
    pinMode(led_rojo, OUTPUT);
    pinMode(led_verde, OUTPUT);
    pinMode(led_amarillo, OUTPUT);
    pinMode(nebulizacion, OUTPUT);
    pinMode(ventiladores, OUTPUT);
    pinMode(buzzer, OUTPUT);

    pinMode(sensor_alcohol, INPUT);
    pinMode(sensor_linea, INPUT);
    pinMode(sensor_presencia, INPUT);
}

void loop()
{
    nebulizacionConstante(400); //Se declara el valor estandar al que se va a encender el nebulizador
    usuario_detectado = digitalRead(sensor_presencia);

    if (usuario_detectado == 1)
    {
        usuario_detectado = digitalRead(sensor_presencia); // Verifica que una persona vaya a tomar temperatura
        oled.clearDisplay();                               // limpia pantalla
        oled.setTextColor(WHITE);                          // establece color al unico disponible (pantalla monocromo)
        oled.setCursor(0, 0);                              // ubica cursor en inicio de coordenadas 0,0
        oled.setTextSize(2.3);                             // establece tamano de texto en 1
        oled.print("Coloque su\nfrente");
        oled.display();
    }
    else if (usuario_detectado == 0 && temperatura_leida == 0)
    {

        oled.clearDisplay();      // limpia pantalla
        oled.setTextColor(WHITE); // establece color al unico disponible (pantalla monocromo)
        oled.setCursor(0, 0);     // ubica cursor en inicio de coordenadas 0,0
        oled.setTextSize(3);      // establece tamano de texto en 1
        oled.print(".");
        oled.display();
        delay(1000);
        oled.clearDisplay();
        oled.print(".");
        oled.display();
        delay(1000);
        oled.clearDisplay();
        oled.print(".");
        oled.display();
        delay(1000);
        oled.clearDisplay();                              // Limpia pantalla
        oled.setCursor(0, 0);                             // Ubica cursor en inicio de coordenadas 0,0
        temperatura_leida = mlx.get_object_temp();        //Guarda el valor de la temperatura leida por el sensor
        temperatura_mostrada = String(temperatura_leida); //Transforma a string y guarda el valor de la temperatura leida
        oled.print(temperatura_mostrada + "oC");          // Escribe en pantalla el texto

        int large = temperatura_mostrada.length();
        for (int i = 0; i < large; i++)
        {
            temperatura_enviada[i] = temperatura_mostrada[i];
        }
        Serial.write(temperatura_enviada, 6);
        oled.display(); // Muestra en pantalla la temperatura del usuario en °C

        if (temperatura_leida >= 37 && temperatura_leida != 0) //Fiebre
        {
            digitalWrite(led_rojo, HIGH);
            digitalWrite(led_verde, LOW);
            for (int i = 0; i == buzzer_tiempo; i++)
            {
                digitalWrite(buzzer, !digitalRead(buzzer));
                delay(buzzer_tiempo);
            }
            delay(5000);
            digitalWrite(buzzer, LOW);
            digitalWrite(led_rojo, LOW);
            temperatura_leida = 0;
        }
        else if (temperatura_leida < 37 && temperatura_leida != 0) // Sin fiebre
        {
            digitalWrite(led_verde, HIGH);
        }
    }
}

void nebulizacionConstante(int neblina_estandar) //Funcion de estado general, llena constantemente de sustancia el ambiente
{
    valor_alcohol = analogRead(sensor_alcohol);

    if (valor_alcohol > neblina_estandar)
    {
        digitalWrite(led_verde, HIGH);
    }
    else
    {
        digitalWrite(led_verde, LOW);
    }

    if (valor_alcohol < neblina_estandar)
    {
        digitalWrite(led_amarillo && nebulizacion, HIGH);
    }
    else
    {
        digitalWrite(led_amarillo && nebulizacion, LOW);
    }
}