# Automated Security System with User Control and Facial Recognition
  The project encompasses a model that integrates an Arduino UNO development board, an LCD screen, an LED traffic light module, a door controlled by a stepper motor with the necessary driver, a keypad, a buzzer, a video camera and a photoresistor. <br/>
<br/>
  The system operates under two primary scenarios based on the photoresistor's light detection capability:<br/>
  <br/>
_**Automatic Mode (Sufficient Light Detection):**_ <br/>
  **- Face Detection:**  The camera is continuously active and instantly detects faces. If a recognized face is detected, the green LED lights up and the door opens. <br/>
  **- Unrecognized Face:**  If the face is not recognized, the door will either remain closed or close if it was previously open and the yellow LED will light up. <br/>
  **- Prolonged Unrecognition:**  If no face is recognized for an extended period, an alarm will sound and the red LED will light up. The alarm will stop only when a recognized face is detected again. <br/>
<br/>
The automated module was developed with Python, using the OpenCV and CVzone libraries through the following steps: <br/>
**- Image Capture:** The program captures a series of images, which are then uploaded to a database with corresponding IDs and names. <br/>
**- Training:** The images are used to train the system with .yml files. <br/>
**- Detection:** The system determines whether a face is recognized or not, displaying the person's name if recognized or indicating an unrecognized face. <br/>
  To enhance functionality, the program prioritizes detection of a recognized person if both recognized and unrecognized faces are present in the frame, ensuring the door opens.<br/> 
<br/>
_**Manual Mode (Insufficient Light Detection):**_ <br/>
  The traffic light LEDs illuminate and the system awaits input via the keypad. If the correct password is entered, the door opens and closes automatically after a few seconds. If the password is incorrect, the door remains closed and each incorrect attempt results in one LED turning off until three failed attempts trigger the alarm. <br/>
  The manual mode was developed using Arduino code with the board continuously monitoring the input data to control the model's components. <br/>
  <br/>
  **Technical Implementation:** For each scenario, data is transmitted to the Arduino, enabling appropriate sounds and messages to be displayed on the LCD screen. The project involves both hardware and software integration to create a cohesive and functional security system.
