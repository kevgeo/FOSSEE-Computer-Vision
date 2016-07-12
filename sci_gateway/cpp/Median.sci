//**************************************************
// Author : Asmita Bhar
//**************************************************

function [out] = Median(image,varargin)
	[lhs,rhs] = argn(0);
	if rhs<1 then
		error(msprintf("Not enough input arguments"));
	end
	if rhs>7 then
		error(msprintf("Too many input arguments"));
	end
	
	[iRows iCols]=size(image(1))
	iChannels = size(image)
	
	sortMethod = 'Quick sort';
	dimension = 'All';
	customDimension = 1;
	ROIPROCESSING = 'true';
	Matrix_Col = [];
	Matrix_Row = [];

	flag=0;
	i=1;
	while(i<rhs-1)
		if strcmpi(varargin(i),'SortMethod')==0 then
			sortMethod = varargin(i+1);
			if strcmpi(sortMethod,"Quick sort") & strcmpi(sortMethod,"Insertion sort") then
                		error(msprintf(" wrong input argument #%d,SortMethod not matched",i))
            		end
		elseif strcmpi(varargin(i),'Dimension')==0 then
			dimension = varargin(i+1)
			if strcmpi(dimension,"Column") & strcmpi(dimension,"Row") &strcmpi(dimension,"All") & strcmpi(dimension,"Custom") then
                		error(msprintf(" wrong input argument #%d, Dimension not matched",i))
            		end

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
	
	if (strcmpi(dimension,'Custom') & (flag==1))
		error(msprintf("The CustomDimension property is not relevant in this configuration"));
	end

	image2 = roipoly(image, Matrix_Col, Matrix_Row)
	if(iChannels==1) then
		if(dimension=='All') then
			out = median(image2(1));
		elseif(dimension=='Row') then
			out = median(image2(1),'c');
		elseif(dimension=='Column') then
			out = median(image2(1),'r');
		elseif(dimension=='Custom') then
			if(customDimension==1) then
				out = median(image2(1),'r');
			elseif(customDimension==2) then
				out = median(image2(1),'c');
			end
		end
	elseif(iChannels==3)
		if(dimension=='Row') then
			out(:,:,1) = median(image2(1),'c');
			out(:,:,2) = median(image2(2),'c');
			out(:,:,3) = median(image2(3),'c');
		elseif(dimension=='Column') then
			out(:,:,1) = median(image2(1),'r');
			out(:,:,2) = median(image2(2),'r');
			out(:,:,3) = median(image2(3),'r');
		elseif(dimension=='Custom') then
			if(customDimension==1) then
				out(:,:,1) = median(image2(1),'r');
				out(:,:,2) = median(image2(2),'r');
				out(:,:,3) = median(image2(3),'r');
			elseif(customDimension==2) then
				out(:,:,1) = median(image2(1),'c');
				out(:,:,2) = median(image2(2),'c');
				out(:,:,3) = median(image2(3),'c');
			elseif(customDimension==3) then
				
endfunction
	
