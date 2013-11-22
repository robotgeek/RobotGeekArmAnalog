// RobotGeek Robot Arm Manual Joystick control code
//=============================================================================
//
//  This code is a Work In Progress and is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
//  
//=============================================================================



#include <Servo.h>

//define analog pins that will be connected to the joystick pins
#define BASE     0
#define SHOULDER 1
#define ELBOW    2
#define WRIST    3
#define GRIPPER  4

// Declare servos for Servo library
Servo BAS_SERVO;  // 
Servo SHL_SERVO;  // 
Servo ELB_SERVO;  // 
Servo WRI_SERVO;  // 
Servo WRO_SERVO;  // 
Servo GRI_SERVO;  // 

// Servo position limitations
#define BASE_MIN      600
#define BASE_MAX      2400
#define SHOULDER_MIN  600
#define SHOULDER_MAX  2400
#define ELBOW_MIN     600
#define ELBOW_MAX     2400
#define WRIST_MIN     600
#define WRIST_MAX     2400 
#define GRIPPER_MIN   800
#define GRIPPER_MAX   1800 

//generic deadband limits - not all joystics will center at 512, so these limits remove 'drift' from joysticks that are off-center.
#define DEADBANDLOW 462
#define DEADBANDHIGH 662


int Base     =1500;     //current position of the Base servo
int Shoulder =1500;    //current position of the Shoulder servo  
int Elbow    =1500;   //current position of the Elbow servo
int Wrist    =1500;  //current position of the Wrist servo
int Gripper  =1500; //current position of the Gripper servo


int joyBaseVal = 0;      //current value of the Base input (analog 0)
int joyShoulderVal = 0;  //current value of the Shoulder input (analog 1)
int joyElbowVal = 0;
int joyWristVal = 0;
int joyGripperVal = 0;


int joyBaseMapped = 0;//tilt joystick value, mapped from 1-1023 to -500-500
int joyShoulderMapped = 0;//pan joystick value, mapped from 1-1023 to -500-500
int joyElbowMapped = 0;//tilt joystick value, mapped from 1-1023 to -500-500
int joyWristMapped = 0;//pan joystick value, mapped from 1-1023 to -500-500
int joyGripperMapped = 0;//tilt joystick value, mapped from 1-1023 to -500-500


int speed = 10;  //speed modififer, increase this to increase the speed of the movement


//===================================================================================================
// Setup 
//====================================================================================================
void setup() {

  // Attach servo and set limits
  BAS_SERVO.attach(3, BASE_MIN, BASE_MAX);
  SHL_SERVO.attach(5, SHOULDER_MIN, SHOULDER_MAX);
  ELB_SERVO.attach(6, ELBOW_MIN, ELBOW_MAX);
  WRI_SERVO.attach(9, WRIST_MIN, WRIST_MAX);
  GRI_SERVO.attach(10, GRIPPER_MIN, GRIPPER_MAX);
  

  delay(1000);
  
  // Move servos to center
  set_servo();

}

void loop() {
  
   //read analog values from joysticks
   joyBaseVal = analogRead(BASE);
   joyShoulderVal = analogRead(SHOULDER);
   joyElbowVal = analogRead(ELBOW);
   joyWristVal = analogRead(WRIST);
   joyGripperVal = analogRead(GRIPPER);
        
   //Mapping analog joystick value to servo PWM signal range
     joyBaseMapped = map(joyBaseVal, 1023, 0, BASE_MIN, BASE_MAX);
     Base = joyBaseMapped;
    
   //deadzone and incremental code for Joystick values
   if(joyShoulderVal > DEADBANDHIGH || joyShoulderVal < DEADBANDLOW)
   {
     joyShoulderMapped = map(joyShoulderVal, 0, 1023, -speed, speed);
     Shoulder += joyShoulderMapped;
   }

   //deadzone and incremental code for Joystick values  
   if(joyElbowVal > DEADBANDHIGH || joyElbowVal < DEADBANDLOW)
   {
     joyElbowMapped = map(joyElbowVal, 0, 1023, -speed, speed);
     Elbow -= joyElbowMapped;
   }

   //deadzone and incremental code for Joystick values 
   if(joyWristVal > DEADBANDHIGH || joyWristVal < DEADBANDLOW)
   {
     joyWristMapped = map(joyWristVal, 0, 1023, -speed, speed);
     Wrist -= joyWristMapped;
   }
   
   //Mapping analog joystick value to servo PWM signal range
     joyGripperMapped = map(joyGripperVal, 0, 1023, GRIPPER_MIN, GRIPPER_MAX);
     Gripper = joyGripperMapped;


   
      
    //enforce upper/lower limits for servo position variable 
    //(actual servo positions min/max are setup above, this is just for variable)
    if (Base < BASE_MIN)
    {
      Base =BASE_MIN;
    }  
    else if (Base > BASE_MAX)
    {
      Base =BASE_MAX;
    }
    
    
    if (Shoulder < SHOULDER_MIN)
    {
      Shoulder =SHOULDER_MIN;
    }  
  
    else if (Shoulder > SHOULDER_MAX)
    {
      Shoulder =SHOULDER_MAX;
    }
  
  
  
    //enforce upper/lower limits for tilt servo
    if (Elbow < ELBOW_MIN)
    {
      Elbow = ELBOW_MIN;
    }  
    else if (Elbow > ELBOW_MAX)
    {
      Elbow = ELBOW_MAX;
    }
    
    
    //enforce upper/lower limits for pan servo
    if (Wrist < WRIST_MIN)
    {
      Wrist = WRIST_MIN;
    }  
  
    else if (Wrist > WRIST_MAX)
    {
      Wrist =WRIST_MAX;
    }
  
  
      //enforce upper/lower limits for tilt servo
    if (Gripper < GRIPPER_MIN)
    {
      Gripper =GRIPPER_MIN;
    }  
    else if (Gripper > GRIPPER_MAX)
    {
      Gripper = GRIPPER_MAX;
    }
    
    //Funciton to set PWM Servo positions
    set_servo();
  }


void set_servo()
{
  BAS_SERVO.writeMicroseconds(Base);
  WRI_SERVO.writeMicroseconds(Wrist);
  SHL_SERVO.writeMicroseconds(Shoulder);
  ELB_SERVO.writeMicroseconds(Elbow);
  GRI_SERVO.writeMicroseconds(Gripper);
    delay(10);
}


void servo_park()
{
  BAS_SERVO.writeMicroseconds(1500);
  delay(10);
  SHL_SERVO.writeMicroseconds(900);
  delay(10);
  ELB_SERVO.writeMicroseconds(800);
  delay(10);
  WRI_SERVO.writeMicroseconds(1500);
  delay(10);  
  GRI_SERVO.writeMicroseconds(900);
  delay(10); 
  return;
}
