getd('toolbox_general/');
getd('toolbox_signal/');
getd('toolbox_graph/');
stacksize('max');
n = 300;
name = 'road2.png';
//M = load_image('../toolbox_graph/road.png', n);
//M = rescale(M);
RGB = ReadImage('data/testcitymap_new.png')
Image = RGB2Gray(RGB)

pstart = [14,250;13