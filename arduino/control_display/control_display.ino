#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI);
  //uint8_t w, uint8_t h, SPIClass *spi_ptr,
    //                               int8_t dc_pin, int8_t rst_pin, int8_t cs_pin,
      //                             uint32_t bitrate)


// states for demo 
int demo_mode = 1;
static const int max_animation_index = 8;
int current_animation_index = 0;



//reference state
int ref_eye_height = 40;
int ref_eye_width = 40;
int ref_space_between_eye = 10;
int ref_corner_radius = 10;
//current state of the eyes
int left_eye_height = ref_eye_height;
int left_eye_width = ref_eye_width;
int left_eye_x = 32;
int left_eye_y = 32;
int right_eye_x = 32+ref_eye_width+ref_space_between_eye;
int right_eye_y = 32;
int right_eye_height = ref_eye_height;
int right_eye_width = ref_eye_width;




void draw_eyes(bool update=true)
{
    display.clearDisplay();        
    //draw from center
    int x = int(left_eye_x-left_eye_width/2);
    int y = int(left_eye_y-left_eye_height/2);
    display.fillRoundRect(x,y,left_eye_width,left_eye_height,ref_corner_radius,SSD1306_WHITE);
    x = int(right_eye_x-right_eye_width/2);
    y = int(right_eye_y-right_eye_height/2);
    display.fillRoundRect(x,y,right_eye_width,right_eye_height,ref_corner_radius,SSD1306_WHITE);    
    if(update)
    {
      display.display();
    }
    
}


void center_eyes(bool update=true)
{
  //move eyes to the center of the display, defined by SCREEN_WIDTH, SCREEN_HEIGHT
  left_eye_height = ref_eye_height;
  left_eye_width = ref_eye_width;
  right_eye_height = ref_eye_height;
  right_eye_width = ref_eye_width;
  
  left_eye_x = SCREEN_WIDTH/2-ref_eye_width/2-ref_space_between_eye/2;
  left_eye_y = SCREEN_HEIGHT/2;
  right_eye_x = SCREEN_WIDTH/2+ref_eye_width/2+ref_space_between_eye/2;
  right_eye_y = SCREEN_HEIGHT/2;
  
  draw_eyes(update);
}

void blink(int speed=12)
{
  draw_eyes();
  
  
  for(int i=0;i<3;i++)
  {
    left_eye_height = left_eye_height-speed;
    right_eye_height = right_eye_height-speed;    
    draw_eyes();
    delay(1);
  }
  for(int i=0;i<3;i++)
  {
    left_eye_height = left_eye_height+speed;
    right_eye_height = right_eye_height+speed;
    
    draw_eyes();
    delay(1);
  }
}


void sleep()
{
  left_eye_height = 2;
  right_eye_height = 2;
  draw_eyes(true);  
}
void wakeup()
{
  
  sleep();
  
  for(int h=0; h <= ref_eye_height; h+=2)
  {
    left_eye_height = h;
    right_eye_height = h;
    draw_eyes(true);
  }
  

}


void happy_eye()
{
  center_eyes(false);
  //draw inverted triangle over eye lower part
  int offset = ref_eye_height/2;
  for(int i=0;i<10;i++)
  {
    display.fillTriangle(left_eye_x-left_eye_width/2-1, left_eye_y+offset, left_eye_x+left_eye_width/2+1, left_eye_y+5+offset, left_eye_x-left_eye_width/2-1,left_eye_y+left_eye_height+offset,SSD1306_BLACK);
    //display.fillRect(left_eye_x-left_eye_width/2-1, left_eye_y+5, left_eye_width+1, 20,SSD1306_BLACK);

    display.fillTriangle(right_eye_x+right_eye_width/2+1, right_eye_y+offset, right_eye_x-left_eye_width/2-1, right_eye_y+5+offset, right_eye_x+right_eye_width/2+1,right_eye_y+right_eye_height+offset,SSD1306_BLACK);
    //display.fillRect(right_eye_x-right_eye_width/2-1, right_eye_y+5, right_eye_width+1, 20,SSD1306_BLACK);  
    offset -= 2;
    display.display();
    delay(1);
  }
  
  
  display.display();
  delay(1000);
}

void saccade(int direction_x, int direction_y)
{
  //quick movement of the eye, no size change. stay at position after movement, will not move back,  call again with opposite direction
  //direction == -1 :  move left
  //direction == 1 :  move right
  
  int direction_x_movement_amplitude = 8;
  int direction_y_movement_amplitude = 6;
  int blink_amplitude = 8;

  for(int i=0;i<1;i++)
  {
    left_eye_x+=direction_x_movement_amplitude*direction_x;
    right_eye_x+=direction_x_movement_amplitude*direction_x;    
    left_eye_y+=direction_y_movement_amplitude*direction_y;
    right_eye_y+=direction_y_movement_amplitude*direction_y;    
    
    right_eye_height-=blink_amplitude;
    left_eye_height-=blink_amplitude;
    draw_eyes();
    delay(1);
  }
  
  for(int i=0;i<1;i++)
  {
    left_eye_x+=direction_x_movement_amplitude*direction_x;
    right_eye_x+=direction_x_movement_amplitude*direction_x;    
    left_eye_y+=direction_y_movement_amplitude*direction_y;
    right_eye_y+=direction_y_movement_amplitude*direction_y;

    right_eye_height+=blink_amplitude;
    left_eye_height+=blink_amplitude;

    draw_eyes();
    delay(1);
  }
  
}

