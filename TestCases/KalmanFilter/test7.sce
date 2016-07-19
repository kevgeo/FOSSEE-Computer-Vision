
//Video filename is  singleball.avi
//Colour of object to be tracked is green
//Delay is set as video is just two second, so we want to make sure we can see the object tracking properly by adding delay
//Drawpath is also set which will draw the path of the tracked object using red crosses

//A negative value is entered for dynamparams 
KalmanFilter("FileName","singleball.avi","dynamParams",-1,"colour","green","delay","true","drawpath","true");

//output->          
//                                                                                                             !--error 999 
//Invalid Value for measureParams. Please enter value more than zero. 

KalmanFilter("FileName","singleball.avi","dynamParams",-1,"colour","green","delay","true","drawpath","true");
//output->          
//                                                                                                             !--error 999 
//Invalid Value for measureParams. Please enter value more than zero. 

