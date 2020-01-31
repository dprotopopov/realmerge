//! [includes]
#include "pch.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

#include <iostream>
#include <filesystem>
//! [includes]

//! [namespace]
using namespace cv;
using namespace std;
using namespace experimental::filesystem;
//! [namespace]

int main(int argc, char** argv)
{
	cout << "Usage:" << endl;
	cout << "	realmerge [video [image]]" << endl;
	cout << "Keys:" << endl;
	cout << "	'+' Zoom In: Image x 2" << endl;
	cout << "	'-' Zoom Out: Image / 2" << endl;
	cout << "	'q' Quit" << endl;

	//! [load]
	String videoName("Madrid.mp4"); // by default
	String imageName("Madrid.jpg"); // by default
	String window_name("realmerge"); // by default

	int scale = 3;

	if (argc > 1)
	{
		videoName = argv[1];

		path p = argv[1];
		p.replace_extension(".jpg");
		imageName = p.stem().string();
	}

	if (argc > 2)
	{
		imageName = argv[2];
	}
	//! [load]

	// Create a VideoCapture object and open the input file
	// If the input is the web camera, pass 0 instead of the video file name
	VideoCapture cap;

	cap.open(videoName);

	// Check if camera opened successfully
	if (!cap.isOpened())
	{
		cout << "Error opening video stream or file" << endl;
		return -1;
	}
	//! [mat]
	Mat image, tmp;
	//! [mat]

	//! [imread]
	cap >> image;
	//! [imread]

	if (image.empty()) // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	Mat accumulate, current, scaled;
	image.convertTo(current, CV_64FC3, 1 / 255.0);
	resize(current, accumulate, Size((image.cols - 1) * scale + 1, (image.rows - 1) * scale + 1));

	long total_frames = 1;

	tmp = accumulate;

	//! [window]
	namedWindow(window_name, WINDOW_AUTOSIZE); // Create a window for display.
	//! [window]

	//! [imshow]
	imshow(window_name, tmp); // Show our image inside it.
	//! [imshow]

	int zoom = 0;

	while (true)
	{
		cap >> image;

		if (image.empty())
			break;

		image.convertTo(current, CV_64FC3, 1 / 255.0);
		resize(current, scaled, Size((image.cols - 1) * scale + 1, (image.rows - 1) * scale + 1));

		accumulate = (total_frames * accumulate + scaled) / (total_frames + 1);
		total_frames++;

		//for (int x = 0; x < image.cols; x++)
		//{
		//	for (int y = 0; y < image.rows; y++)
		//	{
		//		Vec3f &intensity = accumulate.at<Vec3f>(y, x);
		//		Vec3f &intensity1 = current.at<Vec3f>(y, x);
		//		for (int i = 0; i < 3; i++) intensity.val[i] = max(intensity.val[i], intensity1.val[i]);
		//	}
		//}

		int c;

		c = waitKey(1);

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

		if (zoom >= 0) resize(accumulate, tmp, Size(image.cols << zoom, image.rows << zoom));
		if (zoom < 0) resize(accumulate, tmp, Size(image.cols >> -zoom, image.rows >> -zoom));

		imshow(window_name, tmp);
	}

	// When everything done, release the video capture object
	cap.release();

	accumulate.convertTo(image, CV_8UC3, 255);
	imwrite(imageName, image);

	while (true)
	{
		int c;

		c = waitKey(1);

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

		if (zoom >= 0) resize(accumulate, tmp, Size(image.cols << zoom, image.rows << zoom));
		if (zoom < 0) resize(accumulate, tmp, Size(image.cols >> -zoom, image.rows >> -zoom));

		imshow(window_name, tmp);
	}


	// Closes all the frames
	destroyAllWindows();

	return 0;
}
