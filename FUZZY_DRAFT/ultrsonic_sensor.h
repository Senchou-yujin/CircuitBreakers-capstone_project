#define trigPin 5
#define echoPin 6

// Define variables:
long duration;
int distance;

void setup() {
  // Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Begin Serial communication at a baudrate of 9600:
  Serial.begin(9600);
  Serial.println("Ultrasonic Sensor Debug Mode");
}

void loop() {
  // Clear the trigPin by setting it LOW:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Trigger the sensor by setting the trigPin high for 10 microseconds:
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read the echoPin. pulseIn() returns the duration in microseconds:
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance (in cm):
  distance = duration * 0.034 / 2;  // More precise calculation
  
  // Print raw and calculated values for debugging:
  Serial.print("Raw duration: ");
  Serial.print(duration);
  Serial.print(" μs | Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  delay(1000); // Short delay between readings
}