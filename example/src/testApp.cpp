#include "testApp.h"

#include "event/Event.h";
#include "event/EventDispatcher.h";

void userNewHandler(xtionni::Event &e)
{
	ofLog() << "new: " + e.getType();
}

void userLostHandler(xtionni::Event &e)
{
	ofLog() << "lost: " + e.getType();
}

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetFrameRate(60);
	ofBackground(0);

	//IR
	//xtion.initialize(ofFilePath::getCurrentWorkingDirectory() + "../Resources/configIR.xml", "", false, false, false, false, false, false, true);

	//Others
	xtion.initialize(ofFilePath::getCurrentWorkingDirectory() + "../Resources/config.xml", "", true, true, true, true, true, true, false);
	
	//Record
	//xtion.startRecord(ofFilePath::getCurrentWorkingDirectory());

	//Gesture
	xtion.getGestureManager()->addGesture(xtionni::GESTURE_CLICK);
	xtion.getGestureManager()->addGesture(xtionni::GESTURE_WAVE);
	xtion.getGestureManager()->addGesture(xtionni::GESTURE_RAISE_HAND);

	//
	xtionni::Event e0(xtionni::USER_NEW);
	xtionni::Event e1(xtionni::USER_LOST);
	ofLog() << e0.getType();
	ofLog() << e1.getType();

	xtionni::EventDispatcher dispatcher;
	dispatcher.addEventListener(xtionni::USER_NEW, userNewHandler);
	dispatcher.addEventListener(xtionni::USER_NEW, userNewHandler);
	dispatcher.addEventListener(xtionni::USER_LOST, userLostHandler);

	dispatcher.dispatchEvent(e0);
}

//--------------------------------------------------------------
void testApp::update()
{
	xtion.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	if (xtion.useDepth())
	{
		xtion.getDepthManager()->getBitmap()->draw(0, 0);
	}

	if (xtion.useImage())
	{
		xtion.getImageManager()->getBitmap()->draw(320, 0);
	}

	if (xtion.useIR())
	{
		xtion.getIRManager()->getBitmap()->draw(640, 0);
	}

	if (xtion.useUser())
	{
		xtion.getUserManager()->drawSkeletons(0, 0);
	}

	if (xtion.useGesture())
	{
	}

	if (xtion.useHands())
	{
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{
}

void testApp::exit()
{
	xtion.finalize();
	ofLog(OF_LOG_WARNING, "APPLICATION EXIT");
}