#include <SoftwareSerial.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>

SoftwareSerial BTSerial(12, 11);

extern void loop2();
extern void loop3();

void setup()
{
  // put your setup code here, to run once:
  BTSerial.begin(9600);
  Serial.begin(9600);

  Serial.println("Start!");
}

// Start Utility

/**
 * Ref: https://stackoverflow.com/questions/9210528/split-string-with-delimiters-in-c
 */
char **str_split(char *a_str, const char a_delim)
{
  char **result = 0;
  size_t count = 0;
  char *tmp = a_str;
  char *last_comma = 0;
  char delim[2];
  delim[0] = a_delim;
  delim[1] = 0;

  /* Count how many elements will be extracted. */
  while (*tmp)
  {
    if (a_delim == *tmp)
    {
      count++;
      last_comma = tmp;
    }
    tmp++;
  }

  /* Add space for trailing token. */
  count += last_comma < (a_str + strlen(a_str) - 1);

  /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
  count++;

  result = malloc(sizeof(char *) * count);

  if (result)
  {
    size_t idx = 0;
    char *token = strtok(a_str, delim);

    while (token)
    {
      assert(idx < count);
      *(result + idx++) = strdup(token);
      token = strtok(0, delim);
    }
    assert(idx == count - 1);
    *(result + idx) = 0;
  }

  return result;
}

/**
 * Ref: https://stackoverflow.com/questions/4770985/how-to-check-if-a-string-starts-with-another-string-in-c
 */
bool startsWith(const char *pre, const char *str)
{
  size_t lenpre = strlen(pre),
        lenstr = strlen(str);
  return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}

// END Utility

boolean disic = false;
boolean disic_ok = false;

//void identify_device(String device) {
//  if (startsWith("OK+DISC", device)) {
//    disic_ok = true;
//
//  }
//}

boolean disi_ok = false;
boolean disi = false;

void list_devices(String response)
{
  if (disi_ok)
  {
    // if it's not end yet.
    if (response == String("OK+DISCE"))
    {
      disi = false;
      disi_ok = false;
//      break;
    }

//    identify_device(response);
  }

  if (!disi_ok) {
    if (response != String("OK+DISIS"))
      disi_ok = true;
  }
}

boolean bluetooth_in = false;
String response = "";

boolean serial_in = false;
String request = "";

int every = 5;
int countTime = 0;

void loop2() {
  delay(1000);
  Serial.println("Loop2");
}

void loop3() {
  delay(500);
  Serial.println("Loop3");
}

int count = 0;
long lastExecute = millis();
long lastExecute2 = millis();

void loop()
{
  const int REFRESH_INTERVAL = 1000;
  long current = millis();
  
  if (current - lastExecute >= REFRESH_INTERVAL) {
    lastExecute = current;
    Serial.println("I will print every 1s");
  }

  if (current - lastExecute2 >= 5000) {
    lastExecute2 = current;
    BTSerial.print("AT+DISI?");
//    Serial.println("I will print every 0.5s");
  }

  if (BTSerial.available() > 0)
  {
    bluetooth_in = true;

    char c = ' ';
    c = BTSerial.read();
    response += c;
    delay(1);
    //    Serial.print(c);
  }
  else if (bluetooth_in)
  {
    bluetooth_in = false;

    if (disi)
    {
      list_devices(response);
    }

    Serial.println(response);
    response = "";
  }

  if (Serial.available() > 0)
  {
    serial_in = true;
    char c = ' ';
    c = Serial.read();

    // BTSerial.print(c);
    request += c;
    delay(1);
  }
  else if (serial_in)
  {
    serial_in = false;

    if (request == String("AT+DISI?"))
    {
      disi = true;
    }
    BTSerial.print(request);

    request = "";
  }
}
