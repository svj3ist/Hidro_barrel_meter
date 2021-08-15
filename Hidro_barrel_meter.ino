#include <EtherCard.h>

static byte mymac[] = { 0x38,0x09,0x56,0x25,0x60,0x60 };

static byte myip[] = { 192,168,0,102 };

byte Ethernet::buffer[1200];
BufferFiller bfill;

int LedPins[] = { A0,A1,A2,A3,A4,A5,2,3,4,5,6,7,8,9};

//-------------

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



//------------


void homePage()
{
  // Раскомментируйте / закомментируйте нужное вам количество датчиков.
  bool sensor0 = digitalRead(A0);
  bool sensor1 = digitalRead(A1);
  bool sensor2 = digitalRead(A2);
  bool sensor3 = digitalRead(A3);
  bool sensor4 = digitalRead(A4);
  bool sensor5 = digitalRead(A5);
  bool sensor6 = digitalRead(2);
  bool sensor7 = digitalRead(3);
  bool sensor8 = digitalRead(4);
  bool sensor9 = digitalRead(5);
  bool sensor10 = digitalRead(6);
  bool sensor11 = digitalRead(7);


  //------------


  // Для отображения на Web странице значений с аналоговых входов analogRead(A0),(A1),(A2),(A3).

  // Авто обновление Web страницы, пригодится для частоты отображения данных с датчиков.
  // После нажатия на устройстве клиента "ArduinoPIN ON или OFF", 
  // происходит обновление Web страницы, и значение с датчиков на ней.
  // Можно этот процесс автоматизировать, + установить таймер / время авто обновление Web страницы.
  // Если авто обновление Web страницы вам не нужно, закомментируйте стоку "<meta http-equiv='refresh' content='10'/>".  
  // Время таймера устанавливается content='2'/>" '2' это 2 секунды.

  // выводим значения analogRead(A0);.
  bfill.emit_p(PSTR(
    "<title>Barrel Meter</title>" 
  "<meta http-equiv='refresh' content='1'/>")); // Авто обновление Web страницы клиента, каждые 2 секунды.


 // bfill.emit_p(PSTR("<font color=\"brown\"><h3>digitalRead(A0)</h3>""<h1>$D</h1>"),sensor0);  // h1 размер шрифта.
  bfill.emit_p(PSTR("<h1>barrel 1 = $D$D$D""</h1>"),sensor0, sensor1 ,sensor2 );
   bfill.emit_p(PSTR("<h1>barrel 2 = $D$D$D""</h1>"),sensor3, sensor4 ,sensor5 );
    bfill.emit_p(PSTR("<h1>barrel 3 = $D$D$D""</h1>"),sensor6, sensor7 ,sensor8 );
     bfill.emit_p(PSTR("<h1>barrel 4 = $D$D$D""</h1>"),sensor9, sensor10 ,sensor11 );
/*  bfill.emit_p(PSTR("<font color=\"blue\"><h3>DigitalRead(A1)</h3>""<h1>$D</h1>"),sensor1);
  bfill.emit_p(PSTR("<font color=\"Amber\"><h3>DigitalRead(A2)</h3>""<h1>$D</h1>"),sensor2);  
  bfill.emit_p(PSTR("<font color=\"Burnt Orange\"><h3>DigitalRead(A3)</h3>" "<h1>$D</h1>"),sensor3);
  bfill.emit_p(PSTR("<font color=\"Red\"><h3>DigitalRead(A4)</h3>" "<h1>$D</h1>"),sensor4);
  bfill.emit_p(PSTR("<font color=\"Green\"><h3>DigitalRead(A5)</h3>" "<h1>$D</h1>"),sensor5);
  bfill.emit_p(PSTR("<font color=\"Green\"><h3>DigitalRead(2)</h3>" "<h1>$D</h1>"),sensor6);
  bfill.emit_p(PSTR("<font color=\"Green\"><h3>DigitalRead(3)</h3>" "<h1>$D</h1>"),sensor7);
  bfill.emit_p(PSTR("<font color=\"Green\"><h3>DigitalRead(4)</h3>" "<h1>$D</h1>"),sensor8);
  bfill.emit_p(PSTR("<font color=\"Green\"><h3>DigitalRead(5)</h3>" "<h1>$D</h1>"),sensor9);
  bfill.emit_p(PSTR("<font color=\"Green\"><h3>DigitalRead(6)</h3>" "<h1>$D</h1>"),sensor10);
  bfill.emit_p(PSTR("<font color=\"Green\"><h3>DigitalRead(7)</h3>" "<h1>$D</h1>"),sensor11);
  */
  // Можно в одной строке.
 // bfill.emit_p(PSTR("<h1>(A0)=$D"": (A1)=$D"": (A2)=$D"": (A3)=$D</h1>"),sensor0, sensor1 ,sensor2 ,sensor3);
}

void setup() {
  if (ether.begin(sizeof Ethernet::buffer, mymac,10) == 0);

  if (!ether.dhcpSetup());
  ether.staticSetup(myip);

 for(int i = 0; i <= 12; i ++)
  {
pinMode(LedPins[i],INPUT);
}
}

void loop() {

  delay(1); // Дёргаем микроконтроллер.

  word len = ether.packetReceive();   // check for ethernet packet / проверить ethernet пакеты.
  word pos = ether.packetLoop(len);   // check for tcp packet / проверить TCP пакеты.
 if (pos) {
    bfill = ether.tcpOffset();
    char *data = (char *) Ethernet::buffer + pos;
    if (strncmp("GET /", data, 5) != 0) {
      bfill.emit_p(http_Unauthorized);
    }
    else {

      data += 5;
      if (data[0] == ' ') {       
        homePage(); // Return home page, Если обнаружено изменения на станице, запускаем функцию.
 
      }


      else {
        // Page not found
        bfill.emit_p(http_Unauthorized);
      }
    }
    ether.httpServerReply(bfill.position());    // send http response
  }
}

