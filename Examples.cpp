#include "Header.h"

// Global Variables
cv::Mat src, erosion_dst, dilation_dst, dst;
int erosion_elem = 0;
int erosion_size = 0;
int dilation_elem = 0;
int dilation_size = 0;
int morph_elem = 0;
int morph_size = 0;
int morph_operator = 0;
int const max_operator = 4;
int const max_elem = 2;
int const max_kernel_size = 21;
std::string NameWindow = "Morphology Demo";

int low_r = 30, low_g = 30, low_b = 30;
int high_r = 100, high_g = 100, high_b = 100;

void examples::Example1()
{
	// This example show the use of the basic class ImageManager and creations and cofigurations of a trackbars

	// Read the image and show it
	cv::Mat imag1 = cv::imread("Datos/Imagenes/Coche1.jpg");
	if (imag1.empty()) { std::cout << "No image Load" << std::endl; return; }
	
	//cv:cvtColor(imag1, imag1, cv::ColorConversionCodes::COLOR_BGR2GRAY);
	
	cv::imshow("Imagen", imag1);
	std::cout << imag1.rows << "  " << imag1.cols << std::endl;
	while (true)
	{
		if (cv::waitKey(0) == 27) break;
	}
	cv::destroyAllWindows();

/**/	/// ----------------------------------------------     USING SIMPLE INSTANCING OF THE CLASS  ----------------------------------------------- ///

	// Window for the controls
	cv::Mat controls = cv::Mat(300, 300, CV_8SC3, cv::Scalar(125, 125, 125));
	cv::namedWindow("Controls", cv::WINDOW_AUTOSIZE);

	// Creating the object
	pcv::ImageManager myclass(imag1, 2);
	myclass.SetMax(2.0); myclass.SetMin(0.15); myclass.SetApplyChangesFinished(true);
	myclass.ShowHistogram(true);

	std::cout << "Max: " << myclass.GetMax() <<"\t Min: " << myclass.GetMin() << std::endl;

	//  ------------------------   CONFIGURATION TRACKBAR   ----------------------------  //

	// Add info to the Struct with the info for the trackbar
	pcv::InfoTrackbar mytrackbar = { "Scale", 200, 10, 100, (static_cast<double>(1) / static_cast<double>(100)) };
	mytrackbar.ptr_funct = pcv::Reseize;
	mytrackbar.Max = static_cast<int>(myclass.GetMax() / mytrackbar.conversationFactor );
	mytrackbar.Min = static_cast<int>(myclass.GetMin() / mytrackbar.conversationFactor);

	std::cout << "Max: " << mytrackbar.Max << "\t Min: " << mytrackbar.Min << "\t\t Current Scale: " << myclass.GetScaleImage() << std::endl;

	pcv::Info Data = { &mytrackbar ,&myclass };

	// Adding the current value to what we have to set to the trackbar
	mytrackbar.currentValue = ( static_cast<double>( myclass.GetScaleImage() ) / static_cast<double>( mytrackbar.conversationFactor ) );

	// Creating the and configuration of the trackbar
	cv::createTrackbar(mytrackbar.NameTrackbar,		// Name of the trackbar
						"Controls",					// Name of the window to put the trackbar
						&mytrackbar.currentValue,	// Actual value of trackbar
						mytrackbar.Max,				// Max value trackbar
						mytrackbar.ptr_funct,		// Callback function to trackbar
						//(void*)&myclass);
						//(void*)&Info);
						(void*)&Data);

	// Set the Min for the trackbar
	cv::setTrackbarMin(mytrackbar.NameTrackbar,
						"Controls",
						mytrackbar.Min);

	myclass.Update();
	myclass.Show();

	/// ----------------------------------------------     USING SIMPLE INSTANCING OF THE CLASS  ----------------------------------------------- ///

/**/

	while (true)
	{
		if (cv::waitKey(0) == 27) break;
	}
	myclass.~ImageManager();	// Invoke the destructor so we have the changes in the variable imag1
	cv::destroyAllWindows();
	
	if (!imag1.empty())
	{
		cv::imshow("Imagen", imag1); std::cout << imag1.rows << "  " << imag1.cols << std::endl;
	}
	else { std::cout << "No image" << std::endl; }
	std::cout << imag1.rows << "  " << imag1.cols << std::endl;

	while (true)
	{
		if (cv::waitKey(0) == 27) break;
	}
	cv::destroyAllWindows();
	

	std::cout << "Current Scale: " << myclass.GetScaleImage() << "  " << myclass.GetNameOut() << std::endl;
	std::cout << mytrackbar.Max << " " << mytrackbar.Min << " " << mytrackbar.NameTrackbar << " " << mytrackbar.conversationFactor << " "
			  << mytrackbar.currentValue << " " << std::endl;

	system("pause");

}

