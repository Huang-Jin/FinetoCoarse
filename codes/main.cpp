/*
 * Copyright (c) 2017-2018, Jin Huang <jin_huang@whu.edu.cn>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under, at your option, the terms of the GNU General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version, or
 * the terms of the simplified BSD license.
 *
 * You should have received a copy of these licenses along this
 * program. If not, see <http://www.gnu.org/licenses/> and
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

#include "demo.h"
#include "io_png.h"

#define VERSION "1.0"

#define USAGE "\
usage  : ftc input.png [eps] [-t] [-s sigma width] \n\
         ftc [-h|-v]\n\
\n\
         input.png   the name of input image\n\
                     if image has 1 channel, \n\
                     we will treat it as a gray image\n\
                     if image has 3 channel, \n\
                     we will treat it as a color image\n\
\n\
         eps     value of epsilon\n\
                 default is 1\n\
         -t      -t for using the third step\n\
                 the third step of ftc algorithm\n\
                 will cost lots of time\n\
         -s      -s for using smooth\n\
         sigma   the sigma used in smooth\n\
         width   the bandwidth used in smooth\n\
\n\
         -h      usage help\n\
         -v      version info\n"

int main(int argc, const char **argv)
{
	FTCParams param;
	param.bThird = false;
	param.bSmooth = false;
	param.eps = 1;

	/* wrong number of parameters */
	if (7 < argc || argc < 2) {
		fprintf(stderr, USAGE);
		return EXIT_FAILURE;
	}

	if (argc == 2)
	{
		/* "-v" : version info */
		if (0 == strcmp("-v", argv[1])) {
			fprintf(stdout, "%s version " VERSION
				", compiled " __DATE__ "\n", argv[0]);
			return EXIT_SUCCESS;
		}
		/* "-h" : usage help */
		if (0 == strcmp("-h", argv[1])) {
			fprintf(stdout, USAGE);
			return EXIT_SUCCESS;
		}
	}
	else if (argc == 3)
	{
		if (0 == strcmp("-t", argv[2])){
			param.bThird = true;
		}
		else param.eps = atof(argv[2]);
	}
	else if (argc == 4)
	{
		param.eps = atof(argv[2]);

		if (0 == strcmp("-t", argv[3])){
			param.bThird = true;
		}
		else
		{
			fprintf(stdout, USAGE);
			return EXIT_FAILURE;
		}
	}
	else if (argc == 5)
	{
		if (0 == strcmp("-s", argv[2])){
			param.bSmooth = true;
			param.sigma = atof(argv[3]);
			param.width = atof(argv[4]);
		}
		else
		{
			fprintf(stdout, USAGE);
			return EXIT_FAILURE;
		}
	}
	else if (argc == 6)
	{
		if (0 == strcmp("-t", argv[2])){
			param.bThird = true;
		}
		else
		{
			param.eps = atof(argv[2]);
		}

		if (0 == strcmp("-s", argv[3])){
			param.bSmooth = true;
			param.sigma = atof(argv[4]);
			param.width = atof(argv[5]);
		}
		else
		{
			fprintf(stdout, USAGE);
			return EXIT_FAILURE;
		}
	}
	else if (argc == 7)
	{
		param.eps = atof(argv[2]);

		if (0 == strcmp("-t", argv[3])){
			param.bThird = true;
		}
		else
		{
			fprintf(stdout, USAGE);
			return EXIT_FAILURE;
		}

		if (0 == strcmp("-s", argv[4])){
			param.bSmooth = true;
			param.sigma = atof(argv[5]);
			param.width = atof(argv[6]);
		}
		else
		{
			fprintf(stdout, USAGE);
			return EXIT_FAILURE;
		}
	}

	
	param.image = io_png_read_f32(argv[1], &param.nx, &param.ny, &param.nc);
	ftc_image_from_io_png(param);
	io_png_write_f32("ftc_image_segmentation.png", param.image, param.nx, param.ny, param.nc);

	free(param.image);

	return 0;
}
