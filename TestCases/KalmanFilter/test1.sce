
//Video filename is  singleball.avi
//Colour of object to be tracked is green
//Delay is set as video is just two second, so we want to make sure we can see the object tracking properly by adding delay
//Drawpath is also set which will draw the path of the tracked object using red crosses

KalmanFilter("FileName","singleball.avi","colour","green","delay","true","drawpath","true");

//output is correct and ball is being tracked by red window