void examples::Example2()
{
	// This example is similar to Example1, the difference here that we use ptr to class in this case


	// Read the image and show it
	cv::Mat imag1 = cv::imread("Datos/Imagenes/Chrome.jpg");
	if (imag1.empty()) { std::cout << "No image Load" << std::endl; return; }

	cv::imshow("Imagen", imag1);
	std::cout << imag1.rows << "  " << imag1.cols << std::endl;
	while (true)
	{
		if (cv::waitKey(0) == 27) break;
	}
	cv::destroyAllWindows();

/**/

	/// -------------------------------------------------------     USING PTR  -------------------------------------------------------- ///
	
	// Window for the controls
	cv::Mat controls = cv::Mat(300, 300, CV_8SC3, cv::Scalar(125, 125, 125));
	cv::namedWindow("Controls", cv::WINDOW_AUTOSIZE);

	// Creating the object
	pcv::ImageManager* myclass = new pcv::ImageManager(imag1, 2.0);
	myclass->SetMax(2.0); myclass->SetMin(0.15); myclass->SetApplyChangesFinished(true);
	myclass->ShowHistogram(true);

	std::cout << "Max: " << myclass->GetMax() << "\t Min: " << myclass->GetMin() << std::endl;

	//  ------------------------   CONFIGURATION TRACKBAR   ----------------------------  //

	// Add info to the Struct with the info for the trackbar
	pcv::InfoTrackbar mytrackbar = { "Scale", 200, 10, 100, (static_cast<double>(1) / static_cast<double>(100)) };
	mytrackbar.ptr_funct = pcv::Reseize;
	mytrackbar.Max = static_cast<int>(myclass->GetMax() / mytrackbar.conversationFactor);
	mytrackbar.Min = static_cast<int>(myclass->GetMin() / mytrackbar.conversationFactor);

	std::cout << "Max: " << mytrackbar.Max << "\t Min: " << mytrackbar.Min << "\t\t Current Scale: " << myclass->GetScaleImage() << std::endl;

	pcv::Info Data = { &mytrackbar , myclass };

	// Adding the current value to what we have to set to the trackbar
	mytrackbar.currentValue = (static_cast<double>(myclass->GetScaleImage()) / static_cast<double>(mytrackbar.conversationFactor));

	// Creating the and configuration of the trackbar
	cv::createTrackbar(mytrackbar.NameTrackbar,		// Name of the trackbar
						"Controls",					// Name of the window to put the trackbar
						&mytrackbar.currentValue,	// Actual value of trackbar
						mytrackbar.Max,				// Max value trackbar
						mytrackbar.ptr_funct,		// Callback function to trackbar
						//(void*)&myclass);
						//(void*)&Info);
						(void*)&Data);

	// Set the Min for the trackbar
	cv::setTrackbarMin(mytrackbar.NameTrackbar,
						"Controls",
						mytrackbar.Min);

	myclass->Update();
	myclass->Show();

	while (true)
	{
	if (cv::waitKey(0) == 27) break;
	}
	delete myclass;
	cv::destroyAllWindows();

/**/

	/// -------------------------------------------------------     USING PTR  -------------------------------------------------------- ///


	if (!imag1.empty())
	{
		cv::imshow("Imagen", imag1); std::cout << imag1.rows << "  " << imag1.cols << std::endl;
	}
	else { std::cout << "No image" << std::endl; }
	std::cout << imag1.rows << "  " << imag1.cols << std::endl;

	while (true)
	{
		if (cv::waitKey(0) == 27) break;
	}
	cv::destroyAllWindows();

	std::cout << "Current Scale: " << myclass->GetScaleImage() << "  " << myclass->GetNameOut() << std::endl;
	std::cout << mytrackbar.Max << " " << mytrackbar.Min << " " << mytrackbar.NameTrackbar << " " << mytrackbar.conversationFactor << " "
			  << mytrackbar.currentValue << " " << std::endl;

	system("pause");

}

void examples::Example3()
{
	// This example show the use of the basic class ImageManager and creations and cofigurations of a trackbars
	
	// Read the image and show it
	cv::Mat imag1 = cv::imread("Datos/Imagenes/Chrome.jpg");
	if (imag1.empty()) { std::cout << "No image Load" << std::endl; return; }



	while (true)
	{
		if (cv::waitKey(0) == 27) break;
	}
	cv::destroyAllWindows();

}

void examples::Example4()
{

	// Read the image and show it
	cv::Mat imag1 = cv::imread("Datos/Imagenes/Chrome.jpg");
	if (imag1.empty()) { std::cout << "No image Load" << std::endl; return; }

	// Creating my class Contrast
	pcv::Contrast myclass(imag1, 2.0, 0.2, 0.0);
	pcv::ImageManager* ptr2myclass = new pcv::Contrast(imag1, 2.0, 0.2, 0.0);

	//pcv::Contrast* ptr = dynamic_cast<pcv::Contrast*>(ptr2myclass);

	//std::cout << dynamic_cast<pcv::Contrast*>(ptr2myclass)->GetContrast() << std::endl;
	//dynamic_cast<pcv::Contrast*>(ptr2myclass)->SetConstrast(1.5);


	//std::cout << dynamic_cast<pcv::Contrast*>(ptr2myclass)->GetContrast() << std::endl;


	std::cout << "Objeto: " << typeid(myclass).name() << std::endl;
	std::cout << "Objeto: " << typeid(ptr2myclass).name() << std::endl;

	examples::VerifyGetClassContrast(ptr2myclass);
	examples::VerifySetClassContrast(ptr2myclass);
	examples::VerifyGetClassContrast(ptr2myclass);

	dynamic_cast<Contrast*>(ptr2myclass)->Redraw();
	//ptr->Redraw();

	//examples::VerifyGetClassContrast(myclass);
	//examples::VerifySetClassContrast(myclass);
	//examples::VerifyGetClassContrast(myclass);

	//myclass.Redraw();

	//std::cout << myclass.GetScaleImage() << "\t" << myclass.GetContrast() << "\t" << myclass.GetBrightness() << std::endl;


	while (true)
	{
		if (cv::waitKey(0) == 27) break;
	}
	myclass.~Contrast();
	delete ptr2myclass;
	cv::destroyAllWindows();


	cv::imshow("Imagen", imag1);
	while (true)
	{
		if (cv::waitKey(0) == 27) break;
	}
	cv::destroyAllWindows();

	return;
}

