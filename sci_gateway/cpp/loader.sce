// This file is released under the 3-clause BSD license. See COPYING-BSD.
// Generated by builder.sce : Please, do not edit this file
// ----------------------------------------------------------------------------
//
libskeleton_imagepr_path = get_absolute_file_path('loader.sce');
//
// ulink previous function with same name
[bOK, ilib] = c_link('libskeleton_imageprocessing');
if bOK then
  ulink(ilib);
end
//
list_functions = [ 'imread';
                   'VideoReader';
                   'epipolarlines';
];
addinter(libskeleton_imagepr_path + filesep() + 'libskeleton_imageprocessing' + getdynlibext(), 'libskeleton_imageprocessing', list_functions);
// remove temp. variables on stack
clear libskeleton_imagepr_path;
clear bOK;
clear ilib;
clear list_functions;
// ----------------------------------------------------------------------------
