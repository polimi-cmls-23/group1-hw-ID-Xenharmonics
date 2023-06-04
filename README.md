# CMLS HW3 - Design and implementation of a Computer Music System - Kinetic Harmonizer
The **JUCE** plugin we've developed for the Interaction Design project is **Kinetic Harmonizer**, a delay-line harmonizer controlled by the motion captured by an accelerometer, communicated to the host through Arduino and Processing (using respectively Serial communication and Open Sound Control messages).</br></br>

<p align="center">
  <img src="images/signalFlow.png"  width="55%" height="55%">
</p>

</br>
These are the main connections that were necessary in order to implement the interactive part of this project.
</br></br>
<p>
  
  <img src="Processing_HW3/Kinetic_harm processing demo.gif"  width="35%" align="right" height="35%">
  </br>
  </br>
  </br>
  </br>
  </br>
  We've implemented a visualization of the accelerometer's motion in Processing, it consists of a rectangle that moves in response to the data being fed to the software by the Arduino connection.
</p>
</br></br>

# About
**Kinetic Harmonizer** is an audio effect plugin that allows the user to add a pitchshifted replica of the input audio to the signal. The pitchshift is determined by the LFO-controlled delay time the signal is processed by.

The user is able to customize the parameters of the harmonizer through motion, which is captured by an accelerometer. In paricular the sensor we've used for this project is the analog MMA7361 Triple-Axis Accelerometer by Freescale. The analog signal is then converted to digital by the Arduino UNO in order to prepare it for processing by the plugin.

This is the **GUI** of the effect we've developed:

<p align="center">
  <img src="images/plugin screenshot.png">
</p>
</br></br>
The following block diagram shows the logical structure of the implementation of the effect in more detail:</br></br>

<p align="center">
  <img src="images/KineticHarm.png"  width="85%" height="85%">
</p>

The main parameters of the plugin are:

+ **Main Controls**:
  - *Dry/wet*: allows to choose the amount of the harmonized signal mixed in the output audio
  - *Gain*: slider to select the gain level of the input/processed signal
  - *Mute*: checkbox that cuts the signal from the out
  - *Pan*: allows to pan the dry or output signal left/right
  - *Semitones*: the amount of cents the pitch of the delayed signal will be shifted by
  - *Delay*: controls how long the delay time of the wet signal is in relation to the input signal
  - *Feedback*: determines the amount of signal that is fed back into the delay line

## Interactive Control
With the three dimensional data provided by the sensor, we can calculate two distinct rotational values in real time in order to control just as many parameters in the plugin:

+ the **Pitch (x axis rotation)** controls the semitone shift
+ the **Roll (x axis rotation)** controls the dry/wet mix