void examples::Example5()
{
	// Read the image and show it
	cv::Mat imag1 = cv::imread("Datos/Imagenes/Monedas3.jpg");
	if (imag1.empty()) { std::cout << "No image Load" << std::endl; return; }
	
	cv::Mat output = imag1.clone();
	cv::cvtColor(output, output, cv::ColorConversionCodes::COLOR_BGR2GRAY);
	cv::Mat input = output.clone();

/**/
	// Window for the controls
	cv::Mat controls = cv::Mat(600, 600, CV_8SC3, cv::Scalar(125, 125, 125));
	cv::namedWindow("Controls", cv::WINDOW_AUTOSIZE);

	examples::Data dat;
	pcv::InfoTrackbar mytrackbar = { "Threshold", 255, 0, 100, 1.0 };
	mytrackbar.ptr_funct = examples::Threshold;

	dat.t = mytrackbar;	dat.out = output; dat.in = input;
	
	// Creating the and configuration of the trackbar
	cv::createTrackbar(mytrackbar.NameTrackbar,		// Name of the trackbar
						"Controls",					// Name of the window to put the trackbar
						&mytrackbar.currentValue,	// Actual value of trackbar
						mytrackbar.Max,				// Max value trackbar
						mytrackbar.ptr_funct,		// Callback function to trackbar
						(void*)&dat);

	// Set the Min for the trackbar
	cv::setTrackbarMin(mytrackbar.NameTrackbar,
						"Controls",
						mytrackbar.Min);
	
/**/
	
	cv::imshow("Monedas Original", imag1);	cv::imshow("Monedas Treshold", output);
	Threshold(100, &dat);
	while (true)
	{
		if (cv::waitKey(1) == 27) break;
	}
	cv::destroyAllWindows();

}

void examples::Example6()
{
	// Read the image and show it
	cv::Mat imag1 = cv::imread("Datos/Imagenes/Sudoku1.jpg");
	if (imag1.empty()) { std::cout << "No image Load" << std::endl; return; }

	cv::Mat output = imag1.clone();
	cv::cvtColor(output, output, cv::ColorConversionCodes::COLOR_BGR2GRAY);
	cv::Mat input = output.clone();


	cv::adaptiveThreshold(input, output, 255, cv::AdaptiveThresholdTypes::ADAPTIVE_THRESH_MEAN_C, 
												cv::ThresholdTypes::THRESH_BINARY, 5, -3);

	cv::imshow("Monedas Original", imag1);	cv::imshow("Monedas Treshold", output);
	while (true)
	{
		if (cv::waitKey(1) == 27) break;
	}
	cv::destroyAllWindows();
}

void examples::Example7()
{
	
	// Read the image and show it
	cv::Mat imag1 = cv::imread("Datos/Imagenes/Monedas3.jpg");
	if (imag1.empty()) { std::cout << "No image Load" << std::endl; return; }

	cv::Mat output = imag1.clone(); double act;
	
	//cv::medianBlur(imag1, output, 7);
	//cv::GaussianBlur(imag1, output, cv::Size(5, 5), 0.5, 0.5);
	cv::bilateralFilter(imag1, output, 5, 100, 100);
	
	cv::imshow("Monedas Original", imag1);	cv::imshow("Monedas Treshold", output);
	while (true)
	{
		if (cv::waitKey(1) == 27) break;
	}
	cv::destroyAllWindows();
}

void examples::Example8()
{
	// Read the image and show it
	cv::Mat imag1 = cv::imread("Datos/Imagenes/Monedas3.jpg");
	if (imag1.empty()) { std::cout << "No image Load" << std::endl; return; }
	
	int ddepth = CV_16S;
	cv::Mat output = imag1.clone();
	cv::Mat grad_x, grad_y, abs_x, abs_y, deriv;
	
	// Convert the image to grayscale
	cv::cvtColor(output, output, cv::COLOR_BGR2GRAY);

	//cv::Sobel(output, grad_x, -1, 1, 0, 3, 1.0, 0.0);		// Derivate 1º in X
	//cv::Sobel(output, grad_y, -1, 0, 1, 3, 1.0, 0.0);		// Derivate 1º in y

	cv::Sobel(output, grad_x, -1, 1, 0, CV_SCHARR, 1, 0);		// Derivate 1º in X using Scharr
	cv::Sobel(output, grad_y, -1, 0, 1, CV_SCHARR, 1, 0);		// Derivate 1º in y	using Scharr
		
	cv::convertScaleAbs(grad_x, abs_x);			// Convert to Abs Scale X
	cv::convertScaleAbs(grad_y, abs_y);			// Convert to Abs Scale X

	cv::addWeighted(grad_x, 0.5, grad_y, 0.5, 0, deriv);	// Mount derivate in x+y
	cv::Sobel(output, output, -1, 1, 1, 3, 1.0, 0.0);		// Mount derivate in x+y

	cv::Mat diff;
	cv::compare(deriv, output, diff, cv::CMP_NE);	// Compare the two first image and save the difference in diff
	int nz = cv::countNonZero(diff);				// Count the number of Zero
	
	cout << nz << endl;

	cv::imshow("Monedas Original", output);	cv::imshow("Sobel in X", deriv); // cv::imshow("Sobel in Y", abs_y);
	while (true)
	{
		if (cv::waitKey(1) == 27) break;
	}
	cv::destroyAllWindows();
}

