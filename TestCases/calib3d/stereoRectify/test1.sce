//Checking if functon works

cam1=[100 0 105;0 106 107; 0 0 1];
cam2=[100 0 105;0 106 107; 0 0 1];
dis1=[0 0 0 4 5];
dis2=[0 0 0 4 5];
r=[1 2 3;4 5 6;0 8 7];
t=[0 0 45];
ImageSize = [480 640];

//[R1,R2,P1,P2,Q] = stereoRectify(cam1,dis1,cam2,dis2,ImageSize,r,t);
[R1,R2,P1] = stereoRectify(cam1,dis1,cam2,dis2,ImageSize,r,t);

