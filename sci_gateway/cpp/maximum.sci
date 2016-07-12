//**************************************************
// Author : Asmita Bhar
//**************************************************

function varargout = maximum(image,varargin)
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
	indexOutputPort = 'true';
	dimension = 'All';
	customDimension = 1;
	ROIPROCESSING = 'true';
	Matrix_Col = [];
	Matrix_Row = [];
	
	flag=0;	
	i=1;
	while(i<rhs-1)
		if strcmpi(varargin(i),'ValueOutputPort')==0 then
			valueOutputPort = varargin(i+1)
		
		elseif strcmpi(varargin(i),'IndexOutputPort')==0 then
			indexOutputPort = varargin(i+1)
		
		elseif strcmpi(varargin(i),'Dimension')==0 then
			dimension = varargin(i+1)
		
		elseif strcmpi(varargin(i),'ROIPROCESSING')==0 then
			ROIPROCESSING = varargin(i+1)
			if strcmpi(ROIPROCESSING,"true") & strcmpi(ROIPROCESSING,"false") then
                		error(msprintf(" wrong input argument #%d,ROIPROCESSING not matched",i))
            		end

		elseif strcmpi(varargin(i),'MatrixCol')==0 then
			Matrix_Col = varargin(i+1)

		elseif strcmpi(varargin(i),'MatrixRow')==0 then
			Matrix_Row = varargin(i+1)

		elseif strcmpi(varargin(i),'CustomDimension')==0 then
			customDimension = varargin(i+1)
			flag=1;
		end
		i=i+2;
	end
	
	if (~strcmpi(valueOutputPort,'false') & ~strcmpi(indexOutputPort,'false'))
		error(msprintf("Both cannot be false at the same time"));
	end
	if (strcmpi(dimension,'Custom') & (flag==1))
		error(msprintf("The CustomDimension property is not relevant in this configuration"));
	end	

	image2 = roipoly(image, Matrix_Col, Matrix_Row)
	if(dimension=='All') then
		if(iChannels==1) then
			[val ind] = max(image2(1))
			ind(3) = 1
		elseif (iChannels==3) then
			val1 = max(image2(1))
			val2 = max(image2(2))
			val3 = max(image2(3))
			if (val1>=val2) & (val1>=val3) then
				[val ind] = max(image2(1))
				ind(3) = 1
			elseif (val2>=val1) & (val2>=val3) then
				[val ind] = max(image2(2))
				ind(3) = 2
			else
				[val ind] = max(image2(3))
				ind(3) = 3
			end
		end
		t = ind(1);
		ind(1) = ind(2);
		ind(2) = t;	
	end
	if (dimension=='Row') then
		if(iChannels==1) then
			[val ind] = max(image2(1),'c');
		elseif(iChannels==3) then
			[val1 ind1] = max(image2(1),'c');
			[val2 ind2] = max(image2(2),'c');
			[val3 ind3] = max(image2(3),'c');
			val = list(val1,val2,val2);
			ind = list(ind1,ind2,ind3);
		end
	end
	if (dimension=='Column') then
		if(iChannels==1) then
			[val ind] = max(image2(1),'r');
		elseif(iChannels==3) then
			[val1 ind1] = max(image2(1),'r');
			[val2 ind2] = max(image2(2),'r');
			[val3 ind3] = max(image2(3),'r');
			val = list(val1,val2,val2);
			ind = list(ind1,ind2,ind3);
		end
	end
	if (dimension=='Custom') then
		if(iChannels==1) then
			if(customDimension==1) then
				[val ind] = min(image2(1),'r');
			elseif(customDimension==2) then
				[val ind] = min(image2(1),'c');
			end
		elseif(iChannels==3) then
			if(customDimension==1) then
				[val1 ind1] = max(image2(1),'r');
				[val2 ind2] = max(image2(2),'r');
				[val3 ind3] = max(image2(3),'r');
				val = list(val1,val2,val2);
				ind = list(ind1,ind2,ind3);
			elseif(customDimension==2) then
				[val1 ind1] = max(image2(1),'c');
				[val2 ind2] = max(image2(2),'c');
				[val3 ind3] = max(image2(3),'c');
				val = list(val1,val2,val2);
				ind = list(ind1,ind2,ind3);
			end
		end
	end
	if (~strcmpi(valueOutputPort,'true') & ~strcmpi(indexOutputPort,'true')) then
		varargout = list(val,ind)
	elseif (~strcmpi(valueOutputPort,'true') & ~strcmpi(indexOutputPort,'false')) then
		varargout = list(val)
	elseif (~strcmpi(indexOutputPort,'true') & ~strcmpi(valueOutputPort,'false')) then
		varargout = list(ind)
	end

endfunction

