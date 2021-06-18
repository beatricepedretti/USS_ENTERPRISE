# AY2021_II_Project_1-1
Code and files of the Final Project 1 - Variant 1. Academic Year 2020/2021, II Semester

Our project consists in developing a system able to perform a three-dimensional scanning of an object using two servomotors , the , and one ultrasonic sensor, the HCSR04. Our sonar must be opportunely moved to detect the distance surfaces in the field of view of the USS.

<h2>Configuration through GUI</h2>
-inserire immagine GUI-
Our GUI consists in six buttons: 
<ul>
  <li>Connect</li>
  <li>Start</li>
  <li>Stop</li>
  <li>Step angle, three possibilities</li>
</ul>
and a space that at the end of the acquisition it will be filled with the reconstructed image. 

<h3>UART communication settings</h3>

The user has the possibility of connecting automatically the GUI with the device through UART communication by pushing the button named 'Connect'. If a board programmed with our code is connected to the computer it will send back a string 'Device succesfully connected$' and our computer will understand which is the right COM to use.  


<h3>Step angle settings</h3>

The user can also configure some device settings through a GUI. The parameter that can be configured is: <em>Step angle</em> of the servomotors. This operation can allow the user to obtain a more defined image by improving the steps of the servomotors and so the number of points that describe the surface.