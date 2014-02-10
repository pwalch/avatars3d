/*
  * 3D Avatars
  * Pierre Walch
  */

#include <iostream>
#include <memory>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <irrlicht.h>
#include <revel.h>
#include <QTime>
#include <locale.h>

#include "mainwindow.h"
#include "avatarsfactory.h"
#include "../libs/tinyxml2.h"
#include "camerawindow.h"
#include "affinetransformation.h"
#include "court.h"
#include "engine.h"

using namespace tinyxml2;

using namespace irr;
using namespace irr::core;
using namespace irr::video;

Engine& Engine::getInstance()
{
    static Engine instance;
    return instance;
}

Engine::Engine()
{
    // Set standard locale to avoid XML float parsing problems
    setlocale(LC_NUMERIC, "C");
    mIsRecording = false;
    mIsPlaying = false;
    mIsLivePlaying = false;
    mCurrentFrame = 0;
}

Engine::~Engine()
{

//    mBallStream->close();
//    mPlayerStream->close();
//    mCameraStream->close();
}

int Engine::start(const QApplication& app, const std::vector<std::string>& args)
{
    if(args.size() != 2) {
        throwError(L"bad arguments, only a unique XML file is accepted as argument");
    }

    std::string cfgPath = args.at(1);
    loadSettings(cfgPath);

    if(mSequenceSettings.mMode == MODE_GUI || mSequenceSettings.mMode == MODE_CONSOLE) {
        updateTrajectories(mSequenceSettings.mFrameNumber);
        setTime(mSequenceSettings.mInitialTime);
    }

    switch(mSequenceSettings.mMode) {
        case MODE_GUI: {
            MainWindow mainWindow;
            mainWindow.show();
            return app.exec();
        }
        case MODE_CONSOLE: {
            saveVideo(mSequenceSettings.mStartTime, mSequenceSettings.mEndTime);
        }
        break;

        case MODE_LIVE: {
            MainWindow mw;
            mw.show();
            mw.setFollowTrajectory(false);
            mw.blockAnimationTab();
            livePlay();
        }
        break;
    }

    return 0;
}

void Engine::loadSettings(const std::string& cfgPath)
{
    setlocale(LC_NUMERIC, "C");

    mFactory = std::unique_ptr<AvatarsFactory>(new AvatarsFactory(cfgPath));

    mCameraStream = mFactory->createCameraStream();
    mPlayerStream = mFactory->createPlayerStream();
    mBallStream = mFactory->createBallStream();

    // Initialize all the components of the program
    mSequenceSettings = mFactory->retrieveSequenceSettings();
    mTransformation = mFactory->createTransformation();
    mCameraWindow = mFactory->createCamera();
    mCourt = mFactory->createCourt();
}

void Engine::updateTrajectories(int nbFramesToCatch)
{
    // Catch new chunks from streams
    const std::pair<VectorSequence, VectorSequence>& cameraChunk = mFactory->createCameraChunk(*mCameraStream, nbFramesToCatch);
    const std::map<int, VectorSequence >& playerChunk = mFactory->createPlayerChunkMap(*mPlayerStream,
                                                                                 mCourt->getPlayers(),
                                                                                 nbFramesToCatch);
    const VectorSequence& ballChunk = mFactory->createBallChunk(*mBallStream, nbFramesToCatch);

    // Update camera trajectory
    mCameraWindow->updatePositions(cameraChunk.first);
    mCameraWindow->updateRotations(cameraChunk.second);

    // Update player and ball trajectories
    mCourt->updateTrajectories(playerChunk, ballChunk);
}

void Engine::throwError(const stringw& errorMessage)
{
    std::wcerr << "Error: " << errorMessage.c_str() << std::endl;
    exit(1);
}

void Engine::setTime(int time)
{
    mCurrentFrame = time;

    mCourt->setTime(time);

    mCameraWindow->setTime(time);
    mCameraWindow->updateScene();
}

const SequenceSettings &Engine::getSequenceSettings() const
{
    return mSequenceSettings;
}

