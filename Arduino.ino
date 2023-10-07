#include <Wire.h>
//Set ADXL345 module pin SDA to A4 and SCL to A5

int num_Measure = 128;
int pinSignal = A0;
long Sound_signal; 
long sum = 0 ;
int dB = 0;
long level = 0 ;   
int ADXL345 = 0x53; 
float X_out, Y_out, Z_out, lastx, lasty, lastz;
float sample = 500;

void setup (){   
  pinMode (pinSignal, INPUT);    
  Serial.begin(9600);
  Wire.begin(); 
  Wire.beginTransmission(ADXL345);
  Wire.write(0x2D);
  Wire.write(8); 
  Wire.endTransmission();
  delay(500);
}  
   
void loop (){    
sound();
vibrate();
delay(500);
}

void sound() {
  for (int i = 0 ; i <num_Measure; i ++)  {  
    Sound_signal = analogRead (pinSignal);  
    sum =sum + Sound_signal;  
  }  
  Serial.print("\n");
  level = sum / num_Measure; 
  dB = (analogRead(pinSignal) + 83.2073) / 11.003;
  Serial.print(dB);
  Serial.print(" ");
  sum = 0;
}

void vibrate(){
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32); 
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true);
  X_out = ( Wire.read()| Wire.read() << 8); 
  X_out = X_out/256; 
  Y_out = ( Wire.read()| Wire.read() << 8); 
  Y_out = Y_out/256;
  Z_out = ( Wire.read()| Wire.read() << 8); 
  Z_out = Z_out/256;

  //*new
  float avgx = abs(X_out - lastx) * 10000.0 / 500 / 10.0; 
  float avgy = abs(Y_out - lasty) * 10000.0 / 500 / 10.0; 
  float avgz = abs(Z_out - lastz) * 10000.0 / 500 / 10.0; 
  float x =  (pow(avgx,2)); 
  float y =  (pow(avgy,2));
  float z =  (pow(avgz,2));
  float avgxyz = x + y + z;
  float getar = sqrt(avgxyz);
  
  Serial.print("");
  Serial.print(getar);

  lastx = X_out;
  lasty = Y_out;
  lastz = Z_out;
  
//  Serial.print(X_out);
//  Serial.print(" ");
//  Serial.print(Y_out);
//  Serial.print(" ");
//  Serial.println(Z_out);
//  Serial.print("");

}
