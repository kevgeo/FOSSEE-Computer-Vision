function[] =imshow(Image)         //retrieving list and creating 3 dimensional matrix out of it
    dimensions=size(Image)
    if dimensions==3 then 
    [c d]=size(Image(1));
     r=matrix(Image(1),c,d);
     g=matrix(Image(2),c,d);
     b=matrix(Image(3),c,d);
     z(:,:,1)=r; 
     z(:,:,2)=g; 
     z(:,:,3)=b;
     [NumberOfRows NumberOfColumns NumberOfChannels] = size(z);
     NumberOfPixels = NumberOfRows * NumberOfColumns;
     MaxGrayValue = 2 ^ 8 - 1;
     ColorMap = double(matrix(z, NumberOfPixels, NumberOfChannels)) ...
           / MaxGrayValue;
     Img = matrix(1 : NumberOfPixels, NumberOfRows, NumberOfColumns);
     elseif dimensions==1 then
     [c d]=size(Image(1));
     Img=matrix(Image(1),c,d);
     MaxUInt8 = 2 ^ 8 - 1;
     MaximumGrayValue = MaxUInt8;
     ColorMap = graycolormap(double(MaximumGrayValue + 1));
     end;
     show(Img,ColorMap);
endfunction
