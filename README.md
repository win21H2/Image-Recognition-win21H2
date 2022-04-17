# Image Recognition

This repo has code from Jupyter Labs and I have used an Nvidia Jetson nano in combination with a USB camera to run image recognition so that when the system recognizes for example a red triangle, it will send a serial command to an Arudino which is also hooked up to the Jetson Nano. 

# Prerequisites
 
 > - Basic understanding of the Ubuntu CLI and how to enter commands/open files and navigate paths as well as sudo commands <br>
 > - Basic understanding of how to use Arduino and how to upload code to the board
# What you need to have on-hand
 > - A Nvidia Jetson Nano board (you can get one [here](https://www.amazon.com/NVIDIA-Jetson-Nano-Developer-945-13541-0000-000/dp/B08J157LHH/ref=sr_1_3?crid=14OGE6DXXIQJV&keywords=nvidia+jetson+nano+2gb&qid=1650155641&sprefix=nvidia+jetson+nano+%2Caps%2C212&sr=8-3) although keep in mind that as of now they might not be in stock) <br>
 > - A USB camera (I used the [Logitech C920x HD Webcam](https://www.amazon.com/Logitech-C920x-Pro-HD-Webcam/dp/B085TFF7M1/ref=sr_1_3?crid=27NQO1U637C2M&keywords=logitech%2Busb%2Bcamera&qid=1650155485&sprefix=logitech%2Busb%2Bcamera%2Caps%2C160&sr=8-3&th=1) although you can use any other USB camera)
 > - A microSD card (you can get one [here](https://www.amazon.com/SanDisk-Ultra-UHS-I-Memory-Adapter/dp/B00M55C0NS/ref=sr_1_6?crid=38NYM8JJMSNRA&keywords=micro%2Bsd%2Bcard&qid=1650155732&sprefix=micro%2Bsd%2Bcard%2Caps%2C255&sr=8-6&th=1) one although I would reccomend getting one that is 64gb)
 > - An Arduino Uno (you can get one [here](https://www.amazon.com/Arduino-A000066-ARDUINO-UNO-R3/dp/B008GRTSV6/ref=sr_1_3?crid=FFOODKO122K8&keywords=Arduino+uno&qid=1650155910&sprefix=arduino+u%2Caps%2C457&sr=8-3) or if you want to get one from the official store get it from [here](https://store.arduino.cc/products/arduino-uno-rev3))

# Getting started
### Setting up the Jetson Nano

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
