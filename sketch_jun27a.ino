#include <HID-Project.h>
#include <ClickEncoder.h>
#include <TimerOne.h>

ClickEncoder *encoder;
int16_t last, value;

void timerIsr() {
  encoder->service();
}

void setup() {

  Serial.begin(9600);
  
  encoder = new ClickEncoder(2, 3, A0, 4);
  encoder->setAccelerationEnabled(0);

  Timer1.initialize(500);
  Timer1.attachInterrupt(timerIsr); 
  
  last = -1;

  Consumer.begin();
}

void print_value() {
  Serial.print("Encoder Value: ");
  Serial.println(value);
}

void loop() {
  // put your main code here, to run repeatedly:

  value += encoder->getValue();

  if (value > last){
    Serial.println("MEDIA_VOL_DOWN");
    Consumer.write(MEDIA_VOL_DOWN);
    print_value();
  }
  else if (value < last){
    Serial.println("MEDIA_VOL_UP");
    Consumer.write(MEDIA_VOL_UP);
    print_value();
  }
 
  last = value;
  
  ClickEncoder::Button b = encoder->getButton();
  if (b != ClickEncoder::Open) {
    Serial.println("Button: ");
    switch (b) {
      case ClickEncoder::Pressed:
          Serial.println("pressed");
          break;
          
      case ClickEncoder::Released:
          Serial.println("release");
          break;
          
      case ClickEncoder::Held:
          Serial.println("HELD");
//          Consumer.write(MEDIA_FAST_FORWARD);
          break; 
          
      case ClickEncoder::Clicked: 
          Serial.println("clicked");
          Consumer.write(MEDIA_PLAY_PAUSE);
          break;
          
      case ClickEncoder::DoubleClicked:
          Serial.println("ClickEncoder::DoubleClicked");
          Consumer.write(MEDIA_NEXT);
        break;
    }
  }    

}