void examples::Example9()
{
	// Read the image and show it
	src = cv::imread("Datos/Imagenes/Monedas3.jpg");
	if (src.empty()) { std::cout << "No image Load" << std::endl; return; }

	cv::namedWindow("Erosion Demo", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("Dilation Demo", cv::WINDOW_AUTOSIZE);

	// Create Erosion Trackbar
	cv::createTrackbar("Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Erosion Demo",
						&erosion_elem, max_elem,
						Erosion);

	cv::createTrackbar("Kernel size:\n 2n +1", "Erosion Demo",
						&erosion_size, max_kernel_size,
						Erosion);

	// Create Dilation Trackbar
	cv::createTrackbar("Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Dilation Demo",
						&dilation_elem, max_elem,
						Dilation);

	cv::createTrackbar("Kernel size:\n 2n +1", "Dilation Demo",
						&dilation_size, max_kernel_size,
						Dilation);


	/// Default start
	Erosion(0, 0);
	Dilation(0, 0);

	while (true)
	{
		if (cv::waitKey(1) == 27) break;
	}
	cv::destroyAllWindows();
}

void examples::Example10()
{
	// Read the image and show it
	src = cv::imread("Datos/Imagenes/Monedas3.jpg");
	if (src.empty()) { std::cout << "No image Load" << std::endl; return; }
	
	cv::namedWindow(NameWindow, cv::WINDOW_AUTOSIZE);
	

	//![create_trackbar1]
	/// Create Trackbar to select Morphology operation
	cv::createTrackbar("Operator:\n 0: Opening - 1: Closing  \n 2: Gradient - 3: Top Hat \n 4: Black Hat", 
						NameWindow, &morph_operator, max_operator, Morphology_Operations);
	//![create_trackbar1]

	//![create_trackbar2]
	/// Create Trackbar to select kernel type
	cv::createTrackbar("Element:\n 0: Rect - 1: Cross - 2: Ellipse", NameWindow,
						&morph_elem, max_elem,
						Morphology_Operations);
	//![create_trackbar2]

	//![create_trackbar3]
	/// Create Trackbar to choose kernel size
	cv::createTrackbar("Kernel size:\n 2n +1", NameWindow,
						&morph_size, max_kernel_size,
						Morphology_Operations);
	//![create_trackbar3]


	/// Default start
	Morphology_Operations(0, 0);

	while (true)
	{
		if (cv::waitKey(1) == 27) break;
	}
	cv::destroyAllWindows();
}

void examples::Example11()
{
	const int rate = 50;
	cv::Mat input_image = (cv::Mat_<uchar>(8, 8) <<
							0, 0, 0, 0, 0, 0, 0, 0,
							0, 255, 255, 255, 0, 0, 0, 255,
							0, 255, 255, 255, 0, 0, 0, 0,
							0, 255, 255, 255, 0, 255, 0, 0,
							0, 0, 255, 0, 0, 0, 0, 0,
							0, 0, 255, 0, 0, 255, 255, 0,
							0, 255, 0, 255, 0, 0, 255, 0,
							0, 255, 255, 255, 0, 0, 0, 0);
	
	cv::Mat kernel = (cv::Mat_<int>(3, 3) <<
						0, 1, 0,
						1, -1, 1,
						0, 1, 0);

	cv::Mat output_image;
	
	cv::morphologyEx(input_image, output_image, cv::MORPH_HITMISS, kernel);

	kernel = (kernel + 1) * 127;
	kernel.convertTo(kernel, CV_8U);
	
	cv::resize(kernel, kernel, cv::Size(), rate, rate, cv::INTER_NEAREST);
	cv::imshow("kernel", kernel);
	cv::moveWindow("kernel", 0, 0);
	
	cv::resize(input_image, input_image, cv::Size(), rate, rate, cv::INTER_NEAREST);
	cv::imshow("Original", input_image);
	cv::moveWindow("Original", 0, 200);
	
	cv::resize(output_image, output_image, cv::Size(), rate, rate, cv::INTER_NEAREST);
	cv::imshow("Hit or Miss", output_image);
	cv::moveWindow("Hit or Miss", 500, 200);

	while (true)
	{
		if (cv::waitKey(1) == 27) break;
	}
	cv::destroyAllWindows();

}

void examples::Example12()
{

	cv::Mat frame, frame_threshold;
	cv::VideoCapture cap(0);
	cv::namedWindow("Video Capture", cv::WINDOW_NORMAL);
	cv::namedWindow("Object Detection", cv::WINDOW_NORMAL);
	
	//-- Trackbars to set thresholds for RGB values
	cv::createTrackbar("Low R", "Object Detection", &low_r, 255, on_low_r_thresh_trackbar);
	cv::createTrackbar("High R", "Object Detection", &high_r, 255, on_high_r_thresh_trackbar);
	cv::createTrackbar("Low G", "Object Detection", &low_g, 255, on_low_g_thresh_trackbar);
	cv::createTrackbar("High G", "Object Detection", &high_g, 255, on_high_g_thresh_trackbar);
	cv::createTrackbar("Low B", "Object Detection", &low_b, 255, on_low_b_thresh_trackbar);
	cv::createTrackbar("High B", "Object Detection", &high_b, 255, on_high_b_thresh_trackbar);
	
	while ( (char)cv::waitKey(1) != 'q') {
		cap >> frame;
		if (frame.empty())
			break;
		//-- Detect the object based on RGB Range Values
		inRange(frame, cv::Scalar(low_b, low_g, low_r), cv::Scalar(high_b, high_g, high_r), frame_threshold);
		//-- Show the frames
		cv::imshow("Video Capture", frame);
		cv::imshow("Object Detection", frame_threshold);

	}

}

void examples::Example13()
{
	// Declare the variables we are going to use
	cv::Mat src, src_gray, dst;
	int kernel_size = 3;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	std::string window_name = "Laplace Demo";

	src = cv::imread("Datos/Imagenes/Monedas3.jpg", cv::ImreadModes::IMREAD_COLOR); // Load an image
	if (src.empty()) { std::cout << "No image Load" << std::endl; return; }
	
	// Reduce noise by blurring with a Gaussian filter ( kernel size = 3 )
	cv::GaussianBlur(src, src, cv::Size(3, 3), 0, 0, cv::BorderTypes::BORDER_DEFAULT);
	cvtColor(src, src_gray, cv::ColorConversionCodes::COLOR_BGR2GRAY); // Convert the image to grayscale
	
	cv::Mat abs_dst;
	cv::Laplacian(src_gray, dst, ddepth, kernel_size, scale, delta, cv::BorderTypes::BORDER_DEFAULT);
	
	// converting back to CV_8U
	cv::convertScaleAbs(dst, abs_dst);
	
	cv::imshow(window_name, abs_dst);	cv::imshow("Original", src);
	while (true)
	{
		if (cv::waitKey(1) == 27) break;
	}
	cv::destroyAllWindows();
}

void examples::Example14()
{
	
	src = cv::imread("Datos/Imagenes/notes.png"); // Load an image
	if (src.empty()) { std::cout << "No image Load" << std::endl; return; }

	// Show source image
	cv::imshow("src", src);

	// Transform source image to gray if it is not already
	cv::Mat gray, bw;
	if (src.channels() == 3)
		cvtColor(src, gray, CV_BGR2GRAY);
	else
		gray = src;
	
	// Show gray image
	show_wait_destroy("gray", gray);
	// Apply adaptiveThreshold at the bitwise_not of gray, notice the ~ symbol
	adaptiveThreshold(~gray, bw, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 15, -2);
	
	// Show binary image
	show_wait_destroy("binary", bw);
	
	// Create the images that will use to extract the horizontal and vertical lines
	cv::Mat horizontal = bw.clone();  cv::Mat vertical = bw.clone();

	// Specify size on horizontal and vertical axis
	int horizontal_size = horizontal.cols / 30;		int vertical_size = vertical.rows / 30;
	// Create structure element for extracting horizontal lines through morphology operations
	cv::Mat horizontalStructure = cv::getStructuringElement(cv::MorphShapes::MORPH_RECT, cv::Size(horizontal_size, 1));
	// Apply morphology operations
	cv::erode(horizontal, horizontal, horizontalStructure, cv::Point(-1, -1));
	cv::dilate(horizontal, horizontal, horizontalStructure, cv::Point(-1, -1));
	
	// Show extracted horizontal lines
	show_wait_destroy("horizontal", horizontal);
	// Create structure element for extracting vertical lines through morphology operations
	cv::Mat verticalStructure = cv::getStructuringElement(cv::MorphShapes::MORPH_RECT, cv::Size(1, vertical_size));
	// Apply morphology operations
	cv::erode(vertical, vertical, verticalStructure, cv::Point(-1, -1));
	cv::dilate(vertical, vertical, verticalStructure, cv::Point(-1, -1));
	// Show extracted vertical lines
	show_wait_destroy("vertical", vertical);
	// Inverse vertical image
	bitwise_not(vertical, vertical);
	show_wait_destroy("vertical_bit", vertical);
	// Extract edges and smooth image according to the logic
	// 1. extract edges
	// 2. dilate(edges)
	// 3. src.copyTo(smooth)
	// 4. blur smooth img
	// 5. smooth.copyTo(src, edges)
	// Step 1
	cv::Mat edges;
	cv::adaptiveThreshold(vertical, edges, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 3, -2);
	show_wait_destroy("edges", edges);
	// Step 2
	cv::Mat kernel = cv::Mat::ones(2, 2, CV_8UC1);
	cv::dilate(edges, edges, kernel);
	show_wait_destroy("dilate", edges);
	// Step 3
	cv::Mat smooth;
	vertical.copyTo(smooth);
	// Step 4
	cv::blur(smooth, smooth, cv::Size(2, 2));
	// Step 5
	smooth.copyTo(vertical, edges);
	// Show final result
	show_wait_destroy("smooth - final", vertical);
}

void examples::Example15()
{
	cv::Mat gray, source;
	//source = cv::imread("Datos/Imagenes/smarties.png");			// Load an image
	source = cv::imread("Datos/Imagenes/Monedas2.jpg");			// Load an image
	if (source.empty()) { std::cout << "No image Load" << std::endl; return; }

	cv::cvtColor(source, gray, cv::ColorConversionCodes::COLOR_BGR2GRAY);
	cv::medianBlur(gray, gray, 7);
	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 1,
						gray.rows / 6,			// change this value to detect circles with different distances to each other
						100, 30, 20, 95 );		// change the last two parameters
												// (min_radius & max_radius) to detect larger circles

	for (size_t i = 0; i < circles.size(); i++)
	{
		cv::Vec3i c = circles[i];
		cv::Point center = cv::Point(c[0], c[1]);
		// circle center
		cv::circle(source, center, 1, cv::Scalar(255, 255, 255), 3, cv::LineTypes::LINE_AA);
		// circle outline
		int radius = c[2];
		cv::circle(source, center, radius, cv::Scalar(255, 0, 255), 3, cv::LineTypes::LINE_AA);
	}
	
	cv::imshow("detected circles", source);
	while (true)
	{
		if (cv::waitKey(1) == 27) break;
	}
	cv::destroyAllWindows();

}

