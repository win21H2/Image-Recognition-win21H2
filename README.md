# Image Recognition

##### Note: This repository is <b>not</b> finished yet so there <b>will</b> be some bugs and incomplete code files<br>
##### Also note: This repository is built on the Jupyter Labs notebook provided by the Nvidia developers page.

#### TODO
> - Continue addings the docs for the image recognition program
> - Merge the final code into one file instead of multiple and figure out how to create folders to sort out the files

##### Image recognition description:
>During the first part of this repository, I will take you through how to setup the Nvidia Jetson nano with Jupyter Labs, run a sample camera script to check your camera is functioning, run the sample image recognition program (which comes with the docker), and finnaly finish off with changing the code to use an Arduino Uno.

### Chapters <br>
#### Image recognition <br>
>1 - Prerequisites<br>
>2 - What you need to have on hand<br>
>3 - Getting started<br>
>4 - Running the default image recognition program<br>
>5 - Making edits to the program<br>
>6 - Including the Arduino board (Without the Jetson)<br>
>7 - Including the Arduino board (With the Jetson using an example)<br>
>8 - Wrap up/conclusion<br>

# Prerequisites
 > - Basic understanding of the Ubuntu/Linux CLI and how to enter commands/open files and navigate paths as well as sudo, pip, and sh commands <br>
 > - Understanding of how to use the Arduino IDE and how to upload code <br>

