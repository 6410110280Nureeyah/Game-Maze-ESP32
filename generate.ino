uint8_t geni, genj, genk, genval, genmod;
int8_t genx, geny;

RTC_DATA_ATTR uint16_t Maze[] = {
  0b1101111111111111,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1111111111111101,
  0b0000000000000000
};

bool readPixel(uint8_t i, uint8_t j) {
  uint16_t Data = Maze[i];
  byte Hi = (Data & 0xFF00) >> 8;
  byte Lo = Data & 0x00FF;
  if (j > 7) {
    return bitRead(Lo, 7 - (j % 8));
  } else {
    //j=j-8;
    return bitRead(Hi, 7 - (j % 8));
  }
}

void drawMaze() {
  uint8_t i, j;
  bool wall;

  for (i = 0; i <= MAZEHEIGHT; i++) {
    for (j = 0; j <= MAZEWIDTH; j++) {
      wall = readPixel(i, j);
      if (wall and j > 1) {
        if (wallPhase == 1) {
          if (illuminatedRow >= i) {
            display.drawPixel(i * 4, j * 4 + 2, WHITE);
            display.drawPixel(i * 4, j * 4 + 2 + 2, WHITE);
            display.drawPixel(i * 4 + 1, j * 4 + 1 + 2, WHITE);
            display.drawPixel(i * 4 + 1, j * 4 + 3 + 2, WHITE);
            display.drawPixel(i * 4 + 2, j * 4 + 2 + 2, WHITE);
            display.drawPixel(i * 4 + 2, j * 4 + 4 + 2, WHITE);
            display.drawPixel(i * 4 + 3, j * 4 + 1 + 2, WHITE);
            display.drawPixel(i * 4 + 3, j * 4 + 3 + 2, WHITE);
          } else {
            display.drawPixel(i * 4 + 2, j * 4 + 2 + 2, WHITE);
          }
        } else if (wallPhase == 2) {
          if (illuminatedRow >= i) {
            display.fillRect(i * 4, j * 4 + 2, 4, 4, WHITE);
          } else {
            //display.drawRect(i*4,j*4+1,4,4,WHITE);
            display.drawPixel(i * 4, j * 4 + 2, WHITE);
            display.drawPixel(i * 4, j * 4 + 2 + 2, WHITE);
            display.drawPixel(i * 4 + 1, j * 4 + 1 + 2, WHITE);
            display.drawPixel(i * 4 + 1, j * 4 + 3 + 2, WHITE);
            display.drawPixel(i * 4 + 2, j * 4 + 2 + 2, WHITE);
            display.drawPixel(i * 4 + 2, j * 4 + 4 + 2, WHITE);
            display.drawPixel(i * 4 + 3, j * 4 + 1 + 2, WHITE);
            display.drawPixel(i * 4 + 3, j * 4 + 3 + 2, WHITE);
          }
        } else if (wallPhase == 3) {
          display.fillRect(i * 4, j * 4 + 2, 4, 4, WHITE);
        }
      }
    }
  }

  if(blinkPlayer==1){
    display.fillCircle(posx*4+1,posy*4+1+2,3,WHITE);
    display.fillCircle(posx*4+1,posy*4+1+2,1,BLACK);
  }else{
    display.fillCircle(posx*4+1,posy*4+1+2,3,BLACK);
    display.fillCircle(posx*4+1,posy*4+1+2,1,WHITE);
  }

  blinkPlayer++;
  if(blinkPlayer>16){
    blinkPlayer=1;
  }

  if (illuminatedRow < MAZEHEIGHT + 1) {
    illuminatedRow++;
  } else {
    if (wallPhase < 3) {
      wallPhase++;
      if (wallPhase == 2) {
        illuminatedRow = 0;
      }
    }
  }
}

void generateMaze() {
  bool alternate = false;
  for (geni = 1; geni < MAZEHEIGHT - 1; geni++) {
    if (!alternate) {
      Maze[geni] = 0b1100000000000001;
    } else {
      Maze[geni] = 0b1101010101010101;
    }
    alternate = !alternate;
  }

  genmod = 4;
  for (geni = 2; geni < MAZEHEIGHT - 2; geni += 2) {
    for (genj = 3; genj < MAZEWIDTH - 1; genj += 2) {

      Maze[geni] |= (0x8000 >> genj);

      do {
        randomSeed(11);
        genval = (uint8_t)(random(10000) & 0x00FF) % genmod;
        genx = 0, geny = 0;
        if (genval == 0) geny = 1;   //right
        if (genval == 1) genx = -1;  //up
        if (genval == 2) genx = 1;   //down
        if (genval == 3) geny = -1;  //left
      } while ((Maze[geni + geny] & (0x8000 >> (genj + genx))));
      Maze[geni + geny] |= (0x8000 >> (genj + genx));
    }
    genmod = 3;
  }
}