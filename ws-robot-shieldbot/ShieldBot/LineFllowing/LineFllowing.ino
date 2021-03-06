/*
 * LineFllowingSimple.ino
 *
 *  Created on: 2014年8月12日
 *      Author: caixiaoliang
 */
#include <Shieldbot.h>
//#define SHIELDBOTDEBUG
//Make sure the dip switches are turned ON, and none of your shields are using pins A0,A1,A2,A3 or D4
int maxSpeed = 100;
int speedFactor = 1.5;
Shieldbot shieldbot = Shieldbot();	//decares a Shieldbot object
int S1,S2,S3,S4,S5;	//values to store state of sensors
int n,k=50;//用于控制PWM数值
uint8_t flag=1;//用于标志位

void setup(){
  Serial.begin(9600);//Begin serial comm for debugging
  shieldbot.setMaxSpeed(maxSpeed,maxSpeed);//255 is max, if one motor is faster than another, adjust values
}

void loop()
{
  //Read all the sensors
  S1 = shieldbot.readS1();
  S2 = shieldbot.readS2();
  S3 = shieldbot.readS3();
  S4 = shieldbot.readS4();
  S5 = shieldbot.readS5();

  //Print the status of each sensor to the Serial console
#ifdef SHIELDBOTDEBUG
  Serial.print("S5: ");
  Serial.print(S5);
  Serial.print(" S4: ");
  Serial.print(S4);
  Serial.print(" S3: ");
  Serial.print(S3);
  Serial.print(" S2: ");
  Serial.print(S2);
  Serial.print(" S1: ");
  Serial.print(S1);
  Serial.println();

  //Note about IR sensors
  //if a sensor sees HIGH, it means that it just sees a reflective surface background(ex. whie)
  //if a sensor sees LOW, it means that it sees a non-reflective surface or empty space (ex. black tape line, or empty space off ledge)

  if(S1 == HIGH && S5 == HIGH)
  {	//if the two outer IR line sensors see background, go forward
    shieldbot.forward();
	Serial.println("Forward");
  }
  else if(S1 == LOW && S5 == LOW)
  {	//if either of the two outer IR line sensors see empty space (like edge of a table) stop moving
    shieldbot.stop();
	Serial.println("Stop");
    delay(100);
  }
  else if((S1 == LOW) || (S2 == LOW))
  {	//if the two most right IR line sensors see black tape, turn right
    shieldbot.drive(127*speedFactor,-128*speedFactor);// to turn right, left motor goes forward and right motor backward
	Serial.println("Right");
    delay(50);
  }
  else if((S5 == LOW) || (S4 == LOW))
  {	//if either of the two most left IR line sensors see black , turn left
    shieldbot.drive(-128*speedFactor,127*speedFactor);// to turn right, left motor goes backward and right motor forward
	Serial.println("Left");
    delay(50);
  }
  else	//otherwise just go forward
    shieldbot.forward();
#endif

  if(S3==LOW&&S2==HIGH&&S4==HIGH)
  {
	  k++;
	  if(k>=100)//速度如果太大，转弯时会直接冲出赛道
	  {
		  k=100;//限制速度最大值
	  }
	  shieldbot.drive(k*speedFactor,k*speedFactor);
	  flag=1;//置标志位
  }

  else if(S3==LOW&&S2==HIGH&&S4==LOW)  //说明小车向右偏了
  {
	  if(flag==1)
	  {
		  k=90;//重置速度初始值
		  n=90;
		  flag=0;
	  }
	  else
	  {
		  n=n-1;
		  k=k+1;
		  shieldbot.drive(n*speedFactor,k*speedFactor);//左边pwm变小，右边PWM变大，从而使小车向左转
		  if(n<=75)//PWM值太小的话，小车动不了，右轮扭矩小，所以相差转角大一些
		  {
			  n=90;
			  k=90;
		  }
	  }
  }
  else if(S3==LOW&&S2==LOW&&S4==HIGH)//说明小车向左偏离了
  {
	  if(flag==1)
	  {
		  k=90;//重置速度初始值
		  n=90;
		  flag=0;
	  }
	  else
	  {
		  n=n-1;
		  k=k+1;
		  shieldbot.drive(k*speedFactor,n*speedFactor);//左边pwm变大，右边PWM变小，从而使小车向右转
		  if(n<=80)//
		  {
			  n=90;
			  k=90;
		  }
	  }
  }
  else if(S3==HIGH&&S2==HIGH&&S4==LOW)//说明小车已经向右偏离较大了
  {

	  shieldbot.drive(10*speedFactor,125*speedFactor);//因为实际中发现电机左右扭矩不一样，右边电机扭矩小
  }
  else if(S3==HIGH&&S2==LOW&&S4==HIGH)//说明小车向左偏离了比较大的角度了
  {

	  shieldbot.drive(110*speedFactor,10*speedFactor);
  }
  else if(S5==LOW||S4==LOW)//小车偏移向右已经太多了，需要大范围校准
  {
	  shieldbot.drive(0*speedFactor,120*speedFactor);
  }
  else if(S1==LOW||S2==LOW)//小车偏移向左已经太多了，需要大范围校准
  {
	  shieldbot.drive(120*speedFactor,0*speedFactor);
  }
  else if(S1==HIGH&&S2==HIGH&&S3==HIGH&&S4==HIGH&&S5==HIGH)
  {
	  shieldbot.forward();
  }
  else
  {
	  shieldbot.stop();//其他的话说明，不符合标准电胶布宽度，或者小车 一开始没有放好在黑胶布中间,或者被拿起来了
  }
}





