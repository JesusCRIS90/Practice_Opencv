
#include "Header.h"

#pragma region METHODS CLASS IMAGE_MANAGER

pcv::ImageManager::ImageManager()
{
	//std::cout << "En Based Class" << std::endl;
	SetNameOut("Output").SetScaleImage(1).SetApplyChangesFinished(false);		// Call in cascade
	ShowHistogram(false);	SetHistogramInfo();
	SetModeFile(2);	// Modo Imagen
}

pcv::ImageManager::ImageManager(cv::Mat& _imag, double _scale, bool _ApplyChangesFinished)
{
	SetImage(_imag).SetNameOut("Output").SetScaleImage(_scale).SetApplyChangesFinished(_ApplyChangesFinished);		// Call in cascade
	ShowHistogram(false);	SetHistogramInfo();
	SetModeFile(2);	// Modo Imagen
}

pcv::ImageManager::~ImageManager()
{
	if (GetApplyChangesFinished() == true)
		*ptr2orig = output.clone();
}

void pcv::ImageManager::SetHistogramInfo()
{
	// First Set the info for histogram
	this->Hist.Info_Hist_Data.histSize = 256;
	this->Hist.Info_Hist_Data.range[0] = 0;  this->Hist.Info_Hist_Data.range[1] = 256;
	this->Hist.Info_Hist_Data.histRange = { this->Hist.Info_Hist_Data.range };
	// After, Set the info for the window
	this->Hist.Info_Hist_Window_Data.width = 512;
	this->Hist.Info_Hist_Window_Data.high  = 400;
	this->Hist.Info_Hist_Window_Data.bin_w = cvRound((double)Hist.Info_Hist_Window_Data.width / Hist.Info_Hist_Data.histSize);
	this->Hist.Info_Hist_Window_Data.histWindow = cv::Mat(this->Hist.Info_Hist_Window_Data.high,
															this->Hist.Info_Hist_Window_Data.width,
															CV_8UC3, cv::Scalar(255, 255, 255));

}

pcv::ImageManager& pcv::ImageManager::SetScaleImage(double _value)
{
	if (_value < this->MinValueScale) { this->scaleImage = this->MinValueScale; }
	else if (_value > this->MaxValueScale) { this->scaleImage = this->MaxValueScale; }
	else { this->scaleImage = _value; }
	return *this;
}

void pcv::ImageManager::Show()
{
	if (this->modoArchivo == 2)
	{
		if (this->original.empty()) { std::cout << "No Input Image" << std::endl; return; }
		cv::imshow("Input", this->original);
	}

	if (this->output.empty()) { std::cout << "No Output Image" << std::endl; return; }
	cv::imshow( GetNameOut(), (*this).output);
	
	if (GetShowHistogram() == true)
		cv::imshow("Histogram", this->Hist.Info_Hist_Window_Data.histWindow);

}

void pcv::ImageManager::Update()
{
	//std::cout << "IN CLASS IMAGE MANAGER" << std::endl;
	if (this->modoArchivo == 2)
	{
		cv::destroyWindow(GetNameOut());
	}
	//cv::destroyWindow("Histogram");

	// FIRST, RESET THE IMAGE IMPUT
	ResetOutput(); /* VerifyScale(); */
	// SECOND, APPLY THE SCALE FACTOR
	ApplyScale();
	// FINALLY VERIFY IF WE HAVE TO MAKE THE HISTOGRAM
	if ( GetShowHistogram() == true)
	{
		CalculateHistogram();
		DrawHistogram();
	}
}

void pcv::ImageManager::VerifyScale()
{
	if ( GetScaleImage() < this->MinValueScale) { SetScaleImage(GetMin()); }
	if ( GetScaleImage() > this->MaxValueScale) { SetScaleImage(GetMax()); }
}

void pcv::ImageManager::SetMax(double _value)
{
	if (_value < 0){ std::cout << " Not allowed Negative values " << std::endl; }
	else
	{
		if (_value >= (GetMin() + 0.01)) { static_cast<double>(MaxValueScale) = _value;  VerifyScale(); }
		else { std::cout << " No assignation ---> Max < Min " << std::endl; }
	}
}

void pcv::ImageManager::SetMin(double _value)
{
	if (_value <= 0) { std::cout << " Not allowed Negative values " << std::endl; }
	else 
	{
		if (_value <= (GetMax() - 0.01)) { static_cast<double>(MinValueScale) = _value; VerifyScale(); }
		else { std::cout << " No assignation ---> Min > Max " << std::endl; }
	}

}

