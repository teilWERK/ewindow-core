#include "volumemanager.h"

void VolumeManager::setVolume(int percent)
{
    std::stringstream cmd;
    cmd << "amixer set Master " << percent << "%";
    system(cmd.str().c_str());
}
