void sendSMS(String number, String message)
{
  Serial.println("Sending SMS...");               //Show this message on serial monitor
  Serial2.print("AT+CMGF=1\r");                   //Set the module to SMS mode
  delay(100);
  Serial2.print("AT+CMGS=\"+964"+number+"\"\r");  //Your phone number don't forget to include your country code, example +212123456789"
  Serial.print("AT+CMGS=\"+964"+number+"\"\r");
  delay(500);
  Serial2.print(message);       //This is the text to send to the phone number, don't make it too long or you have to modify the SoftwareSerial buffer
  delay(1500);
  Serial2.print((char)26);// (required according to the datasheet)
  delay(500);
  Serial2.println();
  Serial.println("Text Sent.");
  delay(500);

}