void examples::Example16()
{
	cv::Mat Lut(1, 256, CV_8U); 	cv::Mat Lut2(1, 256, CV_32F);
	float mydato;
	for (int i = 0; i < 256; i++)
	{
		//Lut.at< uchar >(i) = 255 - i;
		float mydato = pow((float)i, (float)3.0) / (255 * 255);
		uchar data = pow((float)i, (float)3.0) / (255 * 255);	// Raiz cubica
		Lut.at< uchar >(i) = data;
		Lut2.at< float >(i) = mydato;
	}
	cout << Lut << "\n\n" << endl;
	cout << Lut2 << "\n\n" << endl;
	


	
	cv::waitKey(0);
}

void examples::Example17()
{
	// Read the image
	cv::Mat imag1 = cv::imread("Datos/Imagenes/Coche3.jpg");
	if (imag1.empty()) { std::cout << "No image Load" << std::endl; return; }

	/// ----------------------- Calculate the histogram of the image -------------------------------

	// Split each channel toward the vector --->  bgr[0]->B_channel ; bgr[1]->G_channel ; bgr[2]->R_channel
	std::vector<Mat> bgr_planes;
	cv::split(imag1, bgr_planes);

	// Variables para el histograma
	int histSize = 256;

	// los rangos (A,V,R) 
	float range[] = { 0, 256 };
	const float* histRange = { range };

	//bool uniform = true;
	//bool accumulate = false;

	Mat b_hist, g_hist, r_hist;

	// Calculate the histogram
	cv::calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange);
	cv::calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange);
	cv::calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange);

	//cout << "hist: " << b_hist << endl;

	/// ----------------------- Calculate the histogram of the image -------------------------------

	/// --------------------------- Draw the histogram of the image --------------------------------

	// Dibujar el histograms para A, V y R
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound( (double)hist_w / histSize);

	cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));

	/// Normalizar el resultado a [ 0, histImage.rows ]
	cv::normalize(b_hist, b_hist,
					0,							// Min
					histImage.rows,				// Max
					cv::NORM_MINMAX, -1, Mat());
	cv::normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, Mat());
	cv::normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, Mat());

	//cout << "hist: " << b_hist << endl;

	/// Draw for each channel
	for (int i = 1; i < histSize; i++)
	{
		line(histImage, cv::Point( bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1) ) ),
						cv::Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
			cv::Scalar(255, 0, 0), 2, 8, 0);

		line(histImage, cv::Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
						cv::Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
						cv::Scalar(0, 255, 0), 2, 8, 0);

		line(histImage, cv::Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
						cv::Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
						cv::Scalar(0, 0, 255), 2, 8, 0);
	}

	/// --------------------------- Draw the histogram of the image --------------------------------

	// Showing the images
	cv::imshow("Original", imag1); cv::imshow("Histograma", histImage);

	while (true)
		if (cv::waitKey(1) == 27) break;

	cv::destroyAllWindows();
}

