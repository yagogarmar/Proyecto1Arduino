#include <LiquidCrystal_I2C.h>
#include <Stepper.h>

#include <DHT.h>
#include <DHT_U.h>

// Constantes con los pines 
#define SENSORCREP = 0;  
#define TRIG = 10;       
#define ECO = 9;       
#define SENSOR = 8;  
#define BUZZER_ACTIVO 12; 
#define LED 11; 

//Variables motor 
String estado = "Parada";
int pasos;
bool persiana = false;
int direccion = 100;
// Variables sensor temp
int TEMPERATURA;
int HUMEDAD;
//Variable sensor crepuscular
int VALOR;
String dia = "";
//Variables sensor distancia
int DURACION;
int DISTANCIA;


//Creacion objetos 
DHT dht(SENSOR, DHT11);  
LiquidCrystal_I2C lcd(0x27, 16, 2);
Stepper motor(2048, 2, 5, 3, 4);


void setup() {
  lcd.init();   
  dht.begin();  
  lcd.backlight();
  lcd.clear();
  Serial.begin(9600);  

  pinMode(TRIG, OUTPUT);  
  pinMode(ECO, INPUT);    
  pinMode(BUZZER_ACTIVO, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(7, INPUT);
  pinMode(6, INPUT);

  motor.setSpeed(7);
}

void loop() {
  VALOR = analogRead(SENSORCREP);


  digitalWrite(TRIG, HIGH);
  delay(1);
  digitalWrite(TRIG, LOW);
  DURACION = pulseIn(ECO, HIGH);
  DISTANCIA = DURACION / 58.2;

  TEMPERATURA = dht.readTemperature();  
  HUMEDAD = dht.readHumidity();         

  lcd.clear();
  lcd.setCursor(0, 0);     
  lcd.print("T:");         
  lcd.print(TEMPERATURA);  
  lcd.print(" H:");        
  lcd.print(HUMEDAD);      
  lcd.print(" D:");       
  lcd.print(DISTANCIA);    
  lcd.setCursor(0, 1);     

  if (VALOR < 650) {  
    dia = "Noche";
    digitalWrite(LED, HIGH);
  } else {
    dia = "Dia";
    digitalWrite(LED, LOW);
  }
  lcd.print(dia);
  lcd.setCursor(6, 1);
  lcd.print(estado);

  if (DISTANCIA < 7) {
    digitalWrite(BUZZER_ACTIVO, HIGH);  
    delay(500);                        
    digitalWrite(BUZZER_ACTIVO, LOW);   
  }

  if (digitalRead(7) == HIGH) {
    direccion = 100;
    pasos = 0;
    persiana = true;
    delay(20);
  }
  if (digitalRead(6) == HIGH) {
    direccion = -100;
    pasos = 0;
    persiana = true;
    delay(20);
  }

  if (persiana) {
    motor.step(direccion);

    if (pasos == 10) {
      persiana = false;
    }
    if (direccion < 0) {
      estado = "Subiendo";
    } else {
      estado = "Bajando";
    }
    pasos += 1;

  } else {
    estado = "Parada";
  }


  delay(0);
}