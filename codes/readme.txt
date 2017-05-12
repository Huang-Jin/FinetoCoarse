This program is the demo of FTC algorithm.
It was written by Jin Huang and Julie Delon.

The main.cpp contains the controller of parameter.
demo.cpp is the demo to use the ftc function.
ftc.cpp contains all of the functions that ftc needed.
HistList.cpp defined the data structure ftc used.
io_png.c is the interface of libpng14, provided by IPOL, to read the png files.

The directory "image" contains all of the images used in our paper.
You could use them as example.

To use this demo in Linux, you should input "make" in the console.
We used g++ and libpng14 in our processing of compiling.
While in windows, you need to open a new project and put these files into it and compile it yourself.

To use this program after compiling, you could just input the name of the program to see the usage.
usage  : ftc input.png [eps] [-t] [-s sigma width]
         ftc [-h|-v]

         input.png   the name of input image
                     if image has 1 channel,
                     we will treat it as a gray image
                     if image has 3 channel,
                     we will treat it as a color image

         eps     value of epsilon
                 default is 1
         -t      -t for using the third step
                 the third step of ftc algorithm
                 will cost lots of time
         -s      -s for using smooth
         sigma   the sigma used in smooth
         width   the bandwidth used in smooth

         -h      usage help
         -v      version info

Such as this in linux: ./ftc ./images/flowers.png -s 3 3
You will get some output like these:
First, it will be an output image named "ftc_image_segmentation.png".
It is the result of image segmentation.
Second, it will be a txt file named "histogram.txt".
This is the list of the histogram ftc algorithm used.
Last, it will be a txt file named "index.txt".
This is the list of the minima ftc algorithm found.