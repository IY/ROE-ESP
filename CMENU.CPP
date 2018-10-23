#include <windows.h>
#include "hDrawings.h"
#include "hDirectX.h"
#include "cMenu.h"

bool menuOn = false;
Menu menu;
bool nameesp = true;
bool boxesp = true;
bool distesp = true;
bool caresp = true;
bool dropesp = true;
bool crosshair = true;

void addEntry(char* name, bool *toggled)
{
    MenuEntry *entry = (MenuEntry*)calloc(1, sizeof(MenuEntry));
    char* entryName = (char*)calloc(1, 64);
    strcpy(entryName, name);
    entry->name = entryName;
    entry->toggled = toggled;

    if(menu.entries == NULL){
        menu.entries = entry;
    }else{
        MenuEntry *last = menu.entries;
        while(last->next != NULL){
            last = last->next;
        }
        last->next = entry;
    }
    menu.lastEntry++;
}

void initMenu()
{
    addEntry("Name ESP", &nameesp);
    addEntry("Box ESP", &boxesp);
    addEntry("Distance ESP", &distesp);
    addEntry("Car ESP", &caresp);
    addEntry("Airdrop ESP", &dropesp);
    addEntry("Crosshair", &crosshair);
    menu.selectedEntry = menu.entries;
}

void updateMenu()
{
    while(true){
        if(GetAsyncKeyState(VK_DELETE) & 1){
            menu.toggled = !menu.toggled;
        }else if(GetAsyncKeyState(VK_DOWN) & 1){
            if(menu.selectedEntry->next != NULL)
                menu.selectedEntry = menu.selectedEntry->next;
        }else if(GetAsyncKeyState(VK_UP) & 1){

            MenuEntry *last = menu.entries;
            while(last != NULL){
                if(last->next == menu.selectedEntry){
                    menu.selectedEntry = last;
                    break;
                }
                last = last->next;
            }

        }else if(GetAsyncKeyState(VK_RIGHT) & 1 || GetAsyncKeyState(VK_LEFT) & 1){
            *menu.selectedEntry->toggled = !*menu.selectedEntry->toggled;
        }
        Sleep(5);
    }
}

void drawEntry(MenuEntry *entry, int i)
{
    char p[64];
    sprintf(p, "%s: %d", entry->name, *entry->toggled);
    DrawShadowStringCentered(p, 105, 5 + i * 12, 255, 255, 255, (menu.selectedEntry == entry) ? 255 : 1, 1, 1, pFontSmall);
}

void drawMenu()
{
    if(menu.toggled)
    {
        int height = 8 + 12 * menu.lastEntry;
        GradientFunc(5 , 5, 200, height, 145, 232, 188, 255);
        FillRGB(8, 8 , 194, height - 6, 240, 255, 240, 255);

        int i = 0;
        MenuEntry *last = menu.entries;
        drawEntry(last, i);
        while(last->next != NULL){
            i++;
            last = last->next;
            drawEntry(last, i);
        }
    }
}
