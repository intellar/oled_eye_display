#include <SPI.h>
#include <U8g2lib.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


//be sure to initialize with your display.  see https://github.com/olikraus/u8g2/wiki/setup_tutorial 
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

int COLOR_WHITE = 1;
int COLOR_BLACK = 0;


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
int corner_radius = ref_corner_radius;



void display_clearDisplay()
{
  u8g2.clearBuffer();   
    
}
void display_fillRoundRect(int x,int y,int w, int h, int r, int color)
{
  u8g2.setDrawColor(color);
  
  //behavior is not defined if r is smaller than the height or width,
  if( w<2*(r+1) )
  {
    r = (w/2)-1;
  }
  if( h<2*(r+1) )
  {
    r = (h/2)-1;
  }
  //check if height and width are valid when calling drawRBox
  u8g2.drawRBox(x,y,w<1?1:w,h<1?1:h,r);
}
void display_display()
{
  u8g2.sendBuffer();
}
void display_fillTriangle(int x0,int y0, int x1,int y1,int x2,int y2, int color)
{
  u8g2.setDrawColor(color);
  u8g2.drawTriangle(x0,y0,x1,y1,x2,y2);
}


void draw_eyes(bool update=true)
{
    display_clearDisplay();        
    //draw from center
    int x = int(left_eye_x-left_eye_width/2);
    int y = int(left_eye_y-left_eye_height/2);
    display_fillRoundRect(x,y,left_eye_width,left_eye_height,corner_radius,COLOR_WHITE);
    x = int(right_eye_x-right_eye_width/2);
    y = int(right_eye_y-right_eye_height/2);
    display_fillRoundRect(x,y,right_eye_width,right_eye_height,corner_radius,COLOR_WHITE);    
    if(update)
    {
      display_display();
    }
    
}


void reset_eyes(bool update=true)
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

  corner_radius = ref_corner_radius;
  
  draw_eyes(update);
}

void blink(int speed=12)
{
  reset_eyes(false);

  draw_eyes();
  
  
  for(int i=0;i<3;i++)
  {
    left_eye_height = left_eye_height-speed;
    right_eye_height = right_eye_height-speed;

    left_eye_width = left_eye_width+3;
    right_eye_width = right_eye_width+3;
    draw_eyes();
    delay(1);
  }
  for(int i=0;i<3;i++)
  {
    left_eye_height = left_eye_height+speed;
    right_eye_height = right_eye_height+speed;
    left_eye_width = left_eye_width-3;
    right_eye_width = right_eye_width-3;
    draw_eyes();
    delay(1);
  }
}



void sleep()
{
  reset_eyes(false);

  left_eye_height = 2;
  left_eye_width = ref_eye_width;
  right_eye_height = 2;
  right_eye_width = ref_eye_width;

  
  corner_radius = 0;
  draw_eyes(true);  

  
  
  
}
void wakeup()
{
  reset_eyes(false);

  sleep();
  
  for(int h=2; h <= ref_eye_height; h+=2)
  {
    left_eye_height = h;
    right_eye_height = h;
    corner_radius = h;
    if( corner_radius > ref_corner_radius)
    {
      corner_radius = ref_corner_radius;
    }
    draw_eyes(true);
  }
  

}


void happy_eye()
{
  reset_eyes(false);
  //draw inverted triangle over eye lower part
  int offset = ref_eye_height/2;
  for(int i=0;i<10;i++)
  {
    display_fillTriangle(left_eye_x-left_eye_width/2-1, left_eye_y+offset, left_eye_x+left_eye_width/2+1, left_eye_y+5+offset, left_eye_x-left_eye_width/2-1,left_eye_y+left_eye_height+offset,COLOR_BLACK);    
    display_fillTriangle(right_eye_x+right_eye_width/2+1, right_eye_y+offset, right_eye_x-left_eye_width/2-2, right_eye_y+5+offset, right_eye_x+right_eye_width/2+1,right_eye_y+right_eye_height+offset,COLOR_BLACK);
    offset -= 2;
    display_display();
    delay(1);
  }
  
  
  display_display();
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


  reset_eyes();


}

void setup() {
  // put your setup code here, to run once:
  u8g2.setI2CAddress(0x78);
  u8g2.setDisplayRotation(U8G2_R0);
  u8g2.begin();

  sleep();
  u8g2.setFont(u8g2_font_ncenB10_tr);
  
  u8g2.drawStr(0,10,"intellar.ca");
  
  display_display();
  delay(3000);
  
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
          reset_eyes(true);
          break;
        case 2:
          move_right_big_eye();
          break;
        case 3:
          move_left_big_eye();
          break;
        case 4:      
          blink(12);
          delay(1000);
          break;
        case 5:
          blink(12);
          break;
        case 6:
          happy_eye();      
          break;
        case 7:
          sleep();
          break;
        case 8:
          break;
          reset_eyes(true);
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

  if(demo_mode == 1)
  {
    // cycle animations
    launch_animation_with_index(current_animation_index++);
    if(current_animation_index > max_animation_index)
    {
      current_animation_index = 0;
    }
  }

  
  //send "Ax" for animation x  (ex. A2 will launch animation 2)
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