# What you need to have on hand <br>
 > - A Nvidia Jetson Nano board (you can get one [here](https://www.amazon.com/NVIDIA-Jetson-Nano-Developer-945-13541-0000-000/dp/B08J157LHH/ref=sr_1_3?crid=14OGE6DXXIQJV&keywords=nvidia+jetson+nano+2gb&qid=1650155641&sprefix=nvidia+jetson+nano+%2Caps%2C212&sr=8-3) although keep in mind that as of now they might not be in stock) <br>
 > - A USB camera (I used the [Logitech C920x HD Webcam](https://www.amazon.com/Logitech-C920x-Pro-HD-Webcam/dp/B085TFF7M1/ref=sr_1_3?crid=27NQO1U637C2M&keywords=logitech%2Busb%2Bcamera&qid=1650155485&sprefix=logitech%2Busb%2Bcamera%2Caps%2C160&sr=8-3&th=1) although you can use any other USB camera)
 > - A microSD card (you can get one [here](https://www.amazon.com/SanDisk-Ultra-UHS-I-Memory-Adapter/dp/B00M55C0NS/ref=sr_1_6?crid=38NYM8JJMSNRA&keywords=micro%2Bsd%2Bcard&qid=1650155732&sprefix=micro%2Bsd%2Bcard%2Caps%2C255&sr=8-6&th=1) one although I would recommend getting one that is 64gb)
 > - A Arduino Uno (you can get one [here](https://store.arduino.cc/products/arduino-uno-rev3))

# Getting started
### Setting up the Jetson Nano
>During this stage you will add a SWAP file, setup a docker container for the Jetson nano, and finnaly log into the docker and open Jupyter labs

Step 1.  Power up and SSH into the Nano

Step 2. Run the below command to check how much memory you have on the Jetson
>Description: The image recognition program requires a lot of RAM and using the next few commands, you will make something thats called "virtual RAM" which is as the name suggests, RAM that is stored virtually and tricks the Jetson into thinking it has more RAM compared to what it physically has. <br>

    free -m


Step 3. Run the following commands ONLY if your Jetson does not already have a SWAP file
>Description: The commands below will create the virtual RAM needed for the program to run normally

    sudo systemctl disable nvzramconfig
    sudo fallocate -l 4G /mnt/4GB.swap
    sudo chmod 600 /mnt/4GB.swap
    sudo mkswap /mnt/4GB.swap

    sudo su
    echo "/mnt/4GB.swap swap swap defaults 0 0" >> /etc/fstab
    exit


Step 4. Run the following commands to make the docker container needed to run Jupyter Labs
>Description: The following commands will first make a directory for the docker container, that is followed by the script which is run every time you use the docker command which sets some things up. The last two steps are for making that script executable so that we can use a ./.sh command to run the docker.

    mkdir -p ~/nvdli-data

    echo "sudo docker run --runtime nvidia -it --rm --network host \
    --volume ~/nvdli-data:/nvdli-nano/data \
    --device /dev/video0 \
    nvcr.io/nvidia/dli/dli-nano-ai:v2.0.2-r32.7.1" > docker_dli_run.sh
    
    chmod +x docker_dli_run.sh

    ./docker_dli_run.sh
    
    
Step 5. Log into Jupyter Labs
>Description: You use the URL that was provided after running the docker script above. You have to copy and paste it into a browser and use the default password `dlinano` to login. A picture of the provided URL is below (note this URL will NOT be the same for you)

![image](https://user-images.githubusercontent.com/92825997/163696048-b2b53b30-eeb3-405a-b0b4-602776c7d8d6.png)

>If you see something like the below image, you have setup your Jetson correctly and can head over to the next steps

![image](https://user-images.githubusercontent.com/92825997/163696076-2bb063ed-7d28-462a-8a77-121c0170d2a9.png)

# Including the Arduino board (Without the Jetson)
>During this stage you will find out what the Arduino side of the code looks like, find out what the different lines do, and test out the code in the Serial Monitor (we are not using the Jetson yet because it requires a different baud rate which we have to setup later)

Step 1. Open up the file Image-recognition-win21H2 >> test code >> caseswitch.ino and load it to the Arduino IDE
>Description: You should see something that looks like the below code. In short, this code will be the test code we will use to test the serial connection with the Arduino Uno

```c++
void setup() {
  Serial.begin(9600);
  for (int thisPin = 2; thisPin < 4; thisPin++) {
    pinMode(thisPin, OUTPUT);
  }
}

void loop() {
  if (Serial.available() > 0) {
    int inByte = Serial.read();
    switch (inByte) {
      case 'a':
        digitalWrite(4, HIGH);
        digitalWrite(3, LOW);
        digitalWrite(2, LOW);
        break;
      case 'b':
        digitalWrite(4, HIGH);
        digitalWrite(3, LOW);
        digitalWrite(2, LOW);
        break;
      case 'c':
        digitalWrite(4, HIGH);
        digitalWrite(3, LOW);
        digitalWrite(2, LOW);
        break;
    }
  }
}
```

Step 2. Select the correct COM port and board (in this case the Arduino Uno @ whatever port the Arduino is using and upload the code to the Uno

### Code walk-through

We first use the void setup section to firstly set up the serial baud rate and the pins for the LEDs
>Description: We use a ```for``` statement to make each LED from pin 2 to pin 3 (since the pin = 2 and is < than 4) an output pin and we use the variable name ```thisPin``` to set the values

```c++
void setup() {
  Serial.begin(9600);
  for (int thisPin = 2; thisPin < 4; thisPin++) {
    pinMode(thisPin, OUTPUT);
  }
}
```
We then need to check if there is any serial data using the code below
>Description: We use an ```if``` statement to check if the serial line is avaiable (there is no point sending data if the line is taken up by another thing) and if the value is greater than 0 (0 meaning there is nothing inputted) and make a variable based on the serial input
```c++
void loop() {
  if (Serial.available() > 0) {
    int inByte = Serial.read();
```
We then need to use the ```inByte``` variable in a case switch to check the input and if the input is "a" and make an output (in this case turn on certain LEDs)
>Description: We use the variable to check which letter was inputted to the Serial Monitor and then turn the case on for example if we input "a" so that only the code in case "a" is ran until another input is found. NOTE: this is reapeated for case "b" and "c" but the LEDs are different
```c++
    switch (inByte) {
      case 'a':
        digitalWrite(4, HIGH);
        digitalWrite(3, LOW);
        digitalWrite(2, LOW);
        break;
      case 'b':
        digitalWrite(4, HIGH);
        digitalWrite(3, LOW);
        digitalWrite(2, LOW);
        break;
      case 'c':
        digitalWrite(4, HIGH);
        digitalWrite(3, LOW);
        digitalWrite(2, LOW);
        break;
    }
  }
}
```
Once you have uploaded the code to the board, you can see that if you open up the serial monitor and set the baud rate to ```9600``` and start inputting values like "a", you should see the LED on pin 4 light up and vice versa with the other LEDs

# Including the Arduino board (With the Jetson using an example)
>During this stage you will upload and test the example code with the Jetson nano

Step 1. Open up the file Image-recognition-win21H2 >> test code >> arduinoledtest.ino and load it to the Arduino IDE
>Description: You should see something that looks like the below code. In short, this code will be the test code we will use to check if the Jetson is sending the serial commands to the Arduino.

```c++
int rled = 2;
int gled = 3;

void setup() {
  Serial.begin(11520);
  pinMode(rled, OUTPUT);
  pinMode(gled, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    int inByte = Serial.read();
    switch (inByte) {
      case 'R':
        digitalWrite(rled, HIGH);
        digitalWrite(gled, LOW);
        break;
      case 'B':
        digitalWrite(rled, LOW);
        digitalWrite(gled, HIGH);
        break;
      case 'O':
        digitalWrite(rled, LOW);
        digitalWrite(gled, LOW);
        break;
    }
  }
}
```
Step 2. Select the correct COM port and board (in this case the Arduino Uno @ whatever port the Arduino is using and upload the code to the Uno

### Code walk-through

This code looks similar to the one before right? That is correct although we have to change up a few things. First, we have to change the baud rate and then we have to change the LEDs we have to better suit the task we are trying to complete
>Description: We use the baud rate of ```11520``` because the Jetson nano is able to communicate using that baud rate. NOTE: this baud rate is NOT the same as the default Arduino IDE baud rate so you will not be able to send serial commands to the Arduino via the serial monitor

Once you have uploaded the code to the Arduino board, you can connect the board to the Jetson using the programming cable and get the image recognition program up and running. What you will notice is that when the Prediction score goes above a certain point, the green LED turns on and vice versa!
