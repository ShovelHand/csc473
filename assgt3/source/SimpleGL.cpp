#include <atlas/utils/Application.hpp>
#include "SimpleGLScene.hpp"
#include <atlas/gl/ErrorCheck.hpp>

int main()
{
    // Create a new window (only one per application for now).
    APPLICATION.createWindow(800, 800, "Simple GL");

    // Now give it our scene.
    APPLICATION.addScene(new SimpleGLScene);
    
    // Finally, run the application.
    APPLICATION.runApplication();

    return 0;
}