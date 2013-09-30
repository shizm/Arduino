#include <SPI.h>
#include <Ethernet.h>

#define LED1PIN (7)
#define LED2PIN (8)
#define LED3PIN (9)

#define LINEBUFFERSIZE (128)
#define DELIMITER ("&")

unsigned char MACADDRESS[] = {0x90, 0xA2, 0xDA, 0x0E, 0xF2, 0x36};
unsigned char IPADDRESS[] = { 192, 168, 111, 99 };
EthernetServer g_server(80);

void setup()
{
  Serial.begin(9600);
  while(!Serial) {
    ;
  }
  
  Ethernet.begin(MACADDRESS, IPADDRESS);
  g_server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  pinMode(LED1PIN, OUTPUT);
  pinMode(LED2PIN, OUTPUT);
  pinMode(LED3PIN, OUTPUT);
}

// HTML出力
void PrintHtml(EthernetClient& client, boolean led1, boolean led2, boolean led3, int speed)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  
  client.println("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">");
  client.println("<html lang=\"ja\">");
  client.println("<head>");
  client.println("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
  client.println("<meta http-equiv=\"Content-Style-Type\" content=\"text/css\">");
  client.println("<title></title>");
  client.println("</head>");
  client.println("<body>");
  
  client.println("<p>");
  client.print("LED1 : "); client.print( led1 ? "ON" : "off" ); client.println("<br>");
  client.print("LED2 : "); client.print( led2 ? "ON" : "off" ); client.println("<br>");
  client.print("LED3 : "); client.print( led3 ? "ON" : "off" ); client.println("<br>");
  client.println("<br>");
  client.println("速さ：");
  client.println(speed);
  client.println("</p>");
  
  client.println("<hr>");
  
  client.println("<form method=\"post\" action=\"\">");
  client.print("<input type=\"checkbox\" name=\"led1\" value=\"1\""); if(led1){ client.print(" checked"); } client.println(">LED1<br>");
  client.print("<input type=\"checkbox\" name=\"led2\" value=\"1\""); if(led2){ client.print(" checked"); } client.println(">LED2<br>");
  client.print("<input type=\"checkbox\" name=\"led3\" value=\"1\""); if(led3){ client.print(" checked"); } client.println(">LED3<br>");
  client.println("<br>");
  client.println("速さ<br>");
  client.println("1 ");
  client.print("<input type=\"radio\" name=\"speed\" value=\"1\""); if(1==speed){ client.print(" checked"); } client.println("> ");
  client.print("<input type=\"radio\" name=\"speed\" value=\"2\""); if(2==speed){ client.print(" checked"); } client.println("> ");
  client.print("<input type=\"radio\" name=\"speed\" value=\"3\""); if(3==speed){ client.print(" checked"); } client.println("> ");
  client.print("<input type=\"radio\" name=\"speed\" value=\"4\""); if(4==speed){ client.print(" checked"); } client.println("> ");
  client.print("<input type=\"radio\" name=\"speed\" value=\"5\""); if(5==speed){ client.print(" checked"); } client.println("> ");
  client.print("<input type=\"radio\" name=\"speed\" value=\"6\""); if(6==speed){ client.print(" checked"); } client.println("> ");
  client.print("<input type=\"radio\" name=\"speed\" value=\"7\""); if(7==speed){ client.print(" checked"); } client.println("> ");
  client.print("<input type=\"radio\" name=\"speed\" value=\"8\""); if(8==speed){ client.print(" checked"); } client.println("> ");
  client.print("<input type=\"radio\" name=\"speed\" value=\"9\""); if(9==speed){ client.print(" checked"); } client.println("> ");
  client.println(" 10<br>");
  client.println("<br>");
  client.println("<input type=\"submit\" value=\"送信\">");
  client.println("<input type=\"reset\" value=\"リセット\"></form>");
  client.println("<hr>");
  
  client.println("</body>");
  client.println("</html>");
}

boolean AnalyzeLineString(char* pszLine, boolean& rbLed1, boolean& rbLed2, boolean& rbLed3, int& riSpeed)
{
  char* pszToken = strtok(pszLine, DELIMITER);
  while(pszToken)
  {
    if (strlen(pszToken) == 6 && strncmp(pszToken, "led", 3) == 0) {
      if (pszToken[3] == '1') { rbLed1 = true; }
      else if (pszToken[3] == '2') { rbLed2 = true; }
      else if (pszToken[3] == '3') { rbLed3 = true; }
    } else if (strlen(pszToken) == 7 && strncmp(pszToken, "speed", 5) == 0) {
      riSpeed = pszToken[6] - '0';
    }
    pszToken = strtok(NULL, DELIMITER);
  }
  return true;
}

boolean readHttpRequest(EthernetClient& client, boolean& rbLed1, boolean& rbLed2, boolean& rbLed3, int& riSpeed)
{
  if (!client) {
    return false;
  }
  
  // HTTPリクエスト空行(\r\n\r\n)
  boolean foundRNRN = false;
  boolean currentLineIsBlank = true;
  char szLine[LINEBUFFERSIZE];
  int iIndex = 0;
  while (client.connected()) {
    if (!client.available()) {
      break;
    }
    
    char c = client.read();
    //Serial.print(c);
    if (c == '\n' && currentLineIsBlank) {
      // 空行発見
      foundRNRN = true;
    }
    if (c == '\n') {
      // HTTPリクエスト1行の解析
      if (foundRNRN && 0 != iIndex) {
        szLine[iIndex] = '\0';
        AnalyzeLineString(szLine, rbLed1, rbLed2, rbLed3, riSpeed);
      }
      currentLineIsBlank = true;
      iIndex = 0;
    } else if (c != '\r') {
      currentLineIsBlank = false;
      if (foundRNRN) {
        if (LINEBUFFERSIZE - 1 > iIndex) {
          szLine[iIndex] = c;
          ++iIndex;
        }
      }
    }
  }
  if (foundRNRN && iIndex != 0) {
    szLine[iIndex] = '\0';
    AnalyzeLineString(szLine, rbLed1, rbLed2, rbLed3, riSpeed);
  }
  return foundRNRN;
}

void loop()
{
  static boolean s_led1 = false;
  static boolean s_led2 = false;
  static boolean s_led3 = false;
  static int s_interval = 0;
  
  EthernetClient client = g_server.available();
  boolean led1 = false;
  boolean led2 = false;
  boolean led3 = false;
  int speed = 5;
  if (readHttpRequest(client, led1, led2, led3, speed))
  {
    PrintHtml(client, led1, led2, led3, speed);
    delay(1);
    client.stop();
    s_led1 = led1;
    s_led2 = led2;
    s_led3 = led3;
    s_interval = (int)(500L + (speed - 1) * (10L - 500L) / (9 - 1));
  }
  if (s_led1) { digitalWrite(LED1PIN, HIGH); }
  if (s_led2) { digitalWrite(LED2PIN, HIGH); }
  if (s_led3) { digitalWrite(LED3PIN, HIGH); }
  delay(s_interval);
  digitalWrite(LED1PIN, LOW);
  digitalWrite(LED2PIN, LOW);
  digitalWrite(LED3PIN, LOW);
  delay(s_interval);
}
