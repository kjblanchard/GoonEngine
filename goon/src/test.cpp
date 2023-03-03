#include <sound/sound.h>

void playMusic()
{
    int result = InitializeSound();
    printf("The result is %d", result);
    Bgm *bgm = LoadBgmFromLua("victory2.ogg", 28.220, 51.255);
    PlayBgm(bgm, 1.0);
    while (1)
    {
        UpdateSound();
    }
}
