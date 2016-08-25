#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8); // Configura el puerto serial para el SIM900

char incoming_char; //Variable para guardar los caracteres que envía el SIM900

void setup()
{
  SIM900.begin(19200); //Configura velocidad del puerto serie para el SIM900
  Serial.begin(19200); //Configura velocidad del puerto serie del Arduino
  Serial.println("OK");
  delay (1000);
  SIM900.println("AT + CPIN = \"3525\""); //Comando AT para introducir el PIN de la tarjeta
  delay(25000); //Tiempo para que encuentre una RED
  Serial.println("PIN OK");
  SIM900.print("AT+CLIP=1\r"); // Activamos la identificación de llamadas
  delay(1000);
  SIM900.print("AT+CMGF=1\r"); //Configura el modo texto para enviar o recibir mensajes
  delay(1000);
  SIM900.print("AT+CNMI=2,2,0,0,0\r"); //Configuramos el módulo para que nos muestre los SMS recibidos por comunicacion serie
  delay(1000);

  pinMode(9,OUTPUT);
  incoming_char == 'L';
}

void loop()
{
 
  if (SIM900.available() > 0)
  {
    incoming_char = SIM900.read(); //Guardamos el caracter que llega desde el SIM900y
    Serial.print(incoming_char); //Mostramos el caracter en el monitor serie
    
}
if (incoming_char == 'H')
{
  digitalWrite(9,HIGH);
  mensaje_smsON();

  }
if (incoming_char == 'L')
{
  digitalWrite(9,LOW);
  mensaje_smsOFF();
  
}
}

void mensaje_smsON()
{
  Serial.println("Enviando SMS...");
  SIM900.print("AT+CMGF=1\r"); //Comando AT para mandar un SMS
  delay(1000);
  SIM900.println("AT + CMGS = \"*********\""); //Numero al que vamos a enviar el mensaje: Cambiar asteríscos por nº correcto.
  delay(1000);
  SIM900.println("SISTEMA CONECTADO");// Texto del SMS
  delay(100);
  SIM900.println((char)26);//Comando de finalizacion ^Z
  delay(100);
  SIM900.println();
  delay(5000); // Esperamos un tiempo para que envíe el SMS
  Serial.println("SMS enviado");
  incoming_char == '0';
}

void mensaje_smsOFF()
{
  Serial.println("Enviando SMS...");
  SIM900.print("AT+CMGF=1\r"); //Comando AT para mandar un SMS
  delay(1000);
  SIM900.println("AT + CMGS = \"*********\""); //Numero al que vamos a enviar el mensaje: Cambiar asteríscos por nº correcto.
  delay(1000);
  SIM900.println("SISTEMA DESCONECTADO");// Texto del SMS
  delay(100);
  SIM900.println((char)26);//Comando de finalizacion ^Z
  delay(100);
  SIM900.println();
  delay(5000); // Esperamos un tiempo para que envíe el SMS
  Serial.println("SMS enviado");
  incoming_char == '0';
}
