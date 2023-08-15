// C++ code
//

#include "dcMotor.h"

// Hàm khởi tạo cấu hình ban đầu
void setupDCMotor()
{
  // Khai báo các chân của 2 Motor đóng mở khay là OUTPUT
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  // Khai báo chân đèn tín hiệu
  pinMode(2, OUTPUT);

  // Tắt động cơ ban đầu
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}

// Hàm mở khay thức ăn
void openFoodTray()
{
  // Chiều quay động cơ để mở khay
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);

  // Cấp tín hiệu PWM để điều chỉnh tốc độ động cơ
  delay(4000); // Dừng mở trong 4 giây

  // Tắt động cơ sau khi mở khay
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}

// Hàm đóng khay thức ăn
void closeFoodTray()
{
  // Chiều quay động cơ để đóng khay
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);

  // Cấp tín hiệu PWM để điều chỉnh tốc độ động cơ
  delay(4000); // Dừng đóng trong 4 giây

  // Tắt động cơ sau khi đóng khay
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}

// Hàm mở phễu
void openFunnel()
{
  // Chiều quay động cơ để mở khay
  digitalWrite(funnelPin1, HIGH);
  digitalWrite(funnelPin2, LOW);

  // Cấp tín hiệu PWM để điều chỉnh tốc độ động cơ
  delay(4000); // Dừng mở trong 4 giây
}

// Hàm đóng phễu
void closeFunnel()
{
  // Chiều quay động cơ để đóng khay
  digitalWrite(funnelPin1, LOW);
  digitalWrite(funnelPin2, HIGH);

  // Cấp tín hiệu PWM để điều chỉnh tốc độ động cơ
  delay(4000); // Dừng đóng trong 4 giây
}

// Hàm mở đèn tín hiệu
void turnOnLed()
{
  digitalWrite(2, HIGH);
  delay(1000);
  digitalWrite(2, LOW);
}
// Hàm chính
void loopDCMotor()
{

  // Gọi hàm để mở phễu thức ăn
  openFunnel();

  // Gọi hàm để đóng phễu thức ăn
  closeFunnel();

  // Gọi hàm mở khay thức ăn
  openFoodTray();

  turnOnLed();

  delay(5000); // Đợi 5 giây trước khi đóng khay thức ăn

  // Gọi hàm đóng khay thức ăn
  closeFoodTray();

  turnOnLed();

  delay(5000); // Đợi 5 giây trước khi mở khay thức ăn lại
}
