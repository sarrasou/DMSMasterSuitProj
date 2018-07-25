void vibrate() {
  if (currentMillis - vibTimer > vibInterval && vibrateCount > 0) {
    if (vibState) {
//      Serial.println("hi");
      digitalWrite(vibPin, HIGH);
    }
    if (!vibState) {
//      Serial.println("lo");
      digitalWrite(vibPin, LOW);
      vibrateCount--;
    }
    vibState = !vibState;
    vibTimer = millis();
//    Serial.println(vibrateCount);
  }

}

