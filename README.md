# Cyberduck-Robot

## Disclaimer

The information displayed here is provided as is, you are using it at your own risk. Under no circumstances shall the author be liable for any kind of damages resulting from the use, misuse or inability to use of this code and information even though if the author had previously adivsed of the possibility of such damages.

## Summary

The principal goal of this project is to develop a line follower robot based on Arduino which uses 2 infrared sensors to keep on the track of the black 2cm wide line drawn on a white board. The original goal was to deploy a PID controlled robot, yet it became unattainable due to the constrained timeframe. Nevertheless, there exists a potential option (not illustrated in the circuit diagram but discernible in the images) to incorporate additional IR sensors for the purpose of executing a refined PID algorithm.

## Cyberduck Robot

### Components

Almost all components were repurporsed from previous projects or recycled from different objects, it would be an ardous task to track the origin of all of them.

- Arduino Uno with Direct Soldering
- L298n Motor Driver
- DC Gear Motor with Tire Wheels. Gear rate 1:48. 240 rpm.
- 2 IR Sensor module (https://www.amazon.es/dp/B07G38MXW8?psc=1&ref=ppx_yo2ov_dt_b_product_details)
- 6 AA battery holder + 6 AA batteries
- Caster Wheel
- SG90 Servo
- Plastic piece to tuck inside the duck's head (An old plastic gear was used for this task)
- Laser pointer
- Rubber Duck

Other:

- Various wires
- Soldering tin
- Glue sticks

The chassis is made out of old notebook covers stuck together with hot glue. Duct tape was used to embellish it. Afterwards, the rubber duck (Cyberduck from now on) was cut into three pieces: one from the head and two from the body. The head of Cyberduck contains a laser pointer coming out from its beak and it is filled by a thick gear, which is needed to attach the whole head to the servo that will serve as its neck. The bumper that joins the IR sensors with the whole chassis is made from defective 3D printed pieces. Finally, the hubcaps in the robot's wheels are made from to plastic bottle caps.

#### Circuit

![circuit](https://github.com/CRM-UAM/Cyberduck-Robot/assets/80209320/03fecc09-20bd-4552-84c9-713715fb7199)

### Algorithm 

A very straightforward algorithm was implemented. Given that sensors are positioned on both sides of the line, whenever one of the sensors detects the line, the robot immediately steers in the opposite direction. In other words, the robot is constantly avoiding getting over the line with the IR sensors. The whole code is implemented in the Cyberduck-Robot.ino file.

### Instructions of use

Connect the batteries to the robot and place it on the floor with the line between the sensors. 

## Acknowledgments

I extend my sincere gratitude to Carlos G. S. and Carmen G. M. for their invaluable assistance throughout the project.
