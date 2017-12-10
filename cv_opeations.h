#pragma once

#include "Header.h"

namespace pcv
{
	#define VIDEO 1;
	#define IMAGE 2;

	enum Operation_Mode
	{
		CONTRAST_AND_BRIGHTNESS = 0, GAMACORRECTION
	};
	
	/* ----------------------------------------------------------<      PROTOTYPE OF FUNCTIONS          >------------------------------------------------*/

	// We put here the header for the functions
	void Reseize(int, void*);
	void Change_Contrast(int, void*);
	void Change_Brightness(int, void*);
	void Change_Gamma(int, void*);





	/* ---------------------------------------------------------->      PROTOTYPE OF FUNCTIONS           <-----------------------------------------------*/




	/* ---------------------------------------------------------- <            STRUCTS                  > -----------------------------------------------*/
	
	struct InfoTrackbar
	{
		std::string NameTrackbar;
		int Max;
		int Min;
		int currentValue;
		double conversationFactor;
		void(*ptr_funct) (int, void*);		// Here we save the function that we will pass
	};

	struct InfoWindowHist
	{
		int width;
		int high;
		int bin_w;
		cv::Mat histWindow;
	};

	struct InfoHistogram
	{
		int histSize;
		float range[2];
		const float* histRange;
		cv::Mat b_hist;
		cv::Mat g_hist;
		cv::Mat r_hist;
	};
	
	struct Histogram
	{
		std::vector<cv::Mat> bgr_planes;
		InfoHistogram Info_Hist_Data;
		InfoWindowHist Info_Hist_Window_Data;
	};

	/* ---------------------------------------------------------- >            STRUCTS                  < -----------------------------------------------*/


	/* ----------------------------------------------------------        DESING OF CLASS'S              ------------------------------------------------*/

	class ImageManager
	{

		public:
			// CONSTRUCTORS AND DESTRUCTOR
			ImageManager();
			ImageManager(cv::Mat&, double = 1.0, bool = false);
			~ImageManager();

			// SETTERS
			ImageManager& SetImage(cv::Mat& _imag) { original = _imag; ptr2orig = &_imag ; return *this; }
			 inline ImageManager& SetNameOut(std::string _Name) { nameOut = _Name; return *this; }
			 inline ImageManager& SetScaleImage(double _value);
			 inline ImageManager& SetApplyChangesFinished(bool _value) { ApplyChangesFinished = _value; return *this; }

			 void SetMax(double _value);/*{ static_cast<double>(MaxValueScale) = _value; }*/
			 void SetMin(double _value);/*{ static_cast<double>(MinValueScale) = _value; }*/
			 void ShowHistogram(bool _value) { show_histogram = _value; }

			 void SetModeFile(int _value)
			 {
				 if (_value == 1) { modoArchivo = VIDEO; }
				 if (_value == 2) { modoArchivo = IMAGE; }
			 }


			// GETTERS
			 inline double GetScaleImage() const { return scaleImage; }
			 inline std::string GetNameOut() const { return nameOut; }

			 inline double GetMax() const { return MaxValueScale; }
			 inline double GetMin() const { return MinValueScale; }
			 inline bool GetShowHistogram() const { return show_histogram; }
			
			 inline bool GetApplyChangesFinished() const { return ApplyChangesFinished; }

			// METHODS
			 virtual void Show();				// WILL MAKE VIRTUAL IN THE FUTURE
			 virtual void Update();				// WILL MAKE VIRTUAL IN THE FUTURE
			 void Draw();				// WILL MAKE VIRTUAL IN THE FUTURE

		private:
			double scaleImage;
			std::string nameOut;
			bool ApplyChangesFinished;

			const double MaxValueScale = 1.5;
			const double MinValueScale = 0.25;

			struct Histogram Hist;
			bool show_histogram;

			int modoArchivo;

		protected:
			// It is necesary make it protected so the sons be able to use them with the necesity of add more methods
			cv::Mat original;
			cv::Mat* ptr2orig;			// Ptr that save the memory direction of the original image
			cv::Mat input;				// Check because I think that it is not necesary
			cv::Mat output;
			
			void ResetOutput() { output = original.clone(); }
			void ApplyScale() { cv::resize(this->output, this->output, cv::Size(), GetScaleImage(), GetScaleImage(), cv::InterpolationFlags::INTER_LINEAR); }
			void VerifyScale();
			void SetHistogramInfo();
			void CalculateHistogram();
			void DrawHistogram();
	
	};


	class Contrast : public ImageManager
	{
	public:
		// CONSTRUCTORS AND DESTRUCTOR
		Contrast();
		Contrast(cv::Mat&, double = 1.0, double = 1.0, double = 0.0, double = 1.0, bool = false );
		~Contrast();

		// SETTERS
		 inline Contrast& SetConstrast(double _value);
		 inline Contrast& SetBrightness(double _value);
		 inline Contrast& SetGamma(double _value);
		 inline Contrast& SetModeOperation(Operation_Mode _mode) { mode = _mode; return *this; }

		 void SetMaxConstrast(double _value);
		 void SetMinConstrast(double _value);

		 void SetMaxBrightness(double _value);
		 void SetMinBrightness(double _value);

		 void SetMaxGamma(double _value);
		 void SetMinGamma(double _value);

		// GETTERS
		 inline double GetContrast() const { return contrast; }
		 inline double GetBrightness() const { return brightness; }
		 inline double GetGamma() const { return gamma; }
		 inline Operation_Mode GetOperationMode() { return mode; }

		 inline double GetMaxConstrast() const { return MaxValueConstrast; }
		 inline double GetMinConstrast() const { return MinValueConstrast; }
		
		 inline double GetMaxBrightness() const { return MaxValueBrightness; }
		 inline double GetMinBrightness() const { return MinValueBrightness; }
		
		 inline double GetMaxGamma() const { return MaxValueGamma; }
		 inline double GetMinGamma() const { return MinValueGamma; }

		// METHODS
		 virtual void Show();				// WILL MAKE VIRTUAL IN THE FUTURE
		 virtual void Update();				// WILL MAKE VIRTUAL IN THE FUTURE
		 void Redraw();						// WILL MAKE VIRTUAL IN THE FUTURE

	private:
		
		double contrast;
		double brightness;
		double gamma;

		const double MaxValueConstrast = 1.5;
		const double MinValueConstrast = 0.05;

		const double MaxValueBrightness = 1.5;
		const double MinValueBrightness = 0.25;

		const double MaxValueGamma = 1.5;
		const double MinValueGamma = 0.25;

		Operation_Mode mode;
	
	protected:
		void VerifyInfo();
		void ApplyChanges();

		void Execute_Gamma_Correction();
		void Execute_Constrast_AND_Birghtness();

	};



	/* ----------------------------------------------------------        DESING OF CLASS'S              ------------------------------------------------*/

	struct Info
	{
		InfoTrackbar* S_Info;
		ImageManager* C_Info;
	};

}