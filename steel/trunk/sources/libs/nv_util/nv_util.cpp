/*********************************************************************NVMH1****
File:
nv_util.cpp

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/
#include "nv_util.h"

namespace nv_util
{
    char *   g_search_path = 0;
    char     g_default_search_path[]=
            ".;"
            "../../../MEDIA;"
            "../../../../MEDIA;"
            "../../../../../../../MEDIA;"
            "../../../MEDIA/textures/2D;"
            "../../../../MEDIA/textures/2D;"
            "../../../../../../../MEDIA/textures/2D;"
            "../../../MEDIA/textures/cubemaps;"
            "../../../../MEDIA/textures/cubemaps;"
            "../../../../../../../MEDIA/textures/cubemaps;"
            "../../../MEDIA/textures/rectangles;"
            "../../../../MEDIA/textures/rectangles;"
            "../../../../../../../MEDIA/textures/rectangles;"
            "../../../MEDIA/models;"
            "../../../../MEDIA/models;"
            "../../../../../../../MEDIA/models;"

            /*"./data;"
            "./data/images;"
            "./data/models;"
            "./data/cubemaps;"
            "./data/programs;"
            "./data/models/ase;"
            "./data/models/bbz;"
            "./data/models/md2;"
            "./data/models/x;"
            "./data/sounds;"

            "../../data;"
            "../../data/images;"
            "../../data/models;"
            "../../data/cubemaps;"
            "../../data/programs;"
            "../../data/models/ase;"
            "../../data/models/bbz;"
            "../../data/models/md2;"
            "../../data/models/x;"
            "../../data/sounds;"

            "../../../Common/media;"
            "../../../Common/media/models;"
            "../../../Common/media/models/ase;"
            "../../../Common/media/models/bbz;"
            "../../../Common/media/models/md2;"
            "../../../Common/media/models/x;"
            "../../../Common/media/textures/images;"
            "../../../Common/media/textures/cubemaps;"
            "../../../Common/media/textures/lighting;"
            "../../../Common/media/textures/Caustic;"
            "../../../Common/media/textures/CA;"
            "../../../Common/media/programs;"
            "../../../Common/media/sounds;"

            "../../../../Common/media;"
            "../../../../Common/media/models;"
            "../../../../Common/media/models/ase;"
            "../../../../Common/media/models/bbz;"
            "../../../../Common/media/models/md2;"
            "../../../../Common/media/models/x;"
            "../../../../Common/media/textures/images;"
            "../../../../Common/media/textures/cubemaps;"
            "../../../../Common/media/textures/lighting;"
            "../../../../Common/media/textures/Caustic;"
            "../../../../Common/media/textures/CA;"
            "../../../../Common/media/programs;"
            "../../../../Common/media/sounds;"*/
            ;

    const char *    set_search_path(const char * path)
    {
        if (g_search_path)
        {
            delete [] g_search_path;
            g_search_path = 0;
        }

        if (path)
        {
            int len = strlen(path);
            g_search_path = new char[len + 1];
            strcpy(g_search_path,path);
        }

        return get_search_path();
    }

    const char *    get_search_path()
    {
        return g_search_path != 0 ? g_search_path : g_default_search_path;
    }

    bool findfile(const char * filename, int size, char * pathname)
    {
        static char curr_pathname[1024];
        static char curr_path[1024];

        const char * ptr = get_search_path();
        const char * idx = strchr(ptr,';');

        while (idx)
        {
            if (idx - ptr <= 0)
                break;

            memcpy( curr_path, ptr, idx - ptr);
            curr_path[idx - ptr] = '\0';

            sprintf(curr_pathname, "%s/%s",curr_path,filename);

		    FILE * fp = ::fopen(curr_pathname, "r" );

		    if(fp != 0)
            {
                int len = strlen(curr_pathname) + 1;
                if (len > size)
                    len = size;
                strncpy(pathname,curr_pathname, len);
                fclose(fp);
			    return true;
            }
            ptr = idx + 1;
            idx = strchr(ptr,';');
        }

        fprintf(stderr,"file not found: %s .\n",filename);
        fprintf(stderr,"search path: %s .\n",get_search_path());
        return false;
    }
}
