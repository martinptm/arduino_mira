# arduino_mira
studying course project

Additionally to a theoretical course in signal processing and a series of lectures, covering principles of multiple measurement technologies used in different fields of physics, we were asked to come up with an arduino based project using at least one sensor and actuator. After short time of brainstorming an a look into our arduino starter kit we decided to try to create an robotic arm using what we already had to our hands: components included in the starter kit and two bluetooth modules.
At the end of the course all projects were presented in a conference like session with abstracts, posters and real life demos.

The biggest challenge to us was to integrate all the informations we found regarding single components into our prototype.
New to programming integrated circuits, we were very happy to achieve our goal and hold an working robot arm in our hands we built with low cost electrical components, hot glue and some pieces of wood. 

Practically the sensoric and the motoric parts were split up into two standalone projects communicating via bluetooth. This high modularity makes it also possible, to use the units independent (e.g. only the sensoric unit and use the data to controll an virtual arm or send predefined routines to the motoric unit).

To connect the two bluetooth modules we followed the steps described here: https://www.instructables.com/id/How-to-Use-Bluetooth-40-HM10/ (last check 03.08.2019). Very helpful to understand how to use the Gy-521 modules: http://www.mschoeffler.de/2017/10/05/tutorial-how-to-use-the-gy-521-module-mpu-6050-breakout-board-with-the-arduino-uno/ (last check 03.08.2019).
