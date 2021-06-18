# AY2021_II_Project_1-1
Code and files of the Final Project 1 - Variant 1. Academic Year 2020/2021, II Semester

Our project consists in developing a system able to perform a three-dimensional scanning of an object using two servomotors , the , and one ultrasonic sensor, the HCSR04. Our sonar must be opportunely moved to detect the distance surfaces in the field of view of the USS.

<h2>Configuration through GUI</h2>
-inserire immagine GUI-
<h3>GUI settings</h3>

Our GUI consists in six buttons: <em>Connect</em>, <em>Start</em>, <em>Stop</em>, <em>Step angle</em> and a space that at the end of the acquisition it will be filled with the reconstructed image. 
The user has the possibility of connecting automatically the GUI with the device through UART communication by pushing the button named 'Connect'. The user can also configure some device settings through a GUI. The parameter that can be configured is: <em>Step angle of the servomotors</em>. This operation can allow the user to obtain a more defined image by improving the steps of the servomotors and so the number of points that describe the surface. 


<h3>Temperature settings</h3>
In addition to the device settings, the