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
#include <SFML/Audio/SoundSource.hpp>
#include <SFML/Audio/ALCheck.hpp>
#include <cassert>

namespace sf
{
////////////////////////////////////////////////////////////
SoundSource::SoundSource(bool doubleSource):
    m_doubleSource(doubleSource),
    m_pan(0.f)
{
    if (m_doubleSource)
    {
        alCheck(alGenSources(2, m_sources));
        alCheck(alSourcei(m_sources[Left], AL_BUFFER, 0));
        alCheck(alSourcei(m_sources[Right], AL_BUFFER, 0));
    }
    else
    {
        alGenSources(1, &m_source);
    }
}


////////////////////////////////////////////////////////////
SoundSource::SoundSource(const SoundSource& copy):
    m_doubleSource(copy.m_doubleSource),
    m_pan(copy.m_pan)
{
    if (m_doubleSource)
    {
        alCheck(alGenSources(2, m_sources));
        alCheck(alSourcei(m_sources[Left], AL_BUFFER, 0));
        alCheck(alSourcei(m_sources[Right], AL_BUFFER, 0));
    }
    else
    {
        alCheck(alGenSources(1, &m_source));
        alCheck(alSourcei(m_source, AL_BUFFER, 0));
    }

    setPitch(copy.getPitch());
    setVolume(copy.getVolume());
    setPosition(copy.getPosition());
    setRelativeToListener(copy.isRelativeToListener());
    setMinDistance(copy.getMinDistance());
    setAttenuation(copy.getAttenuation());
}


////////////////////////////////////////////////////////////
SoundSource::~SoundSource()
{
    if (m_doubleSource)
    {
        alCheck(alSourcei(m_sources[Left], AL_BUFFER, 0));
        alCheck(alSourcei(m_sources[Right], AL_BUFFER, 0));
        alCheck(alDeleteSources(2, m_sources));
    }
    else
    {
        alCheck(alSourcei(m_source, AL_BUFFER, 0));
        alCheck(alDeleteSources(1, &m_source));
    }
}


////////////////////////////////////////////////////////////
void SoundSource::setPitch(float pitch)
{
    if (m_doubleSource)
    {
        alCheck(alSourcef(m_sources[Left], AL_PITCH, pitch));
        alCheck(alSourcef(m_sources[Right], AL_PITCH, pitch));
    }
    else
    {
        alCheck(alSourcef(m_source, AL_PITCH, pitch));
    }
}


////////////////////////////////////////////////////////////
void SoundSource::setVolume(float volume)
{
    if (m_doubleSource)
    {
        auto clamp = [](float x, float l, float r)
        {
            if (x < l)
            {
                return l;
            }
            if (x > r)
            {
                return r;
            }
            return x;
        };
        float v = volume * 0.01f;

        float left = clamp(1.f - m_pan, 0.0f, 1.f);
        float right = clamp(1.f + m_pan, 0.0f, 1.f);

        alCheck(alSourcef(m_sources[Left], AL_GAIN, left * v));
        alCheck(alSourcef(m_sources[Right], AL_GAIN, right * v));
    }
    else
    {
        alCheck(alSourcef(m_source, AL_GAIN, volume * 0.01f));
    }
}


////////////////////////////////////////////////////////////
void SoundSource::setPosition(float x, float y, float z)
{
    if (m_doubleSource)
    {
        if (!isRelativeToListener())
        {
            alCheck(alSource3f(m_sources[Left], AL_POSITION, x, y, z));
            alCheck(alSource3f(m_sources[Right], AL_POSITION, x, y, z));
        }
    }
    else
    {
        alCheck(alSource3f(m_source, AL_POSITION, x, y, z));
    }
}


////////////////////////////////////////////////////////////
void SoundSource::setPosition(const Vector3f& position)
{
    setPosition(position.x, position.y, position.z);
}


////////////////////////////////////////////////////////////
void SoundSource::setRelativeToListener(bool relative)
{
    if (m_doubleSource)
    {
        alCheck(alSourcei(m_sources[Left], AL_SOURCE_RELATIVE, relative));
        alCheck(alSourcei(m_sources[Right], AL_SOURCE_RELATIVE, relative));

        if (relative)
        {
            alSource3f(m_sources[Left], AL_POSITION, -1.0f, 0.0f, 0.0f);
            alSource3f(m_sources[Right], AL_POSITION, +1.0f, 0.0f, 0.0f);
        }
    }
    else
    {
        alCheck(alSourcei(m_source, AL_SOURCE_RELATIVE, relative));
    }
}


////////////////////////////////////////////////////////////
void SoundSource::setMinDistance(float distance)
{
    if (m_doubleSource)
    {
        alCheck(alSourcef(m_sources[Left], AL_REFERENCE_DISTANCE, distance));
        alCheck(alSourcef(m_sources[Right], AL_REFERENCE_DISTANCE, distance));
    }
    else
    {
        alCheck(alSourcef(m_source, AL_REFERENCE_DISTANCE, distance));
    }
}


////////////////////////////////////////////////////////////
void SoundSource::setAttenuation(float attenuation)
{
    if (m_doubleSource)
    {
        alCheck(alSourcef(m_sources[Left], AL_ROLLOFF_FACTOR, attenuation));
        alCheck(alSourcef(m_sources[Right], AL_ROLLOFF_FACTOR, attenuation));
    }
    else
    {
        alCheck(alSourcef(m_source, AL_ROLLOFF_FACTOR, attenuation));
    }
}

////////////////////////////////////////////////////////////
void SoundSource::setPan(float pan)
{
    assert(m_doubleSource);
    m_pan = pan;
}

////////////////////////////////////////////////////////////
float SoundSource::getPan() const
{
    return m_pan;
}
////////////////////////////////////////////////////////////
float SoundSource::getPitch() const
{
    ALfloat pitch;
    if (m_doubleSource)
    {
        alCheck(alGetSourcef(m_sources[Left], AL_PITCH, &pitch));
    }
    else
    {
        alCheck(alGetSourcef(m_source, AL_PITCH, &pitch));
    }

    return pitch;
}


////////////////////////////////////////////////////////////
float SoundSource::getVolume() const
{
    ALfloat gain;
    if (m_doubleSource)
    {
        alCheck(alGetSourcef(m_sources[Left], AL_GAIN, &gain));
        ALfloat gain2;
        alCheck(alGetSourcef(m_sources[Right], AL_GAIN, &gain2));
        gain = (gain + gain2) / 2.f;
    }
    else
    {
        alCheck(alGetSourcef(m_source, AL_GAIN, &gain));
    }

    return gain * 100.f;
}


////////////////////////////////////////////////////////////
Vector3f SoundSource::getPosition() const
{
    Vector3f position;
    if (m_doubleSource)
    {
        alCheck(alGetSource3f(m_sources[Left], AL_POSITION, &position.x, &position.y, &position.z));
    }
    else
    {
        alCheck(alGetSource3f(m_source, AL_POSITION, &position.x, &position.y, &position.z));
    }


    return position;
}


////////////////////////////////////////////////////////////
bool SoundSource::isRelativeToListener() const
{
    ALint relative;

    if (m_doubleSource)
    {
        alCheck(alGetSourcei(m_sources[Left], AL_SOURCE_RELATIVE, &relative));
    }
    else
    {
        alCheck(alGetSourcei(m_source, AL_SOURCE_RELATIVE, &relative));
    }

    return relative != 0;
}


////////////////////////////////////////////////////////////
float SoundSource::getMinDistance() const
{
    ALfloat distance;

    if (m_doubleSource)
    {
        alCheck(alGetSourcef(m_sources[Left], AL_REFERENCE_DISTANCE, &distance));
    }
    else
    {
        alCheck(alGetSourcef(m_source, AL_REFERENCE_DISTANCE, &distance));
    }

    return distance;
}


////////////////////////////////////////////////////////////
float SoundSource::getAttenuation() const
{
    ALfloat attenuation;
    if (m_doubleSource)
    {
        alCheck(alGetSourcef(m_sources[Left], AL_ROLLOFF_FACTOR, &attenuation));
    }
    else
    {
        alCheck(alGetSourcef(m_source, AL_ROLLOFF_FACTOR, &attenuation));
    }

    return attenuation;
}


////////////////////////////////////////////////////////////
SoundSource& SoundSource::operator =(const SoundSource& right)
{
    // Leave m_source untouched -- it's not necessary to destroy and
    // recreate the OpenAL sound source, hence no copy-and-swap idiom

    // Assign the sound attributes
    setPitch(right.getPitch());
    setVolume(right.getVolume());
    setPosition(right.getPosition());
    setRelativeToListener(right.isRelativeToListener());
    setMinDistance(right.getMinDistance());
    setAttenuation(right.getAttenuation());

    return *this;
}


////////////////////////////////////////////////////////////
SoundSource::Status SoundSource::getStatus() const
{
    ALint status;
    if (m_doubleSource)
    {
        alCheck(alGetSourcei(m_sources[Left], AL_SOURCE_STATE, &status));
    }
    else
    {
        alCheck(alGetSourcei(m_source, AL_SOURCE_STATE, &status));
    }

    switch (status)
    {
        case AL_INITIAL:
        case AL_STOPPED: return Stopped;
        case AL_PAUSED:  return Paused;
        case AL_PLAYING: return Playing;
    }

    return Stopped;
}

} // namespace sf
