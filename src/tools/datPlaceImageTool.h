//=======================================================================================
// datPlaceImageTool.h
// IFT3100-13
//=======================================================================================
#pragma once

#include "datTool.h"

BEGIN_DAT_NAMESPACE

//=======================================================================================
// Tool to place images in the scene
//=======================================================================================
struct datPlaceImageTool : datEditTool {

private:
    std::unique_ptr<datImage> m_transient;
    std::vector<ofImage> m_imagesToPlace;
    std::unique_ptr<datMouseEvent> m_lastMouseEvent;

    ofxPanel m_panel;
    ofParameter<int> m_paramWidth;
    ofParameter<int> m_paramHeight;

private:
    void updateParameters();
    void onWidthChanged(int& value) { updateTransient(); }
    void onHeightChanged(int& value) { updateTransient(); }
    void updateTransient();

protected:
    virtual void onStartTool() override;
    virtual void onExitTool() override;
    virtual void onLeftMouseButtonDown(datMouseEvent const& ev) override;
    virtual void onRightMouseButtonDown(datMouseEvent const& ev) override;
    virtual void onMouseMotion(datMouseEvent const& ev) override;
    virtual void onDraw() override;


public:
    void SetImagesToPlace(std::vector<ofImage> const& images) { m_imagesToPlace = images; }

    datPlaceImageTool();
    virtual ~datPlaceImageTool();
};

END_DAT_NAMESPACE
