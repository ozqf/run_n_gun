// TODO: Generic implementation of rng_main_menu for use elsewhere
#include <zengine.h>

/*
<struct><labels>
*/

struct TextMenu
{
	char* header;
	char** itemLabels;
	char** itemTags;
	i32 itemCount;
	i32 itemMax;
	i32 index;
	char* buf;
	i32 bufSize;
	zeHandle charSheetHandle;

    void MoveUp()
    {
        if (itemCount <= 0) { return; }
        index--;
        if (index <= 0)
        {
            index = itemCount - 1;
        }
    }

    void MoveDown()
    {
        if (itemCount <= 0) { return; }
        index++;
        if (index >= itemCount)
        {
            index = 0;
        }
    }
};

ze_external TextMenu* TextMenu_Create();

ze_external TextMenu* TextMenu_Create()
{
    return NULL;
}