void pcv::ImageManager::CalculateHistogram()
{
	
	this->Hist.bgr_planes.clear();		// Clear the vector each time we recalculate the histogram
	
	if(this->output.empty()) { cout << " No image Load " << endl; return; }
	
	if (this->output.channels() == 3)
	{
		cv::split(this->output, this->Hist.bgr_planes);
		/// Calculate the histogram
		cv::calcHist(&Hist.bgr_planes[0], 1, 0, Mat(), Hist.Info_Hist_Data.b_hist, 1, &Hist.Info_Hist_Data.histSize, &Hist.Info_Hist_Data.histRange, true, false);
		cv::calcHist(&Hist.bgr_planes[1], 1, 0, Mat(), Hist.Info_Hist_Data.g_hist, 1, &Hist.Info_Hist_Data.histSize, &Hist.Info_Hist_Data.histRange, true, false);
		cv::calcHist(&Hist.bgr_planes[2], 1, 0, Mat(), Hist.Info_Hist_Data.r_hist, 1, &Hist.Info_Hist_Data.histSize, &Hist.Info_Hist_Data.histRange, true, false);
		/// Normalizar el resultado a [ 0, histImage.rows ]
		cv::normalize(Hist.Info_Hist_Data.b_hist,									// Blue channel
					  Hist.Info_Hist_Data.b_hist,
					  0,															// Min
					  this->Hist.Info_Hist_Window_Data.histWindow.rows,				// Max
					  cv::NORM_MINMAX, -1, Mat());
		
		cv::normalize(Hist.Info_Hist_Data.g_hist,									// Green Channel
						Hist.Info_Hist_Data.g_hist,
						0,															// Min
						this->Hist.Info_Hist_Window_Data.histWindow.rows,			// Max
						cv::NORM_MINMAX, -1, Mat());
		
		cv::normalize(Hist.Info_Hist_Data.r_hist,									// Red Channel
						Hist.Info_Hist_Data.r_hist,
						0,															// Min
						this->Hist.Info_Hist_Window_Data.histWindow.rows,			// Max
						cv::NORM_MINMAX, -1, Mat());
	}

	
	else if (this->output.channels() == 1) 
	{
		//this->Hist.bgr_planes.at(0).copyTo(this->output);
		cv::Mat temp = output.clone();		// Review. Try to delete this intermediate variable
		Hist.bgr_planes.push_back(temp);
		/// Calculate the histogram
		cv::calcHist(&Hist.bgr_planes[0], 1, 0, Mat(), Hist.Info_Hist_Data.b_hist, 1, &Hist.Info_Hist_Data.histSize, &Hist.Info_Hist_Data.histRange);
		/// Normalizar el resultado a [ 0, histImage.rows ]
		cv::normalize(Hist.Info_Hist_Data.b_hist,									// Blue channel
						Hist.Info_Hist_Data.b_hist,
						0,															// Min
						this->Hist.Info_Hist_Window_Data.histWindow.rows,			// Max
						cv::NORM_MINMAX, -1, Mat());
	}

	
	else
	{
		cout << " Problem with Number of Channels: " << this->output.channels() << endl; return;
	}

}

