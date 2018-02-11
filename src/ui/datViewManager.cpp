//=======================================================================================
// datViewManager.cpp
// IFT3100-13
//=======================================================================================
#include "datViewManager.h"

USING_DAT_NAMESPACE

datViewManager::datViewManager() :
    m_viewWithMouseInside(nullptr),
    m_mainView(new datView()) {
}


datViewManager::~datViewManager() {

}


datView* datViewManager::FindTargetView(ofVec2f const& point) const {

    datView* pView = m_mainView.get();

    while (pView->IsVisible() && pView->inside(point)) {

        int i = 0;
        for (i = 0; i < pView->m_views.size(); ++i) {

            datView* pChild = pView->m_views[i].get();
            if (nullptr != pChild && pChild->IsVisible() && pChild->inside(point)) {
                pView = pChild;
                break;
            }
        }

        if (0 == i || i == pView->m_views.size())
            break;
    }
    return pView;
}


datView& datViewManager::GetMainView() const {
    assert(nullptr != m_mainView);
    return *m_mainView;
}


datView* datViewManager::GetViewByNameInternal(datView& view, std::string const& name) const {

    if (view.GetName() == name)
        return &view;

    datView* pView = nullptr;
    for (auto const& pChild : view.m_views) {

        pView = (nullptr != pChild) ? GetViewByNameInternal(*pChild, name) : nullptr;
        if (nullptr != pView)
            break;
    }
    return pView;
}

datView* datViewManager::GetViewByName(std::string const& name) const {
    return GetViewByNameInternal(GetMainView(), name);
}


bool datViewManager::SendMouseEvent(datMouseEvent const& ev) {

    datView* pTarget = FindTargetView(ev);
    
    if (nullptr != m_viewWithMouseInside && (nullptr == pTarget || pTarget != m_viewWithMouseInside)) {
        m_viewWithMouseInside->m_isMouseInside = false;
        m_viewWithMouseInside->onBlur();
        m_viewWithMouseInside = nullptr;
    }

    if (nullptr == pTarget)
        return false;

    if (nullptr == m_viewWithMouseInside) {
        m_viewWithMouseInside = pTarget;
        m_viewWithMouseInside->m_isMouseInside = true;
        m_viewWithMouseInside->onHover();
    }

    bool isHandled = false;

    if (ofMouseEventArgs::Type::Pressed == ev.type) {

        if (OF_MOUSE_BUTTON_LEFT == ev.button) {
            isHandled = m_viewWithMouseInside->onLeftMouseButtonDown(ev);
        }
        else if (OF_MOUSE_BUTTON_RIGHT == ev.button) {
            isHandled = m_viewWithMouseInside->onRightMouseButtonDown(ev);
        }
    }
    else if (ofMouseEventArgs::Type::Released == ev.type) {

        if (OF_MOUSE_BUTTON_LEFT == ev.button) {
            isHandled = m_viewWithMouseInside->onLeftMouseButtonUp(ev);
        }
        else if (OF_MOUSE_BUTTON_RIGHT == ev.button) {
            isHandled = m_viewWithMouseInside->onRightMouseButtonUp(ev);
        }
    }
    else if (ofMouseEventArgs::Type::Moved == ev.type) {
        m_viewWithMouseInside->onMouseMotion(ev);
    }

    return isHandled;
}


void datViewManager::DoDrawInternal(datView& view) const {

    if (!view.IsVisible())
        return;

    view.onDraw();

    for (auto& pChild : view.m_views) {

        if (nullptr != pChild) {
            DoDrawInternal(*pChild);
        }
    }
}


void datViewManager::DoDraw() {
    DoDrawInternal(GetMainView());
}
