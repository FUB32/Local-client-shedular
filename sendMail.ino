void sendMail(String textMsg){
  if(resEmail =="")
    {
      Serial.println("no Email");
      return;
    }
  message.addRecipient("fub", resEmail);
if (!smtp.connect(&session))
    return;
message.text.content = textMsg.c_str();
message.text.charSet = "us-ascii";
message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  message.addRecipient("fub", resEmail);
if (!smtp.connect(&session))
    return;
        if (!MailClient.sendMail(&smtp, &message))
            Serial.println("Error sending Email, " + smtp.errorReason());
}
