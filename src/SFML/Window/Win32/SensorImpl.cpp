////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2017 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/SensorImpl.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
void SensorImpl::initialize()
{
    // To be implemented
}


////////////////////////////////////////////////////////////
void SensorImpl::cleanup()
{
    // To be implemented
}


////////////////////////////////////////////////////////////
bool SensorImpl::isAvailable(Sensor::Type /*sensor*/)
{
    // To be implemented
    return false;
}


////////////////////////////////////////////////////////////
bool SensorImpl::open(Sensor::Type /*sensor*/)
{
    // To be implemented
    return false;
}


////////////////////////////////////////////////////////////
void SensorImpl::close()
{
    // To be implemented
}


////////////////////////////////////////////////////////////
Vector3f SensorImpl::update()
{
    // To be implemented
    return Vector3f(0, 0, 0);
}


////////////////////////////////////////////////////////////
void SensorImpl::setEnabled(bool /*enabled*/)
{
    // To be implemented
}

} // namespace priv

} // namespace sf
