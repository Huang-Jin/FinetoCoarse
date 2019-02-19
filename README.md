## A C++ Implementation of Fine-to-Coarse Algorithm for Histogram-based Image Segmentation

Here we provide the source code and images used in the paper of "A C++ Implementation of Fine-to-Coarse Algorithm for Histogram-based Image Segmentation".
It was written by Jin Huang and Julie Delon.

## Requirements

Building and using requires the following libraries

    libpng 1.2.54

The versions match the configuration we have tested on an ubuntu 16.04 system.

## Compile

To compile the codes in Linux,

```
cd ./codes
make -j8
```

## Usage

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
