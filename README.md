# Blayone Rocket Project
A thrust vector controlled model rocket. 
This project was split into three separate parts (more info at https://www.josh.blayone.com/brp). First is Arch, which is the codename given to the flight computer. Second is Bone, which is the codename for the launchpad. Lastly is Dash, which is the name given to the model rocket. 

## Archsoft (Teensy 3.2)
Archsoft is the flight software used created for Arch(the flight computer onboard the model rocket).

This was programmed in C++ using Platform IO. Three different libraries were created exclusively for Arch to make interfacing with it's features easier.


## BoneApp (IOS)
BoneApp is the mobile application used to connect to the launchpad to get live telemetry data while on the pad, and to launch the rocket.

This was programmed in Javascript, using React Native. This app was developed for both IOS and Android but was only tested on IOS.


## Bonesoft (ESP8266)
Bonesoft is the software used for the launchpad computer to control the launchpad, as well as to allow the user to connect to the launchpad via wifi.

This was programmed in C++ using Platform IO.
