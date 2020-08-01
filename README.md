# FaceTracking-MePed
Software used to allow a MePed V2 to detect and track faces with a camera.

Some Notes:
- Make sure to read the headers that are in every python and sketch files
- The MePed has to be connected to the device running the python program at all times
- An option is given in the .ino file for the MePed to track your face even when the camera is not 
  attached to the robot (camera remains stationary at all times), this is enabled by default.
- Make sure the python and sketch files are run independently from each other as the communication 
  port will be occupied when running one or the other
