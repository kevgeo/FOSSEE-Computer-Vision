//output = find(I2 >0)
ROI = zeros(459,612)
[rows cols]= size(output)
for i=1:cols
	ROI1(output(i)) = I1(1)(output(i));
	ROI2(output(i)) = I1(2)(output(i));
	ROI3(output(i)) = I1(3)(output(i));
end