void Engine::stopRecording()
{
    mIsRecording = false;
}

void Engine::stopPlaying()
{
    mIsPlaying = false;
}

void Engine::stopLivePlaying()
{
    mIsLivePlaying = false;
}

int Engine::getCurrentFrame()
{
    return mCurrentFrame;
}

void Engine::play(int from, int to)
{
    // Calculate frametime in milliseconds from framerate
    int frametime = (1.0 / (mSequenceSettings.mFramerate)) * 1000;

    QTime timer;
    mIsPlaying = true;
    for(int i = from; i <= to; ++i) {
        timer.restart();
        setTime(i);

        // Wait frame time
        int remaining = frametime - timer.elapsed();
        if(remaining > 0)
            mCameraWindow->getDevice()->sleep(remaining);

        // Stops to play to process events and check for interruption
        QApplication::processEvents();
        if(!mIsPlaying) {
            break;
        }
    }
    mIsPlaying = false;
}

const AffineTransformation& Engine::getTransformation() const
{
    return *mTransformation;
}



void Engine::saveVideo(int from, int to)
{
    int beforeTime = mCurrentFrame;

    // Define window size and frames to encode
    dimension2d<u32> windowSize = mCameraWindow->getSettings().mWindowSize;
    int width = windowSize.Width;
    int height = windowSize.Height;
    int encodingFrameNumber = to - from;

    //------------------------------------------------------------------------------------------------------
    // The following is a code snippet from Revel examples, with some modifications to interrupt recording
    //------------------------------------------------------------------------------------------------------

    // Make sure the API version of Revel we're compiling against matches the
    // header files!  This is terribly important!
    if (REVEL_API_VERSION != Revel_GetApiVersion()) {
        printf("ERROR: Revel version mismatch!\n");
        printf("Headers: version %06x, API version %d\n", REVEL_VERSION, REVEL_API_VERSION);
        printf("Library: version %06x, API version %d\n", Revel_GetVersion(), Revel_GetApiVersion());
        exit(1);
    }

    // Create an encoder
    int encoderHandle;
    Revel_Error revError = Revel_CreateEncoder(&encoderHandle);
    if (revError != REVEL_ERR_NONE) {
        printf("Revel Error while creating encoder: %d\n", revError);
        exit(1);
    }

    // Set up the encoding parameters.  ALWAYS call Revel_InitializeParams()
    // before filling in your application's parameters, to ensure that all
    // fields especially ones that you may not know about) are initialized
    // to safe values.
    Revel_Params revParams;
    Revel_InitializeParams(&revParams);
    revParams.width = width;
    revParams.height = height;
    revParams.frameRate = mSequenceSettings.mFramerate;
    revParams.quality = 1.0f;
    revParams.codec = REVEL_CD_XVID;

    revParams.hasAudio = false;
    revParams.audioChannels = 2;
    revParams.audioRate = 22050;
    revParams.audioBits = 16;
    revParams.audioSampleFormat = REVEL_ASF_PCM;

    // Initialize encoding
    revError = Revel_EncodeStart(encoderHandle,
                                 mSequenceSettings.mVideoOutputName.c_str(),
                                 &revParams);
    if (revError != REVEL_ERR_NONE) {
        printf("Revel Error while starting encoding: %d\n", revError);
        exit(1);
    }

    // Choose video settings
    Revel_VideoFrame frame;
    frame.width = width;
    frame.height = height;
    frame.bytesPerPixel = 4;
    frame.pixelFormat = REVEL_PF_BGRA;
    // Irrlicht U32 -> Revel BGRA, U32 ABGR -> Revel RGBA
    frame.pixels = new int[width*height];
    memset(frame.pixels, 0, width*height*4);
    int* pixels = (int*) frame.pixels;

    // Discard preceding events
    mCameraWindow->getDevice()->run();
    mIsRecording = true;
    // Fill pixel array for each frame
    for(int i = from; i <= to; ++i)
    {
        setTime(i);
        IImage* image = mCameraWindow->createScreenshot();
        int pixelCounter = 0;
        for(unsigned int y = 0; y < frame.height; ++y) {
            for(unsigned int x = 0; x < frame.width; ++x) {

                pixels[pixelCounter] = image->getPixel(x,y).color;

                // Change color format
//                SColor pixel = image->getPixel(x, y);
//                u8 blue = pixel.getBlue(),
//                   green = pixel.getGreen(),
//                   red = pixel.getRed(),
//                   alpha = pixel.getAlpha();
//                u32 color =
//                        (alpha << 24)
//                        | (blue << 16)
//                        | (green << 8)
//                        | red;
//                pixels[pixelCounter] = color;
                // Short version
//                pixels[pixelCounter] =
//                    (pixel.getAlpha() << 24)
//                    | (pixel.getBlue() << 16)
//                    | (pixel.getGreen() << 8)
//                    | pixel.getRed();

                ++pixelCounter;
            }
        }

        // Drop Irrlicht resource after its process
        image->drop();

        int frameSize;
        revError = Revel_EncodeFrame(encoderHandle, &frame, &frameSize);
        if (revError != REVEL_ERR_NONE) {
            printf("Revel Error while writing frame: %d\n", revError);
            exit(1);
        }
        // printf("Frame %d of %d: %d bytes\n", i+1, encodingFrameNumbers,
        // frameSize);

        // Process Irrlicht events and check for interruption
        mCameraWindow->getDevice()->run();
        if(!mIsRecording) {
            break;
        }
    }
    mIsRecording = false;

    // Choose audio settings
    int totalAudioBytes = 0;
    int audioBufferSize = encodingFrameNumber;
    char* audioBuffer = new char[encodingFrameNumber];
    for(int i = 0; i < encodingFrameNumber; ++i)
        audioBuffer[i] = 0;
    revError = Revel_EncodeAudio(
                            encoderHandle,
                            audioBuffer,
                            audioBufferSize,
                            &totalAudioBytes);
    if (revError != REVEL_ERR_NONE) {
        printf("Revel Error while writing audio: %d\n", revError);
        exit(1);
    }
    // printf("Encoded %d bytes of audio\n", totalAudioBytes);

    // Finalize encoding. If this step is skipped, the output movie will
    // be unviewable!
    int totalSize;
    revError = Revel_EncodeEnd(encoderHandle, &totalSize);
    if (revError != REVEL_ERR_NONE){
        printf("Revel Error while ending encoding: %d\n", revError);
        exit(1);
    }

    // printf("%s written: %dx%d, %d frames, %d bytes\n", videoName.c_str(),
    // width, height, encodingFrameNumber, totalSize);
    // Final cleanup
    Revel_DestroyEncoder(encoderHandle);
    if (audioBuffer != NULL)
        delete [] audioBuffer;
    delete [] (int*)frame.pixels;

    //    const float MBSize = 1048576.0;
    //    float size = totalSize / MBSize;
    //    std::cout.setf(std::ios::fixed);
    //    std::cout << videoName << " (" << std::setprecision(2) << size <<
    //    " MB) saved in " << width << "*" << height << " with " <<
    //    encodingFrameNumber << " frames" << std::endl;


    // Restore current frame because video encoding changed it
    setTime(beforeTime);
}

void Engine::livePlay()
{
    const int windowSize = 20;

    int chunkStart = 0;
    mIsLivePlaying = true;
    while(mIsLivePlaying) {
        QTime timer;
        timer.start();
        updateTrajectories(windowSize);
        std::cout << "Time elapsed : " << timer.elapsed() << std::endl;
        play(chunkStart, chunkStart + windowSize - 1);

        chunkStart = chunkStart + windowSize;
    }

    mIsLivePlaying = false;
}

const Court& Engine::getCourt() const
{
    return *mCourt;
}

CameraWindow& Engine::getCameraWindow() const
{
    return *mCameraWindow;
}

