/*
-----------------------------------------------------------------------------
This source file is part of OGRE-Next
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2016 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#import "HelloViewController.h"
#import "System/iOS/AppDelegate.h"

#import <simd/simd.h>

#include "OgreWindow.h"

#include "GameState.h"
#include "GraphicsSystem.h"
#include "LogicSystem.h"

#include "System/MainEntryPoints.h"

using namespace Final;

@implementation HelloViewController
{
    Final::GameState *_graphicsGameState;
    Final::GraphicsSystem *_graphicsSystem;
    CADisplayLink *_timer;

    double _timeSinceLast;
    CFTimeInterval _startTime;
}

- (void)dealloc {
    [self shutdownOgre];
}

- (void)shutdownOgre {
    if( _graphicsGameState )
    {
        _graphicsSystem->destroyScene();
        _graphicsSystem->deinitialize();
    }

    MainEntryPoints::destroySystems( _graphicsGameState, _graphicsSystem, 0, 0 );
    _graphicsGameState = 0;
    _graphicsSystem = 0;
}

- (void)viewDidLoad {
    [super viewDidLoad];

    if( !_graphicsSystem )
    {
        MainEntryPoints::createSystems( &_graphicsGameState, &_graphicsSystem, 0, 0 );
        _graphicsSystem->initialize( "Finalverse: Hello Worlds!" );

        _graphicsSystem->createScene01();
        _graphicsSystem->createScene02();
    }

    // Connect the UIView created by Ogre to our UIViewController
    Ogre::Window *renderWindow = _graphicsSystem->getRenderWindow();
    void *uiViewPtr = 0;
    renderWindow->getCustomAttribute( "UIView", &uiViewPtr );
    UIView *uiView = CFBridgingRelease( uiViewPtr );
    self.view = uiView;
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];

    // Create the timer required by Metal. iOS will call us at fixed intervals.
    if( _timer )
    {
        [_timer invalidate];
        _timer = nullptr;
    }
    // create a game loop timer using a display link
    _timer = [[UIScreen mainScreen] displayLinkWithTarget:self selector:@selector( mainLoop )];
    _timer.frameInterval = 1;  // VSync to 60 FPS
    [_timer addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];

    _timeSinceLast = 1.0 / 60.0;
    _startTime = CACurrentMediaTime();
}

- (void)viewWillDisappear:(BOOL)animated {
    if( _timer )
    {
        [_timer invalidate];
        _timer = nullptr;
    }

    [super viewWillDisappear:animated];
}

- (void)mainLoop {
    CFTimeInterval endTime = CACurrentMediaTime();
    _timeSinceLast = endTime - _startTime;
    _timeSinceLast = std::min( 1.0, _timeSinceLast );  // Prevent from going haywire.
    _startTime = endTime;

    _graphicsSystem->beginFrameParallel();
    _graphicsSystem->update( _timeSinceLast );
    _graphicsSystem->finishFrameParallel();
    _graphicsSystem->finishFrame();
}

@end

int main( int argc, char *argv[] )
{
    @autoreleasepool
    {
        return UIApplicationMain( argc, argv, nil, NSStringFromClass( [AppDelegate class] ) );
    }
}
