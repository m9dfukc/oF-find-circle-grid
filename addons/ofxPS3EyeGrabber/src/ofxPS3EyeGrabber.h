//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofJson.h"
#include "ofBaseTypes.h"
#include "ofTypes.h"
#include "ofEvents.h"
#include "ofThread.h"
#include "ofVideoGrabber.h"
#include "ps3eye.h"


/// \brief Create an ofxPS3EyeGrabber.
class ofxPS3EyeGrabber:
    public ofBaseVideoGrabber,
    public ofThread
{
public:
    /// \brief Create an uninitialized ofxPS3EyeGrabber.
    /// \param deviceId The device id.
    ofxPS3EyeGrabber(int deviceId = AUTO_CAMERA_ID);

    /// \brief Destroy the PS3EyeGrabber.
    virtual ~ofxPS3EyeGrabber();

    /// \brief Callback for an exit event.
    /// \param args the event callback parameter.
    void exit(ofEventArgs& args);
    void start();
    void stop();

    std::vector<ofVideoDevice> listDevices() const override;
    bool setup(int w, int h) override;
    void update() override;
    bool isFrameNew() const override;
    bool isInitialized() const override;
    ofPixels& getPixels() override;
    const ofPixels& getPixels() const override;
    void close() override;
    float getHeight() const override;
    float getWidth() const override;

    /// \brief Set the requested ofPixelFormat.
    ///
    /// The grabber can output the following pixel ofPixelFormats:
    ///
    ///     OF_PIXELS_RGB (default)
    ///     OF_PIXELS_RGBA
    ///     OF_PIXELS_YUY2
    ///     OF_PIXELS_NATIVE
    ///
    /// OF_PIXELS_NATIVE defaults to OF_PIXELS_YUY2 and requires no colorspace
    /// conversion of pixels copying.
    ///
    /// \param pixelFormat the requested ofPixelFormat.
    /// \returns true if the format was successfully changed.
    bool setPixelFormat(ofPixelFormat pixelFormat) override;
    ofPixelFormat getPixelFormat() const override;
    void setVerbose(bool verbose) override;
    void setDeviceID(int deviceId) override;
    void setDesiredFrameRate(int framerate) override;
    void videoSettings() override;

    void threadedFunction() override;

    int getDeviceId() const;

    /// \returns true iff auto gain is enabled.
    bool getAutogain() const;

    /// \brief Set the auto gain.
    /// \param val the auto gain value.
    void setAutogain(bool val);

    /// \returns true iff auto white balance is enabled.
    bool getAutoWhiteBalance() const;

    /// \brief Set the auto white balance.
    /// \param val the auto white balance value.
    void setAutoWhiteBalance(bool val);

    /// \returns the current gain setting.
    uint8_t getGain() const;

    /// \brief Set the camera's gain.
    /// \param val a gain between 0-63.
    void setGain(uint8_t val);

    /// \returns the current exposure setting.
    uint8_t getExposure() const;

    /// \brief Set the camera's exposure.
    /// \param val an exposure between 0-255.
    void setExposure(uint8_t val);

    /// \returns the current sharpness setting.
    /// \param val a sharpness between 0-63.
    uint8_t getSharpness() const;

    /// \brief Set the camera's sharpness.
    void setSharpness(uint8_t val);

    /// \returns the current contrast setting.
    uint8_t getContrast() const;

    /// \brief Set the camera's contrast.
    /// \param val a contrast between 0-255.
    void setContrast(uint8_t val);

    /// \returns the current brightness setting.
    uint8_t getBrightness() const;

    /// \brief Set the camera's brightness.
    /// \param val a brightness between 0-255.
    void setBrightness(uint8_t val);

    /// \returns the current hue setting.
    uint8_t getHue() const;

    /// \brief Set the camera's hue.
    /// \param val a hue between 0-255.
    void setHue(uint8_t val);

    /// \returns the current red balance setting.
    uint8_t getRedBalance() const;

    /// \brief Set the camera's red balance.
    /// \param val a red balance between 0-255.
    void setRedBalance(uint8_t val);

    /// \returns the current blue balance setting.
    uint8_t getBlueBalance() const;

    /// \brief Set the camera's blue balance.
    /// \param val a blue balance between 0-255.
    void setBlueBalance(uint8_t val);

    /// \returns the current green balance setting.
    uint8_t getGreenBalance() const;

    /// \brief Set the camera's green balance.
    /// \param val a blue balance between 0-255.
    void setGreenBalance(uint8_t val);

    /// \brief Flip the camera's image.
    /// \param enable true for a vertical flip.
    void setFlipVertical(bool enable);
    OF_DEPRECATED_MSG("Use setFlipVertical() instead", void setVerticalFlip(bool enable));

    /// \brief Flip the camera's image.
    /// \param enable true for a horizontal flip.
    void setFlipHorizontal(bool enable);
    OF_DEPRECATED_MSG("Use setFlipHorizontal() instead", void setHorizontalFlip(bool enable));

    /// \brief Enable a test pattern overlay.
    /// \param enable true for a test pattern.
    void setTestPattern(bool enable);

    /// \brief Enable the LED.
    /// \param enable True if the LED should be enabled.
    void setLED(bool enable);

    /// \returns the camera's current requested FPS value.
    float getFPS() const;

    /// \returns the camera's current actual FPS value.
    float getActualFPS() const;

    enum
    {
        /// \brief An automatic camera id will connect to the first available camera.
        AUTO_CAMERA_ID = -1,

        /// \brief The default camera width.
        DEFAULT_WIDTH = 640,

        /// \brief The default camera height.
        DEFAULT_HEIGHT = 480
    };

    /// \brief Setup a ofxPS3EyeGrabber based ofVideoGrabber from JSON.
    ///
    /// \returns the configured and set up camera.
    static std::shared_ptr<ofVideoGrabber> fromJSON(const ofJson& json);

protected:
    /// \brief Constant used for YUV conversion.
    static const int ITUR_BT_601_CY;

    /// \brief Constant used for YUV conversion.
    static const int ITUR_BT_601_CUB;

    /// \brief Constant used for YUV conversion.
    static const int ITUR_BT_601_CUG;

    /// \brief Constant used for YUV conversion.
    static const int ITUR_BT_601_CVG;

    /// \brief Constant used for YUV conversion.
    static const int ITUR_BT_601_CVR;

    /// \brief Constant used for YUV conversion.
    static const int ITUR_BT_601_SHIFT;

    /// \brief Convert YUV422 to RGBA8888
    /// \param yuv_source a pointer to the YUV data source.
    /// \param strid the YUV data stride.
    /// \param destination the destination RGBA8888 pixels.
    /// \param width the pixel width.
    /// \param height the pixel height.
    static void yuv422_to_rgba8888(const uint8_t* yuv_source,
                                   const int stride,
                                   uint8_t* destination,
                                   const int width,
                                   const int height);

    /// \brief Convert YUV422 to RGB888
    /// \param yuv_source a pointer to the YUV data source.
    /// \param strid the YUV data stride.
    /// \param destination the destination RGB888 pixels.
    /// \param width the pixel width.
    /// \param height the pixel height.
    static void yuv422_to_rgb888(const uint8_t* yuv_source,
                                 const int stride,
                                 uint8_t* destination,
                                 const int width,
                                 const int height);


private:
    /// \brief A shared pointer to the underlying camera device.
    std::shared_ptr<ps3eye::PS3EYECam> _cam;

    /// \brief An internal copy (or pointer) to the camera pixels.
    ofPixels _pixels;

    /// \brief The device id.
    std::size_t _requestedDeviceId = AUTO_CAMERA_ID;

    /// \brief The requested framerate.
    int _requestedFrameRate = 60;

    /// \brief True if the frame is new.
    bool _isFrameNew = true;

    /// \brief The desired pixel format.
    ofPixelFormat _pixelFormat = OF_PIXELS_RGB;

    enum
    {
        /// \brief The default FPS sample interval in milliseconds.
        FPS_SAMPLE_INTERVAL = 500
    };

};
