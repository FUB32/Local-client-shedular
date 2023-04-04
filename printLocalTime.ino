void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }

  char dayOfWeek[10];
  strftime(dayOfWeek,10, "%A", &timeinfo);
  char monthOfYear[10];
  strftime(monthOfYear,10, "%B", &timeinfo);
  char dayOfMonth[10];
  strftime(dayOfMonth,10, "%d", &timeinfo);
  char hour[10];
  strftime(hour,10, "%H", &timeinfo);
  char minutes[10];
  strftime(minutes,10, "%M", &timeinfo);
  
  String input="Name: "+inputName+" - "+"Number: "+inputNumber+" - Date: ";
  String date=String(dayOfWeek)+"  "+String(dayOfMonth)+" / "+String(monthOfYear)+"  "+String(hour)+":"+String(minutes);
  
  bot.sendMessage(CHAT_ID, input+date, "");
  
}
