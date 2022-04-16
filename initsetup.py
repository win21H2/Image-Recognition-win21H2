# Run all of these in this exact order or else the rest of the code will not work

!pip3 install pyserial
# We have to install pyserial because it is the library we use for communicating to the Arduino

!ls -ltrh /dev/video*
# We have to check if we are getting any video inputs from the USB ports

from jetcam.usb_camera import USBCamera
camera = USBCamera(width=224, height=224, capture_device=0)
camera.running = True
# We first have to import the USB camera library and then set the values of the camera and run said camera