void examples::Example18()
{
	// Read the images
	cv::Mat imag1 = cv::imread("Datos/Imagenes/IMG.jpg");
	if (imag1.empty()) { std::cout << "No image Load" << std::endl; return; }

	cv::Mat templ = cv::imread("Datos/Imagenes/MM.jpg");
	if (templ.empty()) { std::cout << "No image Load" << std::endl; return; }

	Mat destiny;

	//Reservamos memoria para los diversos metodos
	int iwidth = imag1.cols - templ.cols + 1;
	int iheight = imag1.rows - templ.rows + 1;

	destiny.create(iheight, iwidth, CV_32FC1);
	int match_method = CV_TM_SQDIFF_NORMED;

	// Correlacion	
	cv::matchTemplate(imag1, templ, destiny, match_method);
	normalize(destiny, destiny, 0, 1, cv::NORM_MINMAX, -1, Mat());

	double minVal, maxMal;
	cv::Point minLoc, maxLoc;
	cv::Point matchLoc;

	cv::minMaxLoc(destiny, &minVal, &maxMal, &minLoc, &maxLoc, Mat());
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
		matchLoc = minLoc;
	else
		matchLoc = maxLoc;

	cv::rectangle(imag1, matchLoc, cv::Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), cv::Scalar(255, 0, 0), 4, 8, 0);
	//cv::rectangle(destiny, matchLoc, cv::Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), cv::Scalar(0, 0, 0), 4, 8, 0);
	cv::rectangle(destiny, cv::Point(matchLoc.x - (templ.cols / 2), matchLoc.y - (templ.rows / 2)),
						   cv::Point(matchLoc.x + (templ.cols / 2), matchLoc.y + (templ.rows / 2)), cv::Scalar::all(0), 4, 8, 0);

	cv::imshow("src", imag1);	cv::imshow("Result", destiny);	cv::imshow("templ", templ);



	while (true)
		if (cv::waitKey(1) == 27) break;

	cv::destroyAllWindows();
}

