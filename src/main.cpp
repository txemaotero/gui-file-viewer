#include "application.h"
#include "logging/log.h"


int main()
{
    Log::init();

    Application* app = new Application();
    app->Run();
    delete app;

    return 0;
}
