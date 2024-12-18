#include "System/OSX/OSXUtils.h"

#import <AppKit/NSWindow.h>

namespace Final
{
    unsigned long WindowContentViewHandle( const SDL_SysWMinfo &info )
    {
        NSWindow *window = info.info.cocoa.window;
        NSView *view = [window contentView];
        return (unsigned long)view;
    }

}
