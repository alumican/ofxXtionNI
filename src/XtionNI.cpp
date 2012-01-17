/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE.txt for full license information.
*/

#include "XtionNI.h"

//--------------------------------------------------------------
xtionni::XtionNI::XtionNI()
{
}

//--------------------------------------------------------------
xtionni::XtionNI::~XtionNI(void)
{
}

//--------------------------------------------------------------
void xtionni::XtionNI::initialize(string configFilePath, string recordFilePath, bool useDepth, bool useImage, bool useAudio, bool useUser, bool useGesture, bool useHands, bool useIR)
{
	try
	{
		useDepth_   = useDepth;
		useImage_   = useImage;
		useAudio_   = useAudio;
		useUser_    = useUser;
		useGesture_ = useGesture;
		useHands_   = useHands;
		useIR_      = useIR;

		//Recorder
		isRecording_ = false;
		player_ = NULL;
		recorder_ = NULL;

		//Context
		context_ = new xn::Context();
		if (recordFilePath != "")
		{
			XnStatus status = context_->Init();
			xtionni::Util::checkError(status, "Init context");

			player_ = new xn::Player();
			status = context_->OpenFileRecording(recordFilePath.c_str(), *player_);
			xtionni::Util::checkError(status, "Can't open recording");
		}
		else
		{
			xn::EnumerationErrors errors;
			XnStatus status = context_->InitFromXmlFile(configFilePath.c_str(), &errors);
			xtionni::Util::checkError(status, "Setup context", &errors);
		}

		//Function
		if (useUser_) useDepth_ = true;

		if (useDepth_  ) { depthManager_   = new xtionni::DepthManager();   depthManager_->initialize(this);   }
		if (useImage_  ) { imageManager_   = new xtionni::ImageManager();   imageManager_->initialize(this);   }
		if (useAudio_  ) { audioManager_   = new xtionni::AudioManager();   audioManager_->initialize(this);   }
		if (useUser_   ) { userManager_    = new xtionni::UserManager();    userManager_->initialize(this);    }
		if (useGesture_) { gestureManager_ = new xtionni::GestureManager(); gestureManager_->initialize(this); }
		if (useHands_  ) { handsManager_   = new xtionni::HandsManager();   handsManager_->initialize(this);   }
		if (useIR_     ) { irManager_      = new xtionni::IRManager();      irManager_->initialize(this);      }
	}
	catch (std::exception& ex)
	{
		ofLog(OF_LOG_WARNING) << ex.what();
		ofLog(OF_LOG_WARNING) << "EXIT by initializing error";
		finalize();
	}
}

//--------------------------------------------------------------
void xtionni::XtionNI::update()
{
	if (useDepth_  ) depthManager_->update();
	if (useImage_  ) imageManager_->update();
	if (useAudio_  ) audioManager_->update();
	if (useUser_   ) userManager_->update();
	if (useGesture_) gestureManager_->update();
	if (useHands_  ) handsManager_->update();
	if (useIR_     ) irManager_->update();
}

//--------------------------------------------------------------
void xtionni::XtionNI::finalize()
{
	if (useDepth_  ) delete depthManager_;
	if (useImage_  ) delete imageManager_;
	if (useAudio_  ) delete audioManager_;
	if (useUser_   ) delete userManager_;
	if (useGesture_) delete gestureManager_;
	if (useHands_  ) delete handsManager_;
	if (useIR_     ) delete irManager_;

	stopRecord();

	if (player_ != NULL)
	{
		player_->Release();
		delete player_;
	}

	if (context_ != NULL)
	{
		context_->Shutdown();
		context_->Release();
		delete context_;
	}
}

//--------------------------------------------------------------
void xtionni::XtionNI::checkRecordError_(XnStatus status, string name)
{
	if (status != XN_STATUS_OK)
	{
		xtionni::XtionNI::stopRecord();
		throw std::runtime_error("XtionNI: (" + name + ") Failed to " + (string)xnGetStatusString(status));
	}
}

void xtionni::XtionNI::startRecord(string outputDirectory)
{
	stopRecord();

	char recordFile[256] = {0};
	time_t rawtime;
	struct tm *timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	XnUInt32 size;
	xnOSStrFormat(recordFile, sizeof(recordFile)-1, &size,
		(outputDirectory + "%d_%02d_%02d[%02d_%02d_%02d].oni").c_str(),
		timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

	XnStatus status;
	recorder_ = new xn::Recorder();

	status = context_->CreateAnyProductionTree(XN_NODE_TYPE_RECORDER, NULL, *recorder_);
	xtionni::XtionNI::checkRecordError_(status, "Create recorder");

	status = recorder_->SetDestination(XN_RECORD_MEDIUM_FILE, recordFile);
	xtionni::XtionNI::checkRecordError_(status, "set destination");

	if (useDepth_)
	{
		status = recorder_->AddNodeToRecording(*depthManager_->getGenerator(), XN_CODEC_16Z_EMB_TABLES);
		xtionni::XtionNI::checkRecordError_(status, "add depth node");
	}

	if (useImage_)
	{
		status = recorder_->AddNodeToRecording(*imageManager_->getGenerator(), XN_CODEC_JPEG);
		xtionni::XtionNI::checkRecordError_(status, "add image node");
	}

	isRecording_ = true;
}

void xtionni::XtionNI::stopRecord()
{
	if (isRecording_)
	{
		if (useDepth_) recorder_->RemoveNodeFromRecording(*depthManager_->getGenerator());
		if (useImage_) recorder_->RemoveNodeFromRecording(*imageManager_->getGenerator());

		recorder_->Release();
		delete recorder_;
	}

	isRecording_ = false;
}

//--------------------------------------------------------------
//GETTER
xn::Context *xtionni::XtionNI::getContext() { return context_; }

xtionni::DepthManager   *xtionni::XtionNI::getDepthManager()   { return depthManager_;   }
xtionni::ImageManager   *xtionni::XtionNI::getImageManager()   { return imageManager_;   }
xtionni::AudioManager   *xtionni::XtionNI::getAudioManager()   { return audioManager_;   }
xtionni::UserManager    *xtionni::XtionNI::getUserManager()    { return userManager_;    }
xtionni::GestureManager *xtionni::XtionNI::getGestureManager() { return gestureManager_; }
xtionni::HandsManager   *xtionni::XtionNI::getHandsManager()   { return handsManager_;   }
xtionni::IRManager      *xtionni::XtionNI::getIRManager()      { return irManager_;      }

bool xtionni::XtionNI::useDepth()   { return useDepth_;   }
bool xtionni::XtionNI::useImage()   { return useImage_;   }
bool xtionni::XtionNI::useAudio()   { return useAudio_;   }
bool xtionni::XtionNI::useUser()    { return useUser_;    }
bool xtionni::XtionNI::useGesture() { return useGesture_; }
bool xtionni::XtionNI::useHands()   { return useHands_;   }
bool xtionni::XtionNI::useIR()      { return useIR_;      }