void move_right_big_eye()
{
  move_big_eye(1);
}
void move_left_big_eye()
{
  move_big_eye(-1);
}
void move_big_eye(int direction)
{
  //direction == -1 :  move left
  //direction == 1 :  move right

  int direction_oversize = 1;
  int direction_movement_amplitude = 2;
  int blink_amplitude = 5;

  for(int i=0;i<3;i++)
  {
    left_eye_x+=direction_movement_amplitude*direction;
    right_eye_x+=direction_movement_amplitude*direction;    
    right_eye_height-=blink_amplitude;
    left_eye_height-=blink_amplitude;
    if(direction>0)
    {
      right_eye_height+=direction_oversize;
      right_eye_width+=direction_oversize;
    }else
    {
      left_eye_height+=direction_oversize;
      left_eye_width+=direction_oversize;
    }

    draw_eyes();
    delay(1);
  }
  for(int i=0;i<3;i++)
  {
    left_eye_x+=direction_movement_amplitude*direction;
    right_eye_x+=direction_movement_amplitude*direction;
    right_eye_height+=blink_amplitude;
    left_eye_height+=blink_amplitude;
    if(direction>0)
    {
      right_eye_height+=direction_oversize;
      right_eye_width+=direction_oversize;
    }else
    {
      left_eye_height+=direction_oversize;
      left_eye_width+=direction_oversize;
    }
    draw_eyes();
    delay(1);
  }

  delay(1000);

  for(int i=0;i<3;i++)
  {
    left_eye_x-=direction_movement_amplitude*direction;
    right_eye_x-=direction_movement_amplitude*direction;    
    right_eye_height-=blink_amplitude;
    left_eye_height-=blink_amplitude;
    if(direction>0)
    {
      right_eye_height-=direction_oversize;
      right_eye_width-=direction_oversize;
    }else
    {
      left_eye_height-=direction_oversize;
      left_eye_width-=direction_oversize;
    }
    draw_eyes();
    delay(1);
  }
  for(int i=0;i<3;i++)
  {
    left_eye_x-=direction_movement_amplitude*direction;
    right_eye_x-=direction_movement_amplitude*direction;    
    right_eye_height+=blink_amplitude;
    left_eye_height+=blink_amplitude;
    if(direction>0)
    {
      right_eye_height-=direction_oversize;
      right_eye_width-=direction_oversize;
    }else
    {
      left_eye_height-=direction_oversize;
      left_eye_width-=direction_oversize;
    }
    draw_eyes();
    delay(1);
  }


  center_eyes();


}

void setup() {
  // put your setup code here, to run once:
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  
  
  //for usb communication  
  Serial.begin(115200);
  
  

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  
  // Clear the buffer
  display.clearDisplay();
  
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Intellar.ca"));  
  display.display();  
  delay(2000);
  sleep();
  delay(2000);

  // Draw a single pixel in white
  //display.drawPixel(10, 10, SSD1306_WHITE);
  
  
}



void launch_animation_with_index(int animation_index)
{

  

  if(animation_index>max_animation_index)
  {
    animation_index=8;
  }

  switch(animation_index)
      {
        case 0:
          wakeup();
          break;
        case 1:
          center_eyes(true);
          break;
        case 2:
          move_right_big_eye();
          break;
        case 3:
          move_left_big_eye();
          break;
        case 4:      
          blink(10);
          break;
        case 5:
          blink(20);
          break;
        case 6:
          happy_eye();      
          break;
        case 7:
          sleep();
          break;
        case 8:
          center_eyes(true);
          for(int i=0;i<20;i++)
          { 
            int dir_x = random(-1, 2);
            int dir_y = random(-1, 2);
            saccade(dir_x,dir_y);
            delay(1);
            saccade(-dir_x,-dir_y);
            delay(1);     
          }
          break;
          
      }
}



void loop() {
  // put your main code here, to run repeatedly:

  // put your main code here, to run repeatedly:

  if(demo_mode == 1)
  {
    // cycle animations
    launch_animation_with_index(current_animation_index++);
    if(current_animation_index > max_animation_index)
    {
      current_animation_index = 0;
    }
  }

  
  //send A0 - A5  for animation 0 to 5
  if(Serial.available()) {
    String data = Serial.readString();
    data.trim();
    char cmd = data[0];
    
    
    
    
    if(cmd == 'A')
    {
      demo_mode = 0;

      String arg = data.substring(1,data.length());
      int anim = arg.toInt();
      launch_animation_with_index(anim);
      Serial.print(cmd);
      Serial.print(arg);   
    }

    


  }


}
