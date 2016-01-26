
#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>


#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

#define YP A2  // must be an analog pin, use "An" notation!
#define XM A1  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin

// Calibrate values
#define TS_MINX 125
#define TS_MINY 85
#define TS_MAXX 965
#define TS_MAXY 905

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 100);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

// Assign human-readable names to some common 16-bit color values:
#define  WHITE   0x0000
#define YELLOW    0x001F
#define GREEN     0xF800
#define MAGENTA   0x07E0
#define RED    0x07FF
#define CYAN 0xF81F
#define BLUE  0xFFE0
#define BLACK   0xFFFF


Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define BOXSIZE 40
#define PENRADIUS 3

//Variables:

int tabel2D[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int long counter;
int turn = 0;
int win = 0;

void setup(void) {
  Serial.begin(9600);
  tft.reset();
  tft.begin(0x8357); // SDFP5408
  tft.setRotation(2); // Need for the Mega, please changed for your choice or rotation initial
  drawBorder();//draw border
 
  // Initial screen
  tft.setCursor (90, 50);
  tft.setTextSize (3);
  tft.setTextColor(BLACK);
  tft.println("Tic");
  tft.setCursor (90, 85);
  tft.println("Tac");
  tft.setCursor (90, 120);
  tft.println("Toe");
  tft.setCursor (80, 250);
  tft.setTextSize (1);
  tft.setTextColor(BLACK);
  tft.println("Touch to Start");
  waitOneTouch(); // Wait touch
  tft.fillScreen(WHITE);
 
 //dots in the cornet of the screen
 tft.fillRect(0,0,10,10,RED);
 tft.fillRect(0,310,10,10,RED);
 tft.fillRect(230,0,10,10,RED);
 tft.fillRect(230,310,10,10,RED);

 //the battle-ground
 tft.fillRect(80 , 20 , 10 , 280, BLACK);
 tft.fillRect(160 , 20 , 10 , 280, BLACK);
 tft.fillRect(20, 106, 200, 10, BLACK);
 tft.fillRect(20, 212, 200, 10, BLACK);
 
 pinMode(13, OUTPUT);
}

#define MINPRESSURE 10
#define MAXPRESSURE 1000

void loop()
{
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
//check for pressure
if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    counter++;
      Serial.print("X = "); Serial.print(p.x);
      Serial.print("\tY = "); Serial.print(p.y);
      Serial.print("\tPressure = "); Serial.println(p.z);
  if(counter > 30){
    if(win == 0){
      counter = 0;
      checktouch(413 , 352 , 243 , 169 , p.x , p.y , 0 , 0 , 20 , 20);
      checktouch(660 , 352 , 456 , 169 , p.x , p.y , 1 , 0 , 100 , 20);
      checktouch(866 , 352 , 722 , 169 , p.x , p.y , 2 , 0 , 180 , 20);
      checktouch(406 , 618 , 216 , 406 , p.x , p.y , 3 , 1 , 20 , 126);
      checktouch(622 , 618 , 406 , 392 , p.x , p.y , 4 , 1 , 100 , 126);
      checktouch(871 , 612 , 733 , 383 , p.x , p.y , 5 , 1 , 180 , 126);
      checktouch(415 , 841 , 207 , 670 , p.x , p.y , 6 , 2 , 20 , 232);
      checktouch(672 , 841 , 457 , 661 , p.x , p.y , 7 , 2 , 100 , 232);
      checktouch(886 , 837 , 729 , 653 , p.x , p.y , 8 , 2 , 180 , 232);
    }
  
    wincheck();
   }
  }
}

TSPoint waitOneTouch() {
  TSPoint p;
   do {
    p= ts.getPoint(); 
    pinMode(XM, OUTPUT); //Pins configures again for TFT control
    pinMode(YP, OUTPUT);
    } while((p.z < MINPRESSURE )|| (p.z > MAXPRESSURE));
   return p;
}

void drawBorder () {

  // Draw a border

  uint16_t width = tft.width() - 1;
  uint16_t height = tft.height() - 1;
  uint8_t border = 10;

  tft.fillScreen(BLUE);
  tft.fillRect(border, border, (width - border * 2), (height - border * 2), WHITE);
}

void drawX(int x,int y){
  tft.setCursor (x, y);
  tft.setTextSize (10);
  tft.setTextColor(BLACK);
  tft.print("X");
  //delay(1000); 
}

void drawO(int x,int y){
  tft.setCursor (x, y);
  tft.setTextSize (10);
  tft.setTextColor(BLACK);
  tft.print("O");
  //delay(1000); 
}

void checktouch(int xmax, int ymax, int xmin, int ymin, int px , int py , int i, int j, int xdraw, int ydraw)
{Serial.println("AM INTRAT");
if((px > xmin && px < xmax)&&(py > ymin && py < ymax))
    {Serial.println("AM Verificat");
      if(turn%2 == 0)
        {Serial.println("AM ales X");
        if(tabel2D[i] != 1 && tabel2D[i] != 2)
          {
          drawX(xdraw,ydraw);
          tabel2D[i] = 1;
          turn++;
          Serial.println("X");
          }
        }
      else
        if(turn%2 != 0)
        {Serial.println("AM ales O");
          if(tabel2D[i] != 1 && tabel2D[i] != 2)
          {
            drawO(xdraw,ydraw);
            tabel2D[i] = 2;
            turn++;
            Serial.println("O");
          }
        }
    }
}

void wincheck(){
    Win(addline(0,1,2));Serial.println(addline(0,1,2));
    Win(addline(2,5,8));Serial.println(addline(2,5,8));
    Win(addline(8,7,6));Serial.println(addline(8,7,6));
    Win(addline(0,3,6));Serial.println(addline(6,3,0));
    Win(addline(0,4,8));Serial.println(addline(0,4,8));
    Win(addline(2,4,6));Serial.println(addline(2,4,6));
    Win(addline(1,4,7));Serial.println(addline(2,4,6));
    Win(addline(3,4,5));Serial.println(addline(2,4,6));
    
}

int addline(int a, int b, int c){Serial.println("AM INTRAT IN ADDLINE");
  if((tabel2D[a] == tabel2D[b])&&(tabel2D[a] == tabel2D[c])&&(tabel2D[a] == 1))
    {return 1;Serial.println("AM TRIMIS 1");}
  if((tabel2D[a] == tabel2D[b])&&(tabel2D[a] == tabel2D[c])&&(tabel2D[a] == 2))
    {return 2;Serial.println("AM TRIMIS 2");}
   else
    {return 0;Serial.println("AM TRIMIS 0");}
}

void Win(int A){
  if(A == 1){Serial.println("AM PRIMIT " + A);
    tft.fillScreen(WHITE);
    win = 1;
    tft.setCursor (0, 50);
    tft.setTextSize (3);
    tft.setTextColor(BLACK);
    tft.println("The winner is ");
    tft.setCursor (100, 100);
    tft.println("X");
    tft.setCursor (100, 310);
    tft.setTextSize (1);
    tft.println("Created by Cosmin Dragu");
  }
  if(A == 2){
    tft.fillScreen(WHITE);
    win = 1;
    tft.setCursor (0, 50);
    tft.setTextSize (3);
    tft.setTextColor(BLACK);
    tft.println("The winner is ");
    tft.setCursor (100, 100);
    tft.println("O");
    tft.setCursor (100, 310);
    tft.setTextSize (1);
    tft.println("Created by Cosmin Dragu");
  }
}
