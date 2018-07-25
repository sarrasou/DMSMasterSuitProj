void lcdRefresh() {
  //  lcd.setCursor(0, 0);
  lcd.clear();
  int offset1 = (16 - strlen(lcdMessage1)) / 2;
  lcd.setCursor(offset1, 0);
  lcd.print(lcdMessage1);
  int offset2 = (16 - strlen(lcdMessage2)) / 2;
  lcd.setCursor(offset2, 1);
  lcd.print(lcdMessage2);
}

