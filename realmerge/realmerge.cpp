//! [includes]
#include "pch.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
//! [includes]

//! [namespace]
using namespace cv;
using namespace std;
//! [namespace]

int main(int argc, char** argv)
{
	//! [load]
	String imageName("leopard.jpg"); // by default
	String window_name("Workshop : Pyramids Demo"); // by default
	if (argc > 1)
	{
		imageName = argv[1];
	}
	//! [load]

	//! [mat]
	Mat image, tmp;
	//! [mat]

	//! [imread]
	image = imread(imageName, IMREAD_COLOR); // Read the file
	//! [imread]

	if (image.empty()) // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	tmp = image;

	//! [window]
	namedWindow(window_name, WINDOW_AUTOSIZE); // Create a window for display.
	//! [window]

	//! [imshow]
	imshow(window_name, tmp); // Show our image inside it.
	//! [imshow]

	int zoom = 0;
	while (true)
	{
		int c;

		c = waitKey(1);

		if (c == -1) continue;

		destroyAllWindows();

		if ((char)c == 'q')
			break;

		if ((char)c == '+')
		{
			zoom++;
			cout << " ** Zoom In: Image x 2" << endl;
		}
		if ((char)c == '-')
		{
			zoom--;
			cout << " ** Zoom Out: Image / 2" << endl;
		}

		if (zoom == 0) tmp = image;
		if (zoom > 0) resize(image, tmp, Size(image.cols << zoom, image.rows << zoom));
		if (zoom < 0) resize(image, tmp, Size(image.cols >> -zoom, image.rows >> -zoom));

		imshow(window_name, tmp);
	}
	return 0;
}
