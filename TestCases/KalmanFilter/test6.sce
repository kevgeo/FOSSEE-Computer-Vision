
//Video filename is  singleball.avi
//Colour of object to be tracked is green
//Delay is set as video is just two second, so we want to make sure we can see the object tracking properly by adding delay
//Drawpath is also set which will draw the path of the tracked object using red crosses

//Now testing what output is given when a name value pair argument is mispelled, in this case it is the value "false"
KalmanFilter("FileName","singleball.avi","colour","green","delay","true","drawpath","fal");

//output->          
//                                                                                           !--error 999 
//Invalid value for drawpath 

//Now testing what output is given when a name value pair argument is mispelled, in this case it is the value "false"
KalmanFilter("FileName","jugglingball.mp4","colour","green","delay","true","drawpath","fal");

//output->          
//                                                                                           !--error 999 
//Invalid value for drawpath 

//Now testing what output is given when a name value pair argument is mispelled, in this case it is the value "false"
KalmanFilter("FileName","ped.avi","colour","green","delay","true","drawpath","fal");

//output->          
//                                                                                           !--error 999 
//Invalid value for drawpath 