void examples::Example19()
{
	// This example show the use of the basic class Contrast and creations and cofigurations of the trackbars

	// Read the image and show it
	cv::Mat imag1 = cv::imread("Datos/Imagenes/Coche1.jpg");
	if (imag1.empty()) { std::cout << "No image Load" << std::endl; return; }

/**/	/// ----------------------------------------------     USING SIMPLE INSTANCING OF THE CLASS  ----------------------------------------------- ///

	// Window for the controls
	cv::Mat controls = cv::Mat(300, 300, CV_8SC3, cv::Scalar(125, 125, 125));
	cv::namedWindow("Controls", cv::WINDOW_AUTOSIZE);

	// Creating the object
	pcv::Contrast myclass(imag1);
	myclass.SetApplyChangesFinished(true); myclass.ShowHistogram(true);
	myclass.SetMaxGamma(2.0); myclass.SetMinGamma(0.10);
	myclass.SetModeOperation(pcv::Operation_Mode::GAMACORRECTION);

	
	//  ------------------------   CONFIGURATION TRACKBAR'S   ----------------------------  //

	// Add info to the Struct with the info for the first trackbar
	pcv::InfoTrackbar trackbarSize = { "Scale", 200, 10, 100, (static_cast<double>(1) / static_cast<double>(100)) };
	trackbarSize.ptr_funct = pcv::Reseize;
	trackbarSize.Max = static_cast<int>(myclass.GetMax() / trackbarSize.conversationFactor);
	trackbarSize.Min = static_cast<int>(myclass.GetMin() / trackbarSize.conversationFactor);

	pcv::Info DataSize = { &trackbarSize ,&myclass };

	// Adding the current value to what we have to set to the trackbar
	trackbarSize.currentValue = (static_cast<double>(myclass.GetScaleImage()) / static_cast<double>(trackbarSize.conversationFactor));

	// Creating the and configuration of the trackbar
	cv::createTrackbar(trackbarSize.NameTrackbar,		// Name of the trackbar
						"Controls",					// Name of the window to put the trackbar
						&trackbarSize.currentValue,	// Actual value of trackbar
						trackbarSize.Max,				// Max value trackbar
						trackbarSize.ptr_funct,		// Callback function to trackbar
									//(void*)&myclass);
									//(void*)&Info);
						(void*)&DataSize);

	// Set the Min for the trackbar
	cv::setTrackbarMin(trackbarSize.NameTrackbar,
						"Controls",
						trackbarSize.Min);


	// Add info to the Struct with the info for the second trackbar
	pcv::InfoTrackbar trackbarGamma = { "Gamma", 200, 10, 100, (static_cast<double>(1) / static_cast<double>(100)) };
	trackbarGamma.ptr_funct = pcv::Change_Gamma;
	trackbarGamma.Max = static_cast<int>(myclass.GetMaxGamma() / trackbarGamma.conversationFactor);
	trackbarGamma.Min = static_cast<int>(myclass.GetMinGamma() / trackbarGamma.conversationFactor);

	pcv::Info DataGamma = { &trackbarGamma ,&myclass };

	// Adding the current value to what we have to set to the trackbar
	trackbarGamma.currentValue = (static_cast<double>(myclass.GetGamma()) / static_cast<double>(trackbarGamma.conversationFactor));

	// Creating the and configuration of the trackbar
	cv::createTrackbar(trackbarGamma.NameTrackbar,		// Name of the trackbar
						"Controls",						// Name of the window to put the trackbar
						&trackbarGamma.currentValue,	// Current value of trackbar
						trackbarGamma.Max,				// Max value trackbar
						trackbarGamma.ptr_funct,		// Callback function to trackbar
									//(void*)&myclass);
									//(void*)&Info);
						(void*)&DataGamma);

	// Set the Min for the trackbar
	cv::setTrackbarMin(trackbarGamma.NameTrackbar,
						"Controls",
						trackbarGamma.Min);


	myclass.Update();
	myclass.Show();

	/// ----------------------------------------------     USING SIMPLE INSTANCING OF THE CLASS  ----------------------------------------------- ///

/**/

	while (true)
	{
		if (cv::waitKey(0) == 27) break;
	}
	myclass.~Contrast();		// Invoking destructor so we have the changes in the variable imag1
	cv::destroyAllWindows();

	if (!imag1.empty())
	{
		cv::imshow("Imagen", imag1); std::cout << imag1.rows << "  " << imag1.cols << std::endl;
	}
	else { std::cout << "No image" << std::endl; }
	std::cout << imag1.rows << "  " << imag1.cols << std::endl;

	while (true)
	{
		if (cv::waitKey(0) == 27) break;
	}
	cv::destroyAllWindows();

}

void examples::Example20()
{
	cv::Mat frame;
	pcv::ImageManager myclass(frame);

	cv::VideoCapture cap;
	//cap.open("Datos/Videos/Samsung Galaxy NOTE 8.mp4");
	cap.open("Datos/Videos/Bee.mp4");
	
	//cap.open(0);
	int framerate = 33;

	myclass.SetImage(frame);
	myclass.SetMax(1.0); myclass.SetMin(0.15); myclass.SetApplyChangesFinished(false);
	myclass.ShowHistogram(true); myclass.SetModeFile(1);
	myclass.SetScaleImage(0.4);

	if (cap.isOpened()) 
	{
		for (;;)
		{
	//		
	//		// wait for a new frame from camera and store it into 'frame'
	//		//cap.read(frame);
			cap >> frame;

			// check if we succeeded
			if (frame.empty()) {
				std::cerr << "ERROR! blank frame grabbed\n"; break;
			}	

			myclass.SetImage(frame);

			cv::waitKey(framerate);
			myclass.Update();
			myclass.Show();

		}
	}

	else
	{
		std::cout << "ERROR! Unable to open the video\n";
		cv::waitKey(50000);
		return;
	}
}

void examples::VerifyGetClassContrast(pcv::Contrast& RecClass)
{
	std::cout << std::endl;
	std::cout << "Scale: " << RecClass.GetScaleImage() << "\t" << std::endl;
	std::cout << "Contrast: " << RecClass.GetContrast() << "\tBrightness: " << RecClass.GetBrightness() << "\tGamma: " << RecClass.GetGamma() << std::endl;
	std::cout << "Max Contrast: " << RecClass.GetMaxConstrast() << "\tMin Contrast: " << RecClass.GetMinConstrast() << std::endl;
	std::cout << "Max Brightness: " << RecClass.GetMaxBrightness() << "\tMin Brightness: " << RecClass.GetMinBrightness() << std::endl;
	std::cout << "Max Gamma: " << RecClass.GetMaxGamma() << "\t\tMin Gamma: " << RecClass.GetMinGamma() << std::endl;
	std::cout << (RecClass.GetOperationMode() == pcv::Operation_Mode::CONTRAST_AND_BRIGHTNESS ? "Mode Contrast" : "Mode Gamma") << std::endl;
}

void examples::VerifySetClassContrast(pcv::Contrast& RecClass)
{
	RecClass.SetConstrast(1.15).SetBrightness(1.15).SetGamma(0.1).SetModeOperation(pcv::Operation_Mode::GAMACORRECTION).SetApplyChangesFinished(true);
	RecClass.SetMaxConstrast(1.5); RecClass.SetMinConstrast(0.1);
	RecClass.SetMaxBrightness(1.5); RecClass.SetMinBrightness(0.1);
	RecClass.SetMaxGamma(1.5); RecClass.SetMinGamma(0.1);
	RecClass.SetMax(2.0); RecClass.SetScaleImage(1.5);
}

