for q=1:3    
    for i=1:459
        for j=1:612
            disp("hel");
            a = I2(q);
            b = I1(q);
            if a(i,j)==1 then
                ROI(q)(i,j)=b(i,j);
            else
                NONROI(q)(i,j)=b(i,j);
            end
            disp("roi");
        end
    end
end
