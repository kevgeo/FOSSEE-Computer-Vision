f1=scf(1);
f2=scf(2);
while hasFrame(a)
    stacksize('max');
    scf(f1);
    frame = readFrame(a);
    imshow(frame);
    fg_mask = readopticalFlowFarneback(frame);
    scf(f2);
    imshow(fg_mask);
end
close();
