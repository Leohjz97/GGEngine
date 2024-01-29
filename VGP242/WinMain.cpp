#include "Core/Inc/Core.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	while(true)
	{
		LOG("New Log");
		Sleep(0);
		ASSERT(false, "JUST TESTING ASSERT");
	}
    //ASSERT(false, "test if it works");
	
	//game init
	//game run
	//game cleanup

	//


    return 0;
}