void pcv::ImageManager::DrawHistogram()
{
	// Reset the Matrix
	this->Hist.Info_Hist_Window_Data.histWindow = cv::Mat(this->Hist.Info_Hist_Window_Data.high,
															this->Hist.Info_Hist_Window_Data.width,
																CV_8UC3, cv::Scalar(255, 255, 255));

	if (this->output.channels() == 3)
	{
		for (int i = 1; i < this->Hist.Info_Hist_Data.histSize; i++)
		{
			// BLUE
			cv::line(this->Hist.Info_Hist_Window_Data.histWindow,
					cv::Point(this->Hist.Info_Hist_Window_Data.bin_w*(i - 1), this->Hist.Info_Hist_Window_Data.high - cvRound(this->Hist.Info_Hist_Data.b_hist.at<float>(i - 1))),
					cv::Point(this->Hist.Info_Hist_Window_Data.bin_w*(i), this->Hist.Info_Hist_Window_Data.high - cvRound(this->Hist.Info_Hist_Data.b_hist.at<float>(i))),
					cv::Scalar(255, 0, 0), 2, 8, 0);
			// GREEN
			cv::line(this->Hist.Info_Hist_Window_Data.histWindow,
					cv::Point(this->Hist.Info_Hist_Window_Data.bin_w*(i - 1), this->Hist.Info_Hist_Window_Data.high - cvRound(this->Hist.Info_Hist_Data.g_hist.at<float>(i - 1))),
					cv::Point(this->Hist.Info_Hist_Window_Data.bin_w*(i), this->Hist.Info_Hist_Window_Data.high - cvRound(this->Hist.Info_Hist_Data.g_hist.at<float>(i))),
					cv::Scalar(0, 255, 0), 2, 8, 0);
			// RED
			cv::line(this->Hist.Info_Hist_Window_Data.histWindow,
					cv::Point(this->Hist.Info_Hist_Window_Data.bin_w*(i - 1), this->Hist.Info_Hist_Window_Data.high - cvRound(this->Hist.Info_Hist_Data.r_hist.at<float>(i - 1))),
					cv::Point(this->Hist.Info_Hist_Window_Data.bin_w*(i), this->Hist.Info_Hist_Window_Data.high - cvRound(this->Hist.Info_Hist_Data.r_hist.at<float>(i))),
					cv::Scalar(0, 0, 255), 2, 8, 0);
		}
	}
	else if (this->output.channels() == 1)
	{
		for (int i = 1; i < this->Hist.Info_Hist_Data.histSize; i++)
		{
			// GRAYSCALE
			cv::line(this->Hist.Info_Hist_Window_Data.histWindow,
					cv::Point(this->Hist.Info_Hist_Window_Data.bin_w*(i - 1), this->Hist.Info_Hist_Window_Data.high - cvRound(this->Hist.Info_Hist_Data.b_hist.at<float>(i - 1))),
					cv::Point(this->Hist.Info_Hist_Window_Data.bin_w*(i), this->Hist.Info_Hist_Window_Data.high - cvRound(this->Hist.Info_Hist_Data.b_hist.at<float>(i))),
					cv::Scalar(125, 125, 125), 2, 8, 0);
		}
	}
	else
	{
		cout << " Problem with Number of Channels: " << this->output.channels() << endl; return;
	}

}

#pragma endregion


#pragma region METHODS CLASS CONTRAST

pcv::Contrast::Contrast() : ImageManager()
{
	//std::cout << "En Derivated Class" << std::endl;
	SetConstrast(1.0).SetBrightness(0.0).SetGamma(1.0).SetModeOperation(pcv::Operation_Mode::CONTRAST_AND_BRIGHTNESS);
}

pcv::Contrast::Contrast(cv::Mat& _img, double _scale, double _contrast, double _brightness, double _gamma, bool _ApplyChangesFinished )
	: ImageManager(_img, _scale, _ApplyChangesFinished)
{
	SetConstrast(_contrast).SetBrightness(_brightness).SetGamma(_gamma).SetModeOperation(pcv::Operation_Mode::CONTRAST_AND_BRIGHTNESS);
}

pcv::Contrast::~Contrast()
{
	// Not necesarry call the destructor of the ImageManager because destructors are called in inverse mode that constructors are called
	// First execute destructor of Constrast and after the destructor of ImageManager
}

pcv::Contrast& pcv::Contrast::SetConstrast(double _value)
{
	if ( _value < GetMinConstrast() )		{ this->contrast = GetMinConstrast(); }
	else if ( _value > GetMaxConstrast() )	{ this->contrast = GetMaxConstrast(); }
	else { this->contrast = _value; }
	return *this;
}

pcv::Contrast& pcv::Contrast::SetBrightness(double _value)
{
	if ( _value < GetMinBrightness() )		{ this->brightness = GetMinBrightness(); }
	else if ( _value > GetMaxBrightness() )	{ this->brightness = GetMaxBrightness(); }
	else { this->brightness = _value; }
	return *this;
}

pcv::Contrast& pcv::Contrast::SetGamma(double _value)
{
	if ( _value < GetMinGamma() )		{ this->gamma = GetMinGamma(); }
	else if ( _value > GetMaxGamma() )	{ this->gamma = GetMaxGamma(); }
	else { this->gamma = _value; }
	return *this;
}

void pcv::Contrast::SetMaxConstrast(double _value)
{
	if ( _value < 0 ) { std::cout << " Not allowed Negative values " << std::endl; }
	else
	{
		if (_value >= ( GetMinConstrast() + 0.01) ) { static_cast<double>(MaxValueConstrast) = _value;  VerifyInfo(); }
		else { std::cout << " No assignation ---> Max < Min " << std::endl; }
	}
}

