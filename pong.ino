#define JOYSTICK_1    A0
#define JOYSTICK_2    A1

#define SERIAL_DATA_1  2
#define LATCH          3
#define CLK_1          4
#define SERIAL_DATA_2  5
#define CLK_2          6

typedef struct t_DISPLAY{
  const int pin_display;
  int score;
};

const int leds_7seg [5] = {7,8,9,10,11};
t_DISPLAY t_display [2] = {{12,0},{13,0}};

int i = 0;

typedef struct t_PADDLE {
  byte  paddle_In;
  byte paddle_Out;
  int    position;
  int       value;
};

typedef struct t_BALL {
  byte position_X;
  byte position_Y;
  int       dir_X;
  int       dir_Y;
  int virtual_pos;

};

t_PADDLE paddle[2] = {{B00011100,B01111111,3,0},{B00011100,B11111110,3,0}};
t_BALL ball = {B00001000,B11110111,1,1,3};

void setup() {
  // put your setup code here, to run once:
  pinMode(JOYSTICK_1,INPUT);
  pinMode(JOYSTICK_2,INPUT);

  pinMode(SERIAL_DATA_1,OUTPUT);
  pinMode(LATCH,OUTPUT);
  pinMode(CLK_1,OUTPUT);
  pinMode(SERIAL_DATA_2,OUTPUT);
  pinMode(LATCH,OUTPUT);
  pinMode(CLK_2,OUTPUT);

  for(int index = 0; index < 5; index++){
    pinMode(leds_7seg[index],OUTPUT);
  }
  pinMode(t_display[0].pin_display,OUTPUT);
  pinMode(t_display[1].pin_display,OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  paddle[0].value = analogRead(JOYSTICK_1);
  movePaddle(&paddle[0]);
  printBoard(paddle[0].paddle_In,paddle[0].paddle_Out);
  paddle[1].value = analogRead(JOYSTICK_2);
  movePaddle(&paddle[1]);
  printBoard(paddle[1].paddle_In,paddle[1].paddle_Out);

  moveBall(&ball);
  printBoard(ball.position_X,ball.position_Y);
  collide(&ball);
  delay(100);
  updateScore();
  if(isWin()){
    delay(50);
    exit(0);
  }
}

void reset_game(t_PADDLE *paddle_1,t_PADDLE *paddle_2,t_BALL *ball){
  paddle_1->paddle_In = B00011100;
  paddle_1->paddle_Out = B01111111;
  paddle_1->position = 3;
  paddle_1->value = 0;
  paddle_2->paddle_In = B00011100;
  paddle_2->paddle_Out = B11111110;
  paddle_2->position = 3;
  paddle_2->value = 0;
  ball->position_X = B00001000;
  ball->position_Y = B11110111;
  ball->dir_X = 1;
  ball->dir_Y = 1;
  ball->virtual_pos = 3;
  delay(500);
}

void printBoard(byte paddle_in, byte paddle_out){
  digitalWrite(LATCH,LOW);  
  shiftOut(SERIAL_DATA_1,CLK_1,MSBFIRST,paddle_in);
  shiftOut(SERIAL_DATA_2,CLK_2,MSBFIRST,paddle_out);
  digitalWrite(LATCH,HIGH);
}
void movePaddle(t_PADDLE *paddle){
  if(paddle->value >= 550 && paddle->position+1 < 7){
    paddle->paddle_In *=2;
    paddle->position += 1;
  } else if (paddle->value <= 450 && paddle->position-1 > 0) {
    paddle->paddle_In /=2;
    paddle->position -=1;
  }
}
/**----------------------------------------------------------------------------------------------------------------------------------------------*/
void moveBall(t_BALL *ball){
    ball->position_X *= pow(2,ball->dir_X);
    ball->virtual_pos += 1 * ball->dir_X;
    byte tmp = ~(ball->position_Y);
    tmp *= pow(2,ball->dir_Y);
    ball->position_Y = ~(tmp) ;
}
void collide(t_BALL *ball) {

  if (collideWall())
    ball->dir_X = -ball->dir_X;
  if (collidePaddle())
    ball->dir_Y = -ball->dir_Y;
}
bool collideWall(){
  return (ball.position_X >= B10000000) || (ball.position_X <= B00000001);
}
bool collidePaddle() {
  return (ball.position_Y <= B10111111 && ball.virtual_pos >= paddle[0].position - 1 && ball.virtual_pos <= paddle[0].position + 1) || (ball.position_Y >= B11111101 && ball.virtual_pos >= paddle[1].position - 1 && ball.virtual_pos <=  paddle[1].position + 1); // abajo--arriba
}
/**------------------------------------------------------------------------------------------------------------------------------------------------*/
void updateScore(){
  if(ball.position_Y == B01111111){
    digitalWrite(t_display[1].pin_display,HIGH);
    digitalWrite(t_display[0].pin_display,LOW);
    t_display[0].score += 1;
    printDisplay(t_display[0].score);
    reset_game(&paddle[0],&paddle[1],&ball);
  } else if (ball.position_Y == B11111110) {
    digitalWrite(t_display[0].pin_display,HIGH);
    digitalWrite(t_display[1].pin_display,LOW);
    t_display[1].score += 1;
    printDisplay(t_display[1].score);
    reset_game(&paddle[0],&paddle[1],&ball);
  } else {
    digitalWrite(t_display[(i+1)%2].pin_display,HIGH);
    digitalWrite(t_display[i].pin_display,LOW);
    printDisplay(t_display[i].score);
    i = (i + 1) % 2;
  }
}
void printDisplay(int score){
  if(score == 0){
    digitalWrite(leds_7seg[0],HIGH);
    digitalWrite(leds_7seg[1],HIGH);
    digitalWrite(leds_7seg[2],HIGH);
    digitalWrite(leds_7seg[3],HIGH);
    digitalWrite(leds_7seg[4],LOW);
  } else if (score == 1){
    digitalWrite(leds_7seg[0],LOW);
    digitalWrite(leds_7seg[1],HIGH);
    digitalWrite(leds_7seg[2],LOW);
    digitalWrite(leds_7seg[3],LOW);
    digitalWrite(leds_7seg[4],LOW);
  } else if (score == 2) {
    digitalWrite(leds_7seg[0],HIGH);
    digitalWrite(leds_7seg[1],LOW);
    digitalWrite(leds_7seg[2],HIGH);
    digitalWrite(leds_7seg[3],LOW);
    digitalWrite(leds_7seg[4],HIGH);
  } else if (score == 3) {
    digitalWrite(leds_7seg[0],HIGH);
    digitalWrite(leds_7seg[1],HIGH);
    digitalWrite(leds_7seg[2],LOW);
    digitalWrite(leds_7seg[3],LOW);
    digitalWrite(leds_7seg[4],HIGH);
  }
  Serial.print("Score 1: ");
  Serial.print(t_display[0].score);
  Serial.print("\tScore 2: ");
  Serial.println(t_display[1].score);
}
bool isWin(){
  return t_display[0].score == 3 || t_display[1].score == 3;
}