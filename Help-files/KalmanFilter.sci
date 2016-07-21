// Object Tracking using Kalman Filter
//
// Calling Sequence
// KalmanFilterKalmanFilter("FileName",<value>,"dynamParams",<value>,"measureParams",<value>,"controlParams",<value>,
//                                "type",<value>,"colour",<value>,"drawpath",<value>
//                                "delay",<value>)
// 
// Parameters 
// int dynamparams- This specifies the state dimensionality.
// int measureparams- This specifies the measurement dimensionality
// int controlparams- This specifies the control vector values
// int type- This specifies the datatype of frames in the video
// string colour- This specifies the colour of the object to be tracked
// bool drawpath- This specifies whether you want the path of the moving object to be drawn
// bool delay - This specifies whether video playback show be slower or not.
//
// Description
// The function tracks the object using Kalman Filter. The red box is the object being
// detected using Kalman Filter method wheras the green box is the same except the method used
// here are basic image processing techniques to detect the object in the video frame.
//
// Examples
// //Performing Object Tracking on green ball
// KalmanFilter("FileName","singleball.avi","colour","green","delay","true","drawpath","true")
// //Performing Object Tracking on red juggling ball
// KalmanFilter("FileName","juggling.mp4","colour","red","drawpath","false");
//
// Author
// Kevin George
// 
