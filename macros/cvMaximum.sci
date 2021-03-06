//*********************************************************************//
// Author : Asmita Bhar, Kevin George
//*********************************************************************//

function varargout = cvMaximum(image, varargin)

// Finds maximum values in an input
//
// Calling Sequence
// [val ind] = cvMaximum(image);
// [val ind] = cvMaximum(image, name, value,...);
// val = cvMaximum(image, name, value, ...);
// ind = cvMaximum(image, name, value, ...);
// 
// Parameters 
// image : Input image matrix
// ValueOutputPort (Optional) : This property is set to true to output the minimum value. Default : true.
// IndexOutputPort (Optional) : This property is set to true to output the index of the minimum value. Default : true.
// Dimension (Output) : Dimension along which the function operates - Row, Column, All or Custom. Default : All
// CustomDimension (Optional) : The integer dimension over which the function calculates the maximum. This value cannot exceed the number of dimensions in input. It applies only when 'Dimension' property is set to 'Custom'. Default : 1
//
// Description
// The function calculates the maximum value in a given image matrix.
//
// Examples
// //Load an image
// I = imread('peppers.png');
// //Calculate the maximum (default dimension is 'All')
// [val ind] = cvMaximum(I);
// //Calculate the maximum when dimension is 'Row' and IndexOutputPort is set to false
// val = cvMaximum(I,'ValueOutputPort','false','Dimension','Row');
//
// Authors
// Asmita Bhar
// Kevin George
// 
	[lhs,rhs] = argn(0);
	if rhs<1 then
		error(msprintf("Not enough input arguments"));
	end
	if rhs>13 then
		error(msprintf("Too many input arguments"));
	end
	if lhs>2 then
		error(msprintf("Too many output arguments"));
	end
	[iRows iCols]=size(image(1))
	iChannels = size(image)

	valueOutputPort = 'true';
	indexOutputPort = 'true';
	dimension = 'All';
	customDimension = 1;
	roiProcessing = 'false';
	
	flag1=0; 
	i=1;
	while(i<rhs-1)
		if strcmpi(varargin(i),'ValueOutputPort')==0 then
			valueOutputPort = varargin(i+1)
			if strcmpi(valueOutputPort,"true") & strcmpi(valueOutputPort,"false") then
                		error(msprintf(" wrong input argument #%d,ValidityOutputPort not matched",i))
            		end
			i=i+2;
		elseif strcmpi(varargin(i),'IndexOutputPort')==0 then
			indexOutputPort = varargin(i+1)
			if strcmpi(indexOutputPort,"true") & strcmpi(indexOutputPort,"false") then
                		error(msprintf(" wrong input argument #%d, IndexOutputPort not matched",i))
            		end
			i=i+2;
		elseif strcmpi(varargin(i),'Dimension')==0 then
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
	
	if (~strcmpi(valueOutputPort,'false') & ~strcmpi(indexOutputPort,'false'))
		error(msprintf("Both cannot be false at the same time"));
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
	
	if(roiProcessing=='false') then	
		if (dimension=='All') then
			if(iChannels==1) then
				[val ind] = max(image(1))
				ind(1) = 1;
			elseif (iChannels==3) then
				val1 = max(image(1))
				val2 = max(image(2))
				val3 = max(image(3))
				if (val1>=val2) & (val1>=val3) then
					[val ind] = max(image(1))
					ind(3) = 1
				elseif (val2>=val1) & (val2>=val3) then
					[val ind] = max(image(2))
					ind(3) = 2
				else
					[val ind] = max(image(3))
					ind(3) = 3
				end
			end
			t = ind(1)
			ind(1) = ind(2)
			ind(2) = t
		end
		if (dimension=='Row') then
			if(iChannels==1) then
				[val ind] = max(image(1),'c');
			elseif(iChannels==3) then
				[val1 ind1] = max(image(1),'c');
				[val2 ind2] = max(image(2),'c');
				[val3 ind3] = max(image(3),'c');
				val(:,:,1) = val1;
				val(:,:,2) = val2;
				val(:,:,3) = val3;
				ind(:,:,1) = ind1;
				ind(:,:,2) = ind2;
				ind(:,:,3) = ind3;
			end
		end	
		if (dimension=='Column') then
			if(iChannels==1) then
				[val ind] = max(image(1),'r');
			elseif(iChannels==3) then
				[val1 ind1] = max(image(1),'r');
				[val2 ind2] = max(image(2),'r');
				[val3 ind3] = max(image(3),'r');
				val(:,:,1) = val1;
				val(:,:,2) = val2;
				val(:,:,3) = val3;
				ind(:,:,1) = ind1;
				ind(:,:,2) = ind2;
				ind(:,:,3) = ind3;
			end
		end
		if (dimension=='Custom') then
			if(iChannels==1) then
				if(customDimension==1) then
					[val ind] = max(image(1),'r');
				elseif(customDimension==2) then
					[val ind] = max(image(1),'c');
				end
			elseif(iChannels==3) then
				if(customDimension==1) then
					[val1 ind1] = max(image(1),'r');
					[val2 ind2] = max(image(2),'r');
					[val3 ind3] = max(image(3),'r');
					val(:,:,1) = val1;
					val(:,:,2) = val2;
					val(:,:,3) = val3;
					ind(:,:,1) = ind1;
					ind(:,:,2) = ind2;
					ind(:,:,3) = ind3;
				elseif(customDimension==2) then
					[val1 ind1] = max(image(1),'c');
					[val2 ind2] = max(image(2),'c');
					[val3 ind3] = max(image(3),'c');
					val(:,:,1) = val1;
					val(:,:,2) = val2;
					val(:,:,3) = val3;
					ind(:,:,1) = ind1;
					ind(:,:,2) = ind2;
					ind(:,:,3) = ind3;
				elseif(customDimension==3) then
					a = image(1);
					b = image(2);
					c = image(3);
					for i=1:iRows
						for j=1:iCols
							val(i,j)= max([a(i,j) b(i,j) c(i,j)]);
							if(val(i,j)==a(i,j)) then
								ind(i,j)=1;						
							elseif(val(i,j)==b(i,j)) then
								ind(i,j)=2;
							else
								ind(i,j)=3;
							end						
						end

					end
				end
			end
		end
	end
	if(roiProcessing=='true') then
		I2 = opencv_roipoly(image,c,r);
		out = I2;
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
				[val ind] = max(a(find(a>0)));
				//ind(1) = 1;
			elseif (iChannels==3) then
				a=ROI(1);
				b=ROI(2);
				c=ROI(3);
				val1 = max(a(find(a>0)));
				val2 = max(b(find(b>0)));
				val3 = max(c(find(c>0)));
				if (val1>=val2) & (val1>=val3) then
					[val ind] = max(ROI(1))
					//ind(3) = 1
				elseif (val2>=val1) & (val2>=val3) then
					[val ind] = max(ROI(2))
					//ind(3) = 2
				else
					[val ind] = max(ROI(3))
					//ind(3) = 3
				end
			end
			//t = ind(1)
			//ind(1) = ind(2)
			//ind(2) = t
		end
	end	
	
	
	if (~strcmpi(valueOutputPort,'true') & ~strcmpi(indexOutputPort,'true')) then
		varargout = list(val,ind);
	elseif (~strcmpi(valueOutputPort,'true') & ~strcmpi(indexOutputPort,'false')) then
		varargout = val
	elseif (~strcmpi(indexOutputPort,'true') & ~strcmpi(valueOutputPort,'false')) then
		varargout = ind
	end

endfunction

