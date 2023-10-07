uint8_t Player_x = 64;
uint8_t Player_y = 32;
char state = 'S';
unsigned long prevTimeButton = 0;
unsigned long prevTimeDeboundce = 0;
bool wall;


void ReadJoy() {
  int readX, readY, x, y;
  x = analogRead(X_pin);
  y = analogRead(Y_pin);

  readX = map(x, 0, 4096, 0, 3);
  readY = map(y, 0, 4096, 0, 3);

  if (readX != 1 || readY != 1) {
    if (readY == 0) {
      state = 'U';
    } else if (readY == 2) {
      state = 'D';
    } else if (readX == 0) {
      state = 'L';
    } else if (readX == 2) {
      state = 'R';
    }
  }
}

bool Deboundce(int Pin) {
  static unsigned long current_time;
  char sw;
  sw = digitalRead(Pin);
  if (sw) {
    return false;
  } else {
    current_time = millis();
    if ((current_time - prevTimeDeboundce) > 1000) {
      prevTimeDeboundce = millis();
      return true;
    } else {
      prevTimeDeboundce = millis();
      return false;
    }
  }
}


void controller() {
  if ((millis() - prevTimeButton) > 200) {
    prevTimeButton = millis();
    ReadJoy();
  }
  // Serial.println(state);

  if (gamePause) {
    if (state == 'D' || state == 'U' || state == 'L' || state == 'R') {
      prevTimeSleep = millis();
    }
    
    if (state == 'D') {
      selectedOption++;
      if (selectedOption > 2) {
        selectedOption = 1;
      }
      state = 'S';
      delay(30);
    }

    if (state == 'U') {
      selectedOption--;
      if (selectedOption < 1) {
        selectedOption = 2;
      }
      state = 'S';
      delay(30);
    }


  } else {
    if (state == 'L') {
      if (posx - 1 >= 0) {
        wall = readPixel(posx - 1, posy);
        if (!wall) {
          --posx;
        }
      }

      state = 'S';
    }

    if (state == 'R') {
      if (posx + 1 <= MAZEHEIGHT + 1) {
        wall = readPixel(posx + 1, posy);
        if (!wall) {
          ++posx;
        }
      }

      state = 'S';
    }

    if (state == 'U') {
      if (posy - 1 >= 2) {
        wall = readPixel(posx, posy - 1);
        if (!wall) {
          --posy;
        }
      }

      state = 'S';
    }

    if (state == 'D') {
      if (posy + 1 <= MAZEWIDTH) {
        wall = readPixel(posx, posy + 1);
        if (!wall) {
          ++posy;
        }
        state = 'S';
      }
    }
  }
}
