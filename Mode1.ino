void Game() {
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("level:");
  display.print(level);

  display.setCursor(55, 0);
  display.print(minute / 10);
  display.print(minute % 10);
  display.print(":");
  display.print(second / 10);
  display.print(second % 10);

  for (int shdw = 0; shdw < 127; shdw += 2) {
    display.drawPixel(shdw, 8, WHITE);
  }
  display.drawLine(0, 9, 127, 9, WHITE);

  if (posx == MAZEHEIGHT + 1) {
    timerAlarmDisable(My_timer);
    timerOn = false;
    score = 100 - ((minute * 1.6) + (second * 0.02));
    level++;
    displayLevel();
    minute = 0;
    second = 0;
    posx = 0;
    posy = 2;
    illuminatedRow = 0;
    blinkPlayer = 1;
    wallPhase = 1;
    selectedOption = 1;
  } else {
    drawMaze();
    if (gamePause) {
      gameMenu();
      if (Deboundce(ButtonA)) {
        if (selectedOption == 1) {
          gamePause = false;
        }
        if (selectedOption == 2) {
          ESP.restart();  //jmp 0
        }
      }
    }
  }

  if (!gamePause) {
    prevTimeSleep = millis();
    if (!timerOn) {
      timerAlarmEnable(My_timer);
      timerOn = true;
    }

    if (Deboundce(ButtonM)) {
      gamePause = true;
      if (timerOn) {
        timerAlarmDisable(My_timer);
        timerOn = false;
      }
    }
  }

}
