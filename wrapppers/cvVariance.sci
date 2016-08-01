//*********************************************************************//
// Author : Asmita Bhar, Kevin George
//*********************************************************************//

function [val] = cvVariance(image, varargin)

// Finds variance values in an input
//
// Calling Sequence
// val = cvVariance(image);
// val = cvVariance(image, name, value,...);
// 
// Parameters 
// image : Input image matrix
// Dimension (Output) : Dimension along which the function operates - Row, Column, All or Custom. Default : All
// CustomDimension (Optional) : The integer dimension over which the function calculates the minimum. This value cannot exceed the number of dimensions in input. It applies only when 'Dimension' property is set to 'Custom'. Default : 1
// val : Stores the variance calculated
//
// Description
// The function calculates the variance value in a given image matrix.
//
// Examples
// //Load an image
// I = imread('peppers.png');
// //Calculate the variance (default dimension is 'All')
// val = cvVariance(I);
// //Calculate the variance when dimension is 'Row'
// val = cvVariance(I,'Dimension','Row');
//
// Authors
// Asmita Bhar
// Kevin George
// 
	[lhs,rhs] = argn(0);
	if rhs<1 then
		error(msprintf("Not enough input arguments"));
	end
	if rhs>9 then
		error(msprintf("Too many input arguments"));
	end
	[iRows iCols]=size(image(1))
	iChannels = size(image)

	dimension = 'All';
	customDimension = 1;
	roiProcessing = 'false';
	
	flag1=0; 
	i=1;
	while(i<rhs-1)
		if strcmpi(varargin(i),'Dimension')==0 then
			dimension = varargin(i+1)
			if strcmpi(dimension,"Column") & strcmpi(dimension,"Row") &strcmpi(dimension,"All") & strcmpi(dimension,"Custom") then
               			error(msprintf(" wrong input argument #%d, Dimension not matched",i))
            		end
			i=i+2;
		elseif strcmpi(varargin(i),'CustomDimension')==0 then
			customDimension = varargin(i+1)
			flag1=1;
			i=i+2;
		elseif strcmpi(varargin(i), 'ROIProcessing')==0 then
			roiProcessing = varargin(i+1)
			if(roiProcessing=='true') then
				c = varargin(i+2);
				r = varargin(i+3);
				i=i+4;
			else
				i=i+2;
			end
		end
		
	end
	
	if (strcmpi(dimension,'Custom') & (flag1==1))
		error(msprintf("The CustomDimension property is not relevant in this configuration"));
	end
	
	if (strcmpi(dimension,"All") & strcmpi(roiProcessing,'true'))
		error(msprintf("ROI Property is not relevant in this configuration"));
	end

	if(customDimension<1) 
		error(msprintf("CustomDimension must be greater than or equal to 1"));
	end

	if(iChannels==1) then
		if(customDimension>2)
			error(msprintf("CustomDimension cannot be greater than the dimension of the input."));
		end
	elseif(iChannels==3) then
		if(customDimension>3)
			error(msprintf("CustomDimension cannot be greater than the dimension of the input."));
		end
	end
	
	if(iChannels==1)
		I = double(image(1));
	elseif(iChannels==3)
		I1 = double(image(1));
		I2 = double(image(2));
		I3 = double(image(3));
	end

	if(roiProcessing=='false') then	
		if (dimension=='All') then
			if(iChannels==1) then
				val = variance(I)
			elseif (iChannels==3) then
				val1 = variance(I1)
				val2 = variance(I2)
				val3 = variance(I3)
				val = variance([val1 val2 val3])				
			end
		end
		if (dimension=='Row') then
			if(iChannels==1) then
				val = variance(I,'c');
			elseif(iChannels==3) then
				val1 = variance(I1,'c');
				val2 = variance(I2,'c');
				val3 = variance(I3,'c');
				val(:,:,1) = val1;
				val(:,:,2) = val2;
				val(:,:,3) = val3;
			end
		end	
		if (dimension=='Column') then
			if(iChannels==1) then
				val = variance(I,'r');
			elseif(iChannels==3) then
				val1 = variance(I1,'r');
				val2 = variance(I2,'r');
				val3 = variance(I3,'r');
				val(:,:,1) = val1;
				val(:,:,2) = val2;
				val(:,:,3) = val3;
			end
		end
		if (dimension=='Custom') then
			if(iChannels==1) then
				if(customDimension==1) then
					val = variance(I,'r');
				elseif(customDimension==2) then
					val = variance(I,'c');
				end
			elseif(iChannels==3) then
				if(customDimension==1) then
					val1 = variance(I1,'r');
					val2 = variance(I2,'r');
					val3 = variance(I3,'r');
					val(:,:,1) = val1;
					val(:,:,2) = val2;
					val(:,:,3) = val3;
				elseif(customDimension==2) then
					val1 = variance(I1,'c');
					val2 = variance(I2,'c');
					val3 = variance(I3,'c');
					val(:,:,1) = val1;
					val(:,:,2) = val2;
					val(:,:,3) = val3;
				elseif(customDimension==3) then
					for i=1:iRows
						for j=1:iCols
							val(i,j)= variance([I1(i,j) I2(i,j) I3(i,j)]);
						end

					end
				end
			end
		end
	end
	if(roiProcessing=='true') then
		I4 = roipoly(image,c,r);
		out = I4;
		output = find(out>0);		
		[rows cols] = size(out);
		if(iChannels==1)
			ROI = zeros(iRows,iCols);
			for i=1:cols
				ROI(output(i)) = image(1)(output(i));
			end
		elseif(iChannels==3)
			ROI1 = zeros(iRows,iCols);
			ROI2 = zeros(iRows,iCols);
			ROI3 = zeros(iRows,iCols);
			for i=1:cols
				ROI1(output(i)) = image(1)(output(i));
				ROI2(output(i)) = image(2)(output(i));
				ROI3(output(i)) = image(3)(output(i));
			end
			ROI = list(ROI1,ROI2,ROI3);
		end
		if (dimension=='All') then
			if(iChannels==1) then
				a=ROI;
				val = variance(a(find(a>0)));
			elseif (iChannels==3) then
				a=ROI(1);
				b=ROI(2);
				c=ROI(3);
				val1 = variance(a(find(a>0)));
				val2 = variance(b(find(b>0)));
				val3 = variance(c(find(c>0)));
				val = variance([val1 val2 val3]);
			end
		end	
	end

endfunction

