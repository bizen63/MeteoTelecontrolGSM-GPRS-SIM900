#include <SoftwareSerial.h>

SoftwareSerial SIM900(7, 8); // Configura el puerto serial para el SIM900

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x27

LiquidCrystal_I2C             lcd(I2C_ADDR,2, 1, 0, 4, 5, 6, 7);

char incoming_char; //Variable para guardar los caracteres que envía el SIM900
/*Creamos un array de 8 bytes 
donde haremos el caracter 'º' que necesitamos
*/
byte charGrado[8] = {
  0b01110,
  0b01010,
  0b01110,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};



#include <DHT.h>;

//Constants
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino


//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value

void setup()
{
    
  dht.begin();

   lcd.begin (16,2);    // Inicializar el display con 16 caraceres 2 lineas
   lcd.setBacklightPin(3,POSITIVE);
   lcd.setBacklight(HIGH);


  lcd.createChar(0, charGrado);


  SIM900.begin(19200); //Configura velocidad del puerto serie para el SIM900
  Serial.begin(19200); //Configura velocidad del puerto serie del Arduino
  Serial.println("OK");
  delay (1000);
  SIM900.println("AT + CPIN = \"3525\""); //Comando AT para introducir el PIN de la tarjeta. Cambiar por el vuestro.
  delay(25000); //Tiempo para que encuentre una RED
  Serial.println("PIN OK");
  SIM900.print("AT+CLIP=1\r"); // Activamos la identificación de llamadas
  delay(1000);
  SIM900.print("AT+CMGF=1\r"); //Configura el modo texto para enviar o recibir mensajes
  delay(1000);
  SIM900.print("AT+CNMI=2,2,0,0,0\r"); //Configuramos el módulo para que nos muestre los SMS recibidos por comunicacion serie
  delay(1000);

  
}

void loop()
{
    
    
   
 if (SIM900.available() > 0)
  {
    incoming_char = SIM900.read(); //Guardamos el caracter que llega desde el SIM900y
    Serial.print(incoming_char); //Mostramos el caracter en el monitor serie
}
if (incoming_char == '?')
{
      leeDatos();
     mensaje_sms(); //Envia SMS
 Serial.println(incoming_char);
}
}
   void mensaje_sms()
{
  Serial.println("Enviando SMS...");
  SIM900.print("AT+CMGF=1\r"); //Comando AT para mandar un SMS
  delay(1000);
  SIM900.println("AT + CMGS = \"*********\""); //Numero al que vamos a enviar el mensaje. Cambiar asteriscos por nº correcto.
  delay(1000);
  SIM900.println("INFO METEO V. LAFARGA" );// Texto del SMS
  delay(100);
  SIM900.print("Temperatura: " );// Texto del SMS
  delay(100);
  SIM900.print(temp);// Texto del SMS
  delay(100);
   SIM900.println(" Gr. Celsius, " );// Texto del SMS
  delay(100);
  SIM900.print("Humedad: " );// Texto del SMS
  delay(100);
  SIM900.print(hum);// Texto del SMS
  delay(100);
  SIM900.println("%" );// Texto del SMS
  delay(100);
  SIM900.println((char)26);//Comando de finalizacion ^Z
  delay(100);
  SIM900.println();
  delay(5000); // Esperamos un tiempo para que envíe el SMS
  Serial.println("SMS enviado");
  incoming_char = '0';
}

void leeDatos()
{
  //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");

       lcd.home ();                   // go home
       lcd.print("Temp:");
       lcd.setCursor ( 6, 0 );
       lcd.print(temp);
       lcd.setCursor ( 12, 0 );
       lcd.write(byte(0));
       lcd.setCursor ( 13, 0 );
       lcd.print("C");
       lcd.setCursor ( 0, 1 );
       lcd.print("Hum:");
       lcd.setCursor ( 6, 1 );
       lcd.print(hum);
       lcd.setCursor ( 12, 1 );
       lcd.print("%");
}


