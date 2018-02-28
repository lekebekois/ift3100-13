//=======================================================================================
// datSelectTool.cpp
// IFT3100-13
//=======================================================================================
#include "datSelectTool.h"

USING_DAT_NAMESPACE

void setCursorType1() { datRenderer::GetActiveRenderer().SetActiveCursorType(datRenderer::CursorType::Normal); }
void setCursorType2() { datRenderer::GetActiveRenderer().SetActiveCursorType(datRenderer::CursorType::Circle); }
void setCursorType3() { datRenderer::GetActiveRenderer().SetActiveCursorType(datRenderer::CursorType::X); }
void setCursorType4() { datRenderer::GetActiveRenderer().SetActiveCursorType(datRenderer::CursorType::Cross); }
void setCursorType5() { datRenderer::GetActiveRenderer().SetActiveCursorType(datRenderer::CursorType::Triangle); }

ofEvent<ofxButton> selectCursorTypeEvent;


datSelectTool::datSelectTool() :
    m_isMouseDrag(false),
    m_hasFirstPoint(false),
    m_color(ofColor::dodgerBlue),
    m_radio1(setCursorType1, selectCursorTypeEvent),
    m_radio2(setCursorType2, selectCursorTypeEvent),
    m_radio3(setCursorType3, selectCursorTypeEvent),
    m_radio4(setCursorType4, selectCursorTypeEvent),
    m_radio5(setCursorType5, selectCursorTypeEvent) {
}


datSelectTool::~datSelectTool() {

}


void datSelectTool::selectObjectsAndClearState() {

    //&&AG needswork select geometries using m_rectangle and m_mode
    m_isMouseDrag = false;
    m_hasFirstPoint = false;
}


void datSelectTool::updateRectangle(datMouseEvent const& ev) {

    assert(m_hasFirstPoint);
    m_rectangle.x = std::min(m_downPoint.x, ev.x);
    m_rectangle.y = std::min(m_downPoint.y, ev.y);
    m_rectangle.width = std::abs(m_downPoint.x - ev.x);
    m_rectangle.height = std::abs(m_downPoint.y - ev.y);
}


void datSelectTool::updateSelectionMode(datMouseEvent const& ev) {

    assert(m_hasFirstPoint);
    m_mode = (m_downPoint.x > ev.x && m_downPoint.y > ev.y) ? SelectionMode::Crossing : SelectionMode::Window;
}


void datSelectTool::onStartTool() {

    m_panel.setup("Tool settings", "", 0.4 * ofGetWidth());
    m_panel.add(m_radio1.setup(datLocalization::SelectTool_CursorNormal()));
    m_panel.add(m_radio2.setup(datLocalization::SelectTool_CursorCircle()));
    m_panel.add(m_radio3.setup(datLocalization::SelectTool_CursorX()));
    m_panel.add(m_radio4.setup(datLocalization::SelectTool_CursorCross()));
    m_panel.add(m_radio5.setup(datLocalization::SelectTool_CursorTriangle()));

    const datRenderer::CursorType currentCursorType = datRenderer::GetActiveRenderer().GetActiveCursorType();
    switch (currentCursorType) {
        case datRenderer::CursorType::Normal:
            ofNotifyEvent(selectCursorTypeEvent, m_radio1);
            break;
        case datRenderer::CursorType::Circle:
            ofNotifyEvent(selectCursorTypeEvent, m_radio2);
            break;
        case datRenderer::CursorType::X:
            ofNotifyEvent(selectCursorTypeEvent, m_radio3);
            break;
        case datRenderer::CursorType::Cross:
            ofNotifyEvent(selectCursorTypeEvent, m_radio4);
            break;
        case datRenderer::CursorType::Triangle:
            ofNotifyEvent(selectCursorTypeEvent, m_radio5);
            break;
    }
}



void datSelectTool::onLeftMouseButtonDown(datMouseEvent const& ev) {

    if (m_hasFirstPoint) {

        updateRectangle(ev);
        updateSelectionMode(ev);
        selectObjectsAndClearState();
        return;
    }

    m_downPoint = ev;
    m_hasFirstPoint = true;
}


void datSelectTool::onLeftMouseButtonUp(datMouseEvent const& ev) {

    if (m_hasFirstPoint && m_isMouseDrag) {

        updateRectangle(ev);
        updateSelectionMode(ev);
        selectObjectsAndClearState();
    }
}


void datSelectTool::onMouseMotion(datMouseEvent const& ev) {

    if (!m_isMouseDrag && ev.IsDrag()) {
        m_isMouseDrag = true;
    }

    if (m_hasFirstPoint) {
        updateRectangle(ev);
        updateSelectionMode(ev);
    }
}


void datSelectTool::onKeyPressed(ofKeyEventArgs const& ev) {

    const datRenderer::CursorType activeCursorType = datRenderer::GetActiveRenderer().GetActiveCursorType();
    datRenderer::CursorType cursorType = activeCursorType;

    const char key = static_cast<char>(ev.key);
    switch (key) {
    case '1':
        cursorType = datRenderer::CursorType::Normal;
        break;
    case '2':
        cursorType = datRenderer::CursorType::Circle;
        break;
    case '3':
        cursorType = datRenderer::CursorType::X;
        break;
    case '4':
        cursorType = datRenderer::CursorType::Cross;
        break;
    case '5':
        cursorType = datRenderer::CursorType::Triangle;
        break;
    default:
        break;
    }

    if (cursorType != activeCursorType) {
        datRenderer::GetActiveRenderer().SetActiveCursorType(cursorType);
    }

}


void datSelectTool::onDraw() {

    if (m_hasFirstPoint && datEpsilon < m_rectangle.getArea()) {

        ofSetColor(m_color);
        ofNoFill();
        ofDrawRectangle(m_rectangle);
        ofFill();
    }

    m_panel.draw();
}
