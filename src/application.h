#pragma once

#include "events/event.h"
#include "pch.h"
#include "window.h"

class Application
{
public:
    Application();
    virtual ~Application();

    void Run();

    void OnEvent(Event& e);
private:
    std::unique_ptr<Window> m_Window;
    bool m_Running = true;
};
