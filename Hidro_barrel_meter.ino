#include <EtherCard.h>

static byte mymac[] = { 0x38,0x09,0x56,0x25,0x60,0x60 };

static byte myip[] = { 192,168,0,106 };

byte Ethernet::buffer[1000];
BufferFiller bfill;

int Pins[] = { 14,15,16,17,18,19,3,4,5,6,7,8,9,10,11};


const char http_OK[] =
"HTTP/1.0 200 OK\r\n"
"Content-Type: text/html\r\n"
"Pragma: no-cache\r\n\r\n";

const char http_Found[] =
"HTTP/1.0 302 Found\r\n"
"Location: /\r\n\r\n";

const char http_Unauthorized[] =
"HTTP/1.0 401 Unauthorized\r\n"
"Content-Type: text/html\r\n\r\n"
"<h1>401 Unauthorized</h1>";





void homePage()
{
  bool sensor0 = digitalRead(14);
  bool sensor1 = digitalRead(15);
  bool sensor2 = digitalRead(16);
  bool sensor3 = digitalRead(17);
  bool sensor4 = digitalRead(18);
  bool sensor5 = digitalRead(19);
  bool sensor6 = digitalRead(3);
  bool sensor7 = digitalRead(4);
  bool sensor8 = digitalRead(5);
  bool sensor9 = digitalRead(6);
  bool sensor10 = digitalRead(7);
  bool sensor11 = digitalRead(8);
  bool sensor12 = digitalRead(9);
  bool sensor13 = digitalRead(10);
  bool sensor14 = digitalRead(11);

Serial.print("barrel 1 = ");
Serial.print(!sensor0);
Serial.print(!sensor1);
Serial.println(!sensor2);

Serial.print("barrel 2 = ");
Serial.print(!sensor4);
Serial.print(!sensor5);
Serial.println(!sensor6);

Serial.print("barrel 3 = ");
Serial.print(!sensor7);
Serial.print(!sensor8);
Serial.println(!sensor9);

Serial.print("barrel 4 = ");
Serial.print(!sensor10);
Serial.print(!sensor11);
Serial.println(!sensor12);

  Serial.print("Pressure is:  ");
  float pressure_kPa = map(analogRead(A6), 95, 922, 101, 1200);
  Serial.print(pressure_kPa);
  int kPa = pressure_kPa;
  Serial.print(" kPa ");
  Serial.print(" ");
  float pressure_psi = pressure_kPa / 100;
  Serial.print(pressure_psi);
  int psi = pressure_psi;
  Serial.println(" psi ");

    Serial.print("Pressure2 is: ");
  float pressure2_kPa = map(analogRead(A7), 100, 922, 101, 1200);
  Serial.print(pressure2_kPa);
  int kPa2 = pressure2_kPa;
  Serial.print(" kPa2");
  Serial.print(" ");
  float pressure2_psi = pressure2_kPa * 0.14503773773020923;
  Serial.print(pressure2_psi);
  int psi2 = pressure2_psi;
  Serial.println(" psi2");
  Serial.println(" ");
  bfill.emit_p(PSTR(
    "<title>Barrel Meter</title>" 
  "<meta http-equiv='refresh' content='1'/>")); 


 
  bfill.emit_p(PSTR("<h1>Osmos = $D$D$D""</h1>"),!sensor0, !sensor1 ,!sensor2 );
  bfill.emit_p(PSTR("<h1>Water = $D$D$D""</h1>"),!sensor3, !sensor4 ,!sensor5 );
  bfill.emit_p(PSTR("<h1>Washer = $D$D$D""</h1>"),!sensor6, !sensor7 ,!sensor8 );
  bfill.emit_p(PSTR("<h1>Wax = $D$D$D""</h1>"),!sensor9,!sensor10, !sensor11 );
  bfill.emit_p(PSTR("<h1>Foam = $D$D$D""</h1>"),!sensor12 ,!sensor13, !sensor14 );
  bfill.emit_p(PSTR("<h1>Pressure kPa = $D""  Pressure BAR = $D""</h1>"),kPa, psi );
//  bfill.emit_p(PSTR("<h1>Pressure2 kPa = $D""  Pressure2 psi = $D""</h1>"),kPa2, psi2 );
}

void setup() {
      Serial.begin(9600);
      Serial.println("System ON");
  if (ether.begin(sizeof Ethernet::buffer, mymac,SS) == 0);

  if (!ether.dhcpSetup());
  ether.staticSetup(myip);

 for(int i = 0; i <= 14; i ++)
  {
pinMode(Pins[i],INPUT_PULLUP);
}
}

void loop() {




  delay(1);

  word len = ether.packetReceive(); 
  word pos = ether.packetLoop(len);
 if (pos) {
    bfill = ether.tcpOffset();
    char *data = (char *) Ethernet::buffer + pos;
    if (strncmp("GET /", data, 5) != 0) {
      bfill.emit_p(http_Unauthorized);
    }
    else {

      data += 5;
      if (data[0] == ' ') {       
        homePage();
      }


      else {
        
        bfill.emit_p(http_Unauthorized);
      }
    }
    ether.httpServerReply(bfill.position()); 
  }
}