void pcv::Contrast::SetMinConstrast(double _value)
{
	if (_value <= 0) { std::cout << " Not allowed Negative values " << std::endl; }
	else
	{
		if (_value <= ( GetMaxConstrast() - 0.01) ) { static_cast<double>(MinValueConstrast) = _value; VerifyInfo(); }
		else { std::cout << " No assignation ---> Min > Max " << std::endl; }
	}
}

void pcv::Contrast::SetMaxBrightness(double _value)
{
	if (_value < 0) { std::cout << " Not allowed Negative values " << std::endl; }
	else
	{
		if (_value >= (GetMinBrightness() + 0.01)) { static_cast<double>(MaxValueBrightness) = _value;  VerifyInfo(); }
		else { std::cout << " No assignation ---> Max < Min " << std::endl; }
	}
}

void pcv::Contrast::SetMinBrightness(double _value)
{
	if (_value <= 0) { std::cout << " Not allowed Negative values " << std::endl; }
	else
	{
		if (_value <= (GetMaxBrightness() - 0.01)) { static_cast<double>(MinValueBrightness) = _value; VerifyInfo(); }
		else { std::cout << " No assignation ---> Min > Max " << std::endl; }
	}
}

void pcv::Contrast::SetMaxGamma(double _value)
{
	if (_value < 0) { std::cout << " Not allowed Negative values " << std::endl; }
	else
	{
		if (_value >= (GetMinGamma() + 0.01)) { static_cast<double>(MaxValueGamma) = _value;  VerifyInfo(); }
		else { std::cout << " No assignation ---> Max < Min " << std::endl; }
	}
}

void pcv::Contrast::SetMinGamma(double _value)
{
	if (_value <= 0) { std::cout << " Not allowed Negative values " << std::endl; }
	else
	{
		if (_value <= (GetMaxGamma() - 0.01)) { static_cast<double>(MinValueGamma) = _value; VerifyInfo(); }
		else { std::cout << " No assignation ---> Min > Max " << std::endl; }
	}
}

void pcv::Contrast::VerifyInfo()
{
	
	// Verify the contrast
	if ( GetContrast() < GetMinConstrast() ) { SetConstrast( GetMinConstrast() ); }
	if ( GetContrast() > GetMaxConstrast() ) { SetConstrast( GetMaxConstrast() ); }
	// Verify the Brightness
	if ( GetBrightness() < GetMinBrightness() ) { SetBrightness( GetMinBrightness() ); }
	if ( GetBrightness() > GetMaxBrightness() ) { SetBrightness( GetMaxBrightness() ); }
	// Verify the gamma
	if ( GetGamma() < GetMinGamma() ) { SetGamma( GetMinGamma() ); }
	if ( GetGamma() > GetMaxGamma() ) { SetGamma( GetMaxGamma() ); }

}

void pcv::Contrast::Redraw()
{
	Update();
	Show();
}

void pcv::Contrast::Show()
{
	pcv::ImageManager::Show();
}

void pcv::Contrast::Update()
{
	//std::cout << "IN CLASS CONSTRAST" << std::endl;
	cv::destroyWindow(GetNameOut()); /*cv::destroyWindow("Histogram");*/
	// First reset the output. Method class ImageManager
	ResetOutput();
	// Second apply the changes of my own class
	ApplyChanges();
	// Finally apply the changes of the base class.Method class ImageManager
	ApplyScale();

	// FINALLY VERIFY IF WE HAVE TO MAKE THE HISTOGRAM
	if (GetShowHistogram() == true)
	{
		pcv::ImageManager::CalculateHistogram();
		pcv::ImageManager::DrawHistogram();
	}
}

void pcv::Contrast::ApplyChanges()
{
	// Execute  the function corresponding
	if (this->mode == Operation_Mode::CONTRAST_AND_BRIGHTNESS) { this->Execute_Constrast_AND_Birghtness(); }
	if (this->mode == Operation_Mode::GAMACORRECTION) { this->Execute_Gamma_Correction(); }
}


