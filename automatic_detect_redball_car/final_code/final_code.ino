//2022/8/25
#include <Pixy2.h>
#include <Pixy2CCC.h>
#include <Servo.h>
#include <Stepper.h>


const int TrigPin = 12;  // 超音波模組的觸發腳
const int EchoPin = 13;   // 超音波模組的接收腳
const int stepsPerRevolution = 2048;
const int dangerThresh = 35;

unsigned long prev1;
unsigned long prev2;
unsigned long interval1;
unsigned long interval2;

double distance;      // 暫存接收訊號的高電位持續時間
double tmp;


int left_distance;
int right_distance;
int blocks;

Pixy2 pixy;

Servo head;
Servo arm1;
Servo arm2;
Servo Camera;

Stepper stepper2(stepsPerRevolution, 22, 26, 24, 28);
Stepper stepper1(stepsPerRevolution, 32, 36, 34, 38);

void p();

void arm_right();
void arm_left();
void arm_up();
void arm_down();
void capture();

void move_head();
void forward_normal();
void backward_normal();
void turnLeft_normal();
void turnRight_normal();

void forward_catch();
void backward_catch();
void turnLeft_catch();
void turnRight_catch();

void chase_collect_redball();

void setup() {
  pinMode(TrigPin, OUTPUT);  // 觸發腳設定成「輸出」
  pinMode(EchoPin, INPUT);   // 接收腳設定成「輸入」

  head.attach(10);
  head.write(90);

  pixy.init();
  Serial.begin(9600);

  //arm
  arm1.attach(8);
  arm1.write(140);

  stepper1.setSpeed(10);
  stepper2.setSpeed(10);
  
  prev1 = 0;         // 前回実行時刻を初期化
  prev2 = 0;
  interval1 = 20000;
  interval2 = 10000;

  
}


void loop() {
  p();
  blocks = pixy.ccc.getBlocks();
  distance = 0.01723 * tmp;
  Serial.print("cm=");
  Serial.println(distance);
  blocks = pixy.ccc.getBlocks();
  Serial.print("blocks=");
  Serial.println(blocks);
  if (pixy.ccc.numBlocks) {
    Serial.println("aaa");
    chase_collect_redball();
  } else {
    if (distance > dangerThresh) {
      unsigned long curr1 = millis();
      if ((curr1 - prev1) >= interval1) {
        unsigned long curr2 = millis();
        if ((curr2 - prev2) >= interval2) { // 前回実行時刻から実行周期以上経過していたら
          Serial.println("Right");
          turnRight_normal();
          prev2 = curr2;
        }
        prev1 = curr1;
      }
      else {
        forward_normal();
        Serial.println("Forward");
      }
    }
    else {
      move_head();
      if (left_distance > right_distance) {
        delay(250);
        Serial.println("Left");
        backward_normal();
        delay(250);
        turnLeft_normal();
      } else {
        delay(250);
        Serial.println("RIGHT");
        backward_normal();
        delay(250);
        turnRight_normal();
      }
    }
  }

}
void p() {     // 超音波感測程式
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  pinMode(EchoPin, INPUT);             // 讀取 echo 的電位

  tmp = pulseIn(EchoPin, HIGH);
}

void arm_right() {
  arm1.write(90);
  delay(1000);
}

void arm_left() {
  arm1.write(0);
  delay(1000);
}

void arm_up() {
  for (int i = 30; i <= 140; i++) {
    arm1.write(i);
    delay(10);
  }
}

void arm_down() {
  for (int i = 140; i >= 30; i--) {
    arm1.write(i);
    delay(10);
  }
}

void capture() {
  arm_down();
  delay(2500);
  arm_up();
  delay(2500);
}

void move_head() {
  head.write(150);
  delay(1000);
  p();
  left_distance = 0.01723 * tmp;
  head.write(30);
  delay(1000);
  p();
  right_distance = 0.01723 * tmp;
  Serial.print("left_distance=");
  Serial.println(left_distance);
  Serial.print("right_distance=");
  Serial.println(right_distance);
  head.write(90);

}

void forward_normal() {     // 馬達轉向：前進（兩個馬達都正轉）
  for (int i = 0; i < 1024; i++) {
    stepper1.step(-1);    //正轉一圈
    stepper2.step(1);    //正轉一圈
  }
  delay(250);

}

void backward_normal() {    // 馬達轉向：後退（兩個馬達都反轉）
  for (int i = 0; i < 1024; i++) {
    stepper1.step(1);    //正轉一圈
    stepper2.step(-1);    //正轉一圈
  }
}

void turnLeft_normal() {    // 馬達轉向：左轉（馬達A反轉、馬達B正轉）
  for (int i = 0; i < 1024; i++) {
    stepper1.step(-1);    //正轉一圈
    stepper2.step(-1);    //正轉一圈
  }
  delay(250);

}

void turnRight_normal() {
  for (int i = 0; i < 1024; i++) {
    stepper1.step(1);    //正轉一圈
    stepper2.step(1);    //正轉一圈
  }
  delay(250);
}

void forward_catch() {     // 馬達轉向：前進（兩個馬達都正轉）
  for (int i = 0; i < 360; i++) {
    stepper1.step(-1);    //正轉一圈
    stepper2.step(1);    //正轉一圈
  }
  delay(250);

}

void backward_catch() {    // 馬達轉向：後退（兩個馬達都反轉）
  for (int i = 0; i < 302; i++) {
    stepper1.step(1);    //正轉一圈
    stepper2.step(-1);    //正轉一圈
  }
  delay(250);

}

void turnLeft_catch() {    // 馬達轉向：左轉（馬達A反轉、馬達B正轉）
  for (int i = 0; i < 102; i++) {
    stepper1.step(-1);    //正轉一圈
    stepper2.step(-1);    //正轉一圈
  }
  delay(250);

}

void turnRight_catch() {
  for (int i = 0; i < 153; i++) {
    stepper1.step(1);    //正轉一圈
    stepper2.step(1);    //正轉一圈
  }
  delay(250);

}

void chase_collect_redball() {
  Serial.println("catch_ball_time");
  int x; //修車身
  int width; // 修距離
  int y; // 修距離/10
  int height; // 修距離
  int area;
  int final_data;
  blocks = pixy.ccc.getBlocks();
  x = pixy.ccc.blocks[0].m_x;
  y = pixy.ccc.blocks[0].m_y;
  height = pixy.ccc.blocks[0].m_height;
  width = pixy.ccc.blocks[0].m_width;
  area = height * width; //修正值;
  final_data = y + area / 100;
  Serial.print("blocks=");
  Serial.println(blocks);
  Serial.print("x=");
  Serial.println(x);
  Serial.print("area=");
  Serial.println(area);
  Serial.print("Final_data=");
  Serial.println(final_data);

  if (x <= 200 && x >= 130) {
    if ( y <= 120 && y >= 40) {
      Serial.println("capture");
      capture();
    } else {
      if (y > 120) {
        backward_catch();
        delay(500);
        Serial.println("backward");
      } else {
        forward_catch();
        delay(500);
        Serial.println("forward");
      }
    }
  } else {
    if (x > 200) {
      turnRight_catch();
      delay(500);
      Serial.println("turnRight");
    } else {
      turnLeft_catch();
      delay(500);
      Serial.println("turnLeft");
    }
  }
  delay(1500);//1.5秒測試1次
}
