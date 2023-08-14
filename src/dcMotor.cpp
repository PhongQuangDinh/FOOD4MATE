// C++ code
//
// Khai báo chân kết nối đến module điều khiển động cơ DC (L298N)
#include "dcMotor.h"

// Hàm khởi tạo cấu hình ban đầu
void setupDCMotor() {
  // Khai báo các chân là OUTPUT
  pinMode(enablePin, OUTPUT);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);

  pinMode(2,OUTPUT);
  // Tắt động cơ ban đầu
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
}

// Hàm mở khay thức ăn
void openFoodTray() {
  // Chiều quay động cơ để mở khay
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);

  // Cấp tín hiệu PWM để điều chỉnh tốc độ động cơ
  analogWrite(enablePin, 10); // Giá trị PWM có thể điều chỉnh từ 0 (tắt) đến 255 (tối đa tốc độ)
  delay(4000); // Dừng mở trong 2 giây

  // Tắt động cơ sau khi mở khay
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
}

// Hàm đóng khay thức ăn
void closeFoodTray() {
  // Chiều quay động cơ để đóng khay
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);

  // Cấp tín hiệu PWM để điều chỉnh tốc độ động cơ
  analogWrite(enablePin, 10); // Giá trị PWM có thể điều chỉnh từ 0 (tắt) đến 255 (tối đa tốc độ)
  delay(4000); // Dừng đóng trong 1.5 giây

  // Tắt động cơ sau khi đóng khay
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
}

void turnOnLed()
{
  digitalWrite(2,HIGH);
  delay(1000);
  digitalWrite(2,LOW);
}
// Hàm chính
void loopDCMotor() {
  // Gọi hàm mở khay thức ăn
  openFoodTray();
  turnOnLed();
  
  delay(5000); // Đợi 5 giây trước khi đóng khay thức ăn

  // Gọi hàm đóng khay thức ăn
  closeFoodTray();
  
  turnOnLed();
  
  delay(5000); // Đợi 5 giây trước khi mở khay thức ăn lại

}