void pcv::Contrast::Execute_Constrast_AND_Birghtness()
{
	
	for (size_t _row = 0; _row < this->output.rows; _row++)
	{
		for (size_t _col = 0; _col < this->output.cols; _col++)
		{
			if (this->original.channels() == 3)
			{
				// Change the first channel
				this->output.at<cv::Vec3b>(_row, _col)[0] =
					cv::saturate_cast<uchar>(GetContrast()*(this->original.at<cv::Vec3b>(_row, _col)[0]) + GetBrightness());
				// Change the second channel
				this->output.at<cv::Vec3b>(_row, _col)[1] =
					cv::saturate_cast<uchar>(GetContrast()*(this->original.at<cv::Vec3b>(_row, _col)[1]) + GetBrightness());
				// Change the third channel
				this->output.at<cv::Vec3b>(_row, _col)[2] =
					cv::saturate_cast<uchar>(GetContrast()*(this->original.at<cv::Vec3b>(_row, _col)[2]) + GetBrightness());
			}
			else if (this->original.channels() == 1)
			{
				this->output.at<uchar>(_row, _col) =
					cv::saturate_cast<uchar>(GetContrast()*(this->original.at<uchar>(_row, _col)) + GetBrightness());
			}
			else
				std::cout << "The image had a strange number of channels. Number of channels = " << this->original.channels() << std::endl;
		}
	}


}

void pcv::Contrast::Execute_Gamma_Correction()
{
	cv::Mat lookUpTable(1, 256, CV_8U);
	uchar* p = lookUpTable.ptr();
	for (int i = 0; i < 256; ++i)
		p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, GetGamma()) * 255.0);
	cv::LUT(this->original, lookUpTable, this->output);
}

#pragma endregion



void pcv::Reseize(int _value, void* ptr2class)
{
	// //METODO EMPLEANDO SOLO LA TUPLE

	//std::tuple<pcv::ImageManager, pcv::InfoTrackbar>& that = *(std::tuple<pcv::ImageManager, pcv::InfoTrackbar>*) ptr2class;

	//pcv::ImageManager C_that;
	//pcv::InfoTrackbar* S_that;

	//std::tie(C_that, S_that) = that;   // unpacking tuple into variables

	//// Update the value of the size
	//double scale = ( static_cast<double>(_value) * (S_that->conversationFactor) );
	//C_that.SetScaleImage(scale);
	//std::cout << C_that.GetScaleImage() << "   " << scale << std::endl;
	//C_that.Update();
	//C_that.Show();

	// METODOS EMPLEANDO LA CLASE
	
	pcv::Info* Data = (pcv::Info*)ptr2class;

	double scale = (static_cast<double>(_value) * ( Data->S_Info->conversationFactor ) );
	Data->C_Info->SetScaleImage(scale);

	std::cout << Data->C_Info->GetScaleImage() << "   " << scale << "    " << Data->S_Info->conversationFactor << std::endl;

	Data->C_Info->Update();
	Data->C_Info->Show();


	//// METODOS EMPLEANDO SOLO LA CLASE

	//pcv::ImageManager& C_that = *(pcv::ImageManager*)ptr2class;

	//double scale = (static_cast<double>(_value) * ( C_that.GetConversionFactor() ) );
	//C_that.SetScaleImage(scale);
	//std::cout << C_that.GetScaleImage() << "   " << scale << std::endl;
	//C_that.Update();
	//C_that.Show();


}

void pcv::Change_Contrast(int _value, void* ptr2class)
{
	// NOT COMPLETED
	pcv::Info* Data = (pcv::Info*)ptr2class;


	//double scale = (static_cast<double>(_value) * (Data->S_Info->conversationFactor));
	//Data->C_Info->SetScaleImage(scale);

	Data->C_Info->Update();
	Data->C_Info->Show();
}

void pcv::Change_Brightness(int _value, void* ptr2class)
{
	// NOT COMPLETED
}

void pcv::Change_Gamma(int _value, void* ptr2class)
{
	pcv::Info* Data = (pcv::Info*)ptr2class;
	//pcv::Info& Dato = *(pcv::Info*)ptr2class;
	//pcv::Contrast* ptr = dynamic_cast<Contrast*>(Data->C_Info);
	
	double gamma = (static_cast<double>(_value) * (Data->S_Info->conversationFactor));
	//Data->C_Info->SetScaleImage(gamma);
	dynamic_cast<Contrast*>(Data->C_Info)->SetGamma(gamma);
	//ptr->SetGamma(gamma);

	std::cout << dynamic_cast<Contrast*>(Data->C_Info)->GetGamma() 
			  << "   " << gamma << "    " << Data->S_Info->conversationFactor << std::endl;
	dynamic_cast<Contrast*>(Data->C_Info)->Redraw();

	//std::cout << ptr->GetGamma() << "   " << gamma << "    " << Data->S_Info->conversationFactor << std::endl;
	//ptr->Redraw();

}