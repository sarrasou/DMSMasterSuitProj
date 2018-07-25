void readAccs() {
  lis1.read(); //read the accel data at each loop
  lis2.read(); //read the accel data at each loop

  sensors_event_t event;

  lis1.getEvent(&event);
  lis2.getEvent(&event);

  smoothData();
}

void smoothData() {

  totX1 = totX1 - x1reads[readIndex];
  // read from the sensor:
  x1reads[readIndex] = lis1.x;
  // add the reading to the total:
  totX1 = totX1 + x1reads[readIndex];

  totY1 = totY1 - y1reads[readIndex];
  // read from the sensor:
  y1reads[readIndex] = lis1.y;
  // add the reading to the total:
  totY1 = totY1 + y1reads[readIndex];

  totZ1 = totZ1 - z1reads[readIndex];
  // read from the sensor:
  z1reads[readIndex] = lis1.z;
  // add the reading to the total:
  totZ1 = totZ1 + z1reads[readIndex];


  totX2 = totX2 - x2reads[readIndex];
  // read from the sensor:
  x2reads[readIndex] = lis2.x;
  // add the reading to the total:
  totX2 = totX2 + x2reads[readIndex];

  totY2 = totY2 - y2reads[readIndex];
  // read from the sensor:
  y2reads[readIndex] = lis2.y;
  // add the reading to the total:
  totY2 = totY2 + y2reads[readIndex];

  totZ2 = totZ2 - z2reads[readIndex];
  // read from the sensor:
  z2reads[readIndex] = lis2.z;
  // add the reading to the total:
  totZ2 = totZ2 + z2reads[readIndex];

  // advance to the next position in the array:
  readIndex = readIndex + 1;
  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  avgX1 = totX1 / numReadings;
  avgY1 = totY1 / numReadings;
  avgZ1 = totZ1 / numReadings;
  avgX2 = totX2 / numReadings;
  avgY2 = totY2 / numReadings;
  avgZ2 = totZ2 / numReadings;
}
