#include "LedControl.h"
LedControl lc = LedControl(10, 8, 9, 1);

int w = 8;
boolean world[8][8];
int delaytime = 10;
int deathDelay = 2000;
int stasisDelay = 8000;

boolean dead = false;
boolean stasis = false;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));

  lc.shutdown(0, false);
  lc.setIntensity(0, 0.1);
  lc.clearDisplay(0);

  initialize();
}

void loop() {
  if (dead) {
    delay(deathDelay);
    initialize();
  } else if (stasis) {
    delay(stasisDelay);
    initialize();
  } else {
    generate();
  }

  printWorld();
  show();

  delay(delaytime);
}

void initialize() {
  for (int x = 0; x < w; x++) {
    for (int y = 0; y < w; y++) {
      world[x][y] = random(2);
    }
  }
  dead = false;
  stasis = false;
}

void generate() {
  boolean newWorld[8][8];
  int stasisCount = 0;
  int liveDots = 0;
  for (int x = 0; x < w; x++) {
    for (int y = 0; y < w; y++) {
      int neighbors = 0;
      for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
          int xIndex = x + i;
          int yIndex = y + j;
          if (xIndex >= 0 && xIndex < w && yIndex >= 0 && yIndex < w) {
            neighbors += world[x + i][y + j];
          }
        }
      }

      neighbors -= world[x][y];

      if ((world[x][y] == 1) && (neighbors <  2)) {
        newWorld[x][y] = 0;           // Loneliness
      } else if ((world[x][y] == 1) && (neighbors >  3)) {
        newWorld[x][y] = 0;           // Overpopulation
      } else if ((world[x][y] == 0) && (neighbors == 3)) {
        newWorld[x][y] = 1;           // Reproduction
      } else {
        newWorld[x][y] = world[x][y];  // Stasis
        stasisCount++;
      }

      liveDots += world[x][y];
    }
  }

  dead = (liveDots == 0);
  stasis = (stasisCount == 64);  

  for (int x = 0; x < w; x++) {
    for (int y = 0; y < w; y++) {
      world[x][y] = newWorld[x][y];
    }
  }
}

void show() {
  lc.clearDisplay(0);

  for (int x = 0; x < w; x++) {
    for (int y = 0; y < w; y++) {
      lc.setLed(0, x, y, world[x][y]);
    }
  }
}

void printWorld() {
  for (int x = 0; x < w; x++) {
    for (int y = 0; y < w; y++) {
      Serial.print(world[x][y]);
      Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println();
}