void examples::VerifyGetClassContrast(pcv::ImageManager* RecClass)
{
	pcv::Contrast* ptr = dynamic_cast<pcv::Contrast*>(RecClass);
	
	std::cout << std::endl;
	std::cout << "Scale: " << ptr->GetScaleImage() << "\t" << std::endl;
	std::cout << "Contrast: " << ptr->GetContrast() << "\tBrightness: " << ptr->GetBrightness() << "\tGamma: " << ptr->GetGamma() << std::endl;
	std::cout << "Max Contrast: " << ptr->GetMaxConstrast() << "\tMin Contrast: " << ptr->GetMinConstrast() << std::endl;
	std::cout << "Max Brightness: " << ptr->GetMaxBrightness() << "\tMin Brightness: " << ptr->GetMinBrightness() << std::endl;
	std::cout << "Max Gamma: " << ptr->GetMaxGamma() << "\t\tMin Gamma: " << ptr->GetMinGamma() << std::endl;
	std::cout << (ptr->GetOperationMode() == pcv::Operation_Mode::CONTRAST_AND_BRIGHTNESS ? "Mode Contrast" : "Mode Gamma") << std::endl;
}

void examples::VerifySetClassContrast(pcv::ImageManager* RecClass)
{
	pcv::Contrast* ptr = dynamic_cast<pcv::Contrast*>(RecClass);

	ptr->SetConstrast(0.25).SetBrightness(1.15).SetGamma(0.1).SetModeOperation(pcv::Operation_Mode::CONTRAST_AND_BRIGHTNESS).SetApplyChangesFinished(true);
	ptr->SetMaxConstrast(1.5); ptr->SetMinConstrast(0.1);
	ptr->SetMaxBrightness(1.5); ptr->SetMinBrightness(0.1);
	ptr->SetMaxGamma(1.5); ptr->SetMinGamma(0.1);
	ptr->SetMax(2.0); ptr->SetScaleImage(1.5);
}



static void examples::Threshold(int _value, void* dat)
{
	
	cv::destroyWindow("Monedas Treshold");
	examples::Data& Data = *(examples::Data*)dat;
	
	// Apply the threshold
	Data.t.currentValue = _value;
	double act = static_cast<double>(Data.t.currentValue) * Data.t.conversationFactor;
	act = cv::threshold(Data.in, Data.out, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU );
	//cv::threshold(Data.in, Data.out, 0, act, CV_THRESH_BINARY );
	cv::imshow("Monedas Treshold", Data.out);
	cout << act << "  " << Data.t.currentValue << endl;

}

void examples::Erosion(int, void*)
{
	int erosion_type = 0;
	if (erosion_elem == 0) { erosion_type = cv::MORPH_RECT; }
	else if (erosion_elem == 1) { erosion_type = cv::MORPH_CROSS; }
	else if (erosion_elem == 2) { erosion_type = cv::MORPH_ELLIPSE; }
	cv::Mat element = cv::getStructuringElement(erosion_type, cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
												cv::Point(erosion_size, erosion_size) );
	cv::erode(src, erosion_dst, element);
	cv::imshow("Erosion Demo", erosion_dst);
}

void examples::Dilation(int, void*)
{
	int dilation_type = 0;
	if (dilation_elem == 0) { dilation_type = cv::MORPH_RECT; }
	else if (dilation_elem == 1) { dilation_type = cv::MORPH_CROSS; }
	else if (dilation_elem == 2) { dilation_type = cv::MORPH_ELLIPSE; }
	cv::Mat element = cv::getStructuringElement(dilation_type, cv::Size(2 * dilation_size + 1, 2 * dilation_size + 1),
												cv::Point(dilation_size, dilation_size));
	cv::dilate(src, dilation_dst, element);
	cv::imshow("Dilation Demo", dilation_dst);
}

void examples::Morphology_Operations(int, void*)
{
	// Since MORPH_X : 2,3,4,5 and 6
	
	//![operation]
	int operation = morph_operator + 2;
	//![operation]

	cv::Mat element = cv::getStructuringElement(morph_elem, cv::Size(2 * morph_size + 1, 2 * morph_size + 1), 
												cv::Point(morph_size, morph_size));

	/// Apply the specified morphology operation
	morphologyEx(src, dst, operation, element);
	imshow(NameWindow, dst);
}

void examples::on_low_r_thresh_trackbar(int, void *)
{
	low_r = cv::min(high_r - 1, low_r);
	cv::setTrackbarPos("Low R", "Object Detection", low_r);
}
void examples::on_high_r_thresh_trackbar(int, void *)
{
	high_r = cv::max(high_r, low_r + 1);
	cv::setTrackbarPos("High R", "Object Detection", high_r);
}
void examples::on_low_g_thresh_trackbar(int, void *)
{
	low_g = cv::min(high_g - 1, low_g);
	cv::setTrackbarPos("Low G", "Object Detection", low_g);
}
void examples::on_high_g_thresh_trackbar(int, void *)
{
	high_g = cv::max(high_g, low_g + 1);
	cv::setTrackbarPos("High G", "Object Detection", high_g);
}
void examples::on_low_b_thresh_trackbar(int, void *)
{
	low_b = cv::min(high_b - 1, low_b);
	cv::setTrackbarPos("Low B", "Object Detection", low_b);
}
void examples::on_high_b_thresh_trackbar(int, void *)
{
	high_b = cv::max(high_b, low_b + 1);
	cv::setTrackbarPos("High B", "Object Detection", high_b);
}

void examples::show_wait_destroy(const char* winname, cv::Mat img) {
	cv::imshow(winname, img);
	cv::moveWindow(winname, 500, 0);
	cv::waitKey(0);
	cv::destroyWindow(winname);
}