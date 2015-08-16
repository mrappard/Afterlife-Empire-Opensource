#ifndef After_Life_Empire_Window_OSX_Linux_fix
#define After_Life_Empire_Window_OSX_Linux_fix


#if defined(_WIN32)

    #define sprintfAL sprintf_s
    #define fopenAL fopen_s

#else

    #define sprintfAL sprintf



#endif
#endif