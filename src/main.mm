#include <Foundation/Foundation.h>
#include <string>

std::string iosResourcePath() {
    auto resourcePath = [[NSBundle mainBundle] resourcePath];
    return resourcePath ? [resourcePath UTF8String] : "";
}