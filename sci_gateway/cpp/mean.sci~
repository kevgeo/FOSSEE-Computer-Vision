//*********************************************************************//
// Author : Kevin George
//*********************************************************************//

function [out] = mean(image, varargin)
	[lhs,rhs] = argn(0);
	if rhs<1 then
		error(msprintf("Not enough input arguments"));
	end
	if rhs>9 then
		error(msprintf("Too many input arguments"));
	end
	if lhs>2 then
		error(msprintf("Too many output arguments"));
	end
	[iRows iCols]=size(image(1))
	iChannels = size(image)

	valueOutputPort = 'true';
	//indexOutputPort = 'true';
	dimension = 'All';
	customDimension = 1;
	
	flag=0; 
	i=1;
	while(i<rhs-1)
	//	if strcmpi(varargin(i),'ValueOutputPort')==0 then
	//		valueOutputPort = varargin(i+1)
	//		if strcmpi(valueOutputPort,"true") & strcmpi(valueOutputPort,"false") then
          //      		error(msprintf(" wrong input argument #%d,ValidityOutputPort not matched",i))
            //		end
	//	elseif strcmpi(varargin(i),'IndexOutputPort')==0 then
	//		indexOutputPort = varargin(i+1)
	//		if strcmpi(indexOutputPort,"true") & strcmpi(indexOutputPort,"false") then
          //      		error(msprintf(" wrong input argument #%d, IndexOutputPort not matched",i))
            //		end
		if strcmpi(varargin(i),'Dimension')==0 then
			dimension = varargin(i+1)
			if strcmpi(dimension,"Column") & strcmpi(dimension,"Row") &strcmpi(dimension,"All") & strcmpi(dimension,"Custom") then
                		error(msprintf(" wrong input argument #%d, Dimension not matched",i))
            		end
		elseif strcmpi(varargin(i),'CustomDimension')==0 then
			customDimension = varargin(i+1)
			flag=1;
		end
		i=i+2;
	end
	
	//if (~strcmpi(valueOutputPort,'false') & ~strcmpi(indexOutputPort,'false'))
	//	error(msprintf("Both cannot be false at the same time"));
	//end
	if (strcmpi(dimension,'Custom') & (flag==1))
		error(msprintf("The CustomDimension property is not relevant in this configuration"));
	end
	
	if (dimension=='All') then
		if(iChannels==1) then
			val = mean(image(1))
		elseif (iChannels==3) then
			val1 = mean(image(1))
			val2 = mean(image(2))
			val3 = mean(image(3))
			if (val1<=val2) & (val1<=val3) then
				val = mean(image(1))
			elseif (val2<=val1) & (val2<=val3) then
				val = mean(image(2))
			else
				val = mean(image(3))
			end
		end
		
	end
	if (dimension=='Row') then
		if(iChannels==1) then
			val = mean(image(1),'c');
		elseif(iChannels==3) then
			val1 = mean(image(1),'c');
			val2 = mean(image(2),'c');
			val3 = mean(image(3),'c');
			val(:,:,1) = val1;
			val(:,:,2) = val2;
			val(:,:,3) = val3;
		end
	end
	if (dimension=='Column') then
		if(iChannels==1) then
			val = mean(image(1),'r');
		elseif(iChannels==3) then
			val1 = mean(image(1),'r');
			val2 = mean(image(2),'r');
			val3 = mean(image(3),'r');
			val(:,:,1) = val1;
			val(:,:,2) = val2;
			val(:,:,3) = val3;
		end
	end
	if (dimension=='Custom') then
		if(iChannels==1) then
			if(customDimension==1) then
				val = mean(image(1),'r');
			elseif(customDimension==2) then
				val = mean(image(1),'c');
			end
		elseif(iChannels==3) then
			if(customDimension==1) then
				val1 = mean(image(1),'r');
				val2  = mean(image(2),'r');
				val3  = mean(image(3),'r');
				val(:,:,1) = val1;
				val(:,:,2) = val2;
				val(:,:,3) = val3;
			elseif(customDimension==2) then
				val1  = mean(image(1),'c');
				val2 = mean(image(2),'c');
				val3 = mean(image(3),'c');
				val(:,:,1) = val1;
				val(:,:,2) = val2;
				val(:,:,3) = val3;
				
			end
		end
	end
	//if (~strcmpi(valueOutputPort,'true') & ~strcmpi(indexOutputPort,'true')) then
	//	varargout = list(val,ind);
	//elseif (~strcmpi(valueOutputPort,'true') & ~strcmpi(indexOutputPort,'false')) then
	//	varargout = val
	//elseif (~strcmpi(indexOutputPort,'true') & ~strcmpi(valueOutputPort,'false')) then
	//	varargout = ind
	//end
	
endfunction
