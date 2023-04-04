// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
//    Serial.println(text);

    String from_name = bot.messages[i].from_name;
    if (text == "OK") {
       Serial.println("OK");
       String textMsg = "Your reservation has been accepted.";
       sendMail(textMsg);
       sendSMS(inputNumber, textMsg);

    }
    
    if (text == "NO") {
      Serial.println("NO");
      String textMsg = "Your reservation has been rejected.";
      sendMail(textMsg);
       sendSMS(inputNumber, textMsg);
    }
  }
}
