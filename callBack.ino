//this is where the shit happens . . .

void callback(char* topic, byte* payload, unsigned int length) {

  if (!client.connected()) {
    reconnect();
  }
  client.loop(); //this keeps the client alive (MQTT)

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    incomingMessage[i] = (char)payload[i];
  }
  Serial.println();

  StaticJsonBuffer<300>  jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(incomingMessage);

  if (!root.success()) {
    Serial.print(topic);
    root.printTo(Serial);
    Serial.println("parseObject() failed");
    return;
  }

  ///////////////neoPixelColor
  if (strcmp(topic, "dmsuits/suit3/color") == 0) {
    Serial.println("A message has arrived from the Department of Suits");
    colorCode = root["color"].as<int>();
  }

  /////////////LCDmessages
  if (strcmp(topic, "dmsuits/suit3/lcd") == 0) {
    lcdMessage1 = root["lcd1"].as<const char*>();
    lcdMessage2 = root["lcd2"].as<const char*>();
    lcdRefresh();
  }

  //////////////vibrations
  if (strcmp(topic, "dmsuits/suit3/vib") == 0) {
    vibrateCount = root["vib"].as<int>();
  }
}
