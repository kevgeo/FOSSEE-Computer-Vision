// Estimates optical flow using Farneback method and videoplays the motion.
//
// Calling Sequence
// opticalFlowFarneback("FileName",<file path>);
// opticalFlowFarneback("FileName",<file path>,"NumPyramidLevels",<value>,"PyramidScale",<value>,"NumIteration",<value>
//                       ,"NeighborhoodSize",<value>,"FilterSize",<value>,"PolySigma",<value>);
// 
// Parameters 
// string FileName : Path of video file
// double NumPyramidLevels - Specifies the number of pyramid levels.
// double PyramidScale - Specifies the image scale.
// double NumIteration - Specifies the number of iterations per pyramid level.
// double NeighborhoodSize - Specifies the size of neighborhood in pixels.
// double FilterSize - Specifies the size of filter in pixels.
// double PolySigma - Specifies standard deviation Gaussian value.
//
// Description
// A video is played and the optical flow calculated by the Farneback method is denoted by the green
// dots that turn into lines when a moving object is detected.
//
// Examples
// //Estimating optical flow of moving pedestrians
// opticalFlowFarneback("FileName","ped.avi");
// //Estimating optical flow of juggling balls
// opticalFlowFarneback("FileName","juggling2.mp4","Poly_Sigma",1.2,"FilterSize",6,"NeighborhoodSize",5,"NumIteration",3,"PyramidScale",0.5);
//
// Author
// Kevin George
// 
