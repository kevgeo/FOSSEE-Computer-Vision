     function[] =show(Img,ColorMap) 
     FigureHandle = gcf();
     drawlater();
     FigureHandle.color_map = ColorMap
     FigureHandle.background = -2; // sets the background to white
     FigureHandle.figure_name = "Title";
     [NumberOfRows NumberOfColumns] = size(Img);
     FigureHandle.axes_size = [NumberOfColumns NumberOfRows];
     delete(gca()); // previous image is deleted
     Diagram = gca();
     [NumberOfRows NumberOfColumns] = size(Img);
     Diagram.data_bounds = [1, 1; NumberOfColumns, NumberOfRows];
     Diagram.axes_visible = ['off' 'off' 'off'];
     Diagram.isoview = 'on';
     Options = '082'; // Box is drawn around image.
     Matplot(Img, Options);
     drawnow();
     endfunction
