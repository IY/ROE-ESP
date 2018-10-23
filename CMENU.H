void updateMenu();
void drawMenu();
void initMenu();

extern bool nameesp;
extern bool boxesp;
extern bool distesp;
extern bool caresp;
extern bool dropesp;
extern bool crosshair;

struct MenuEntry{
    char* name;
    bool *toggled;
    MenuEntry *next;
};

struct Menu
{
    bool toggled = NULL;
    MenuEntry *entries = NULL;
    MenuEntry *selectedEntry = NULL;
    int lastEntry = 0;
    bool boxESP = true;
    bool distanceESP = true;
    bool crosshair = true;
};


