#include "Timeline.h"




Timeline::Timeline(char* tag)
{
  
    numCommands = 0;
    currentCommand = 0;

    FILE* fin = fopen("Data/Timeline.txt","rt");
    if(fin==NULL)
    {
        printf("Data/Timeline.txt not found");
        exit(0);   
    }
    int i;
    bool flag = false;
    double time = 0;
    
    char templine[256];
    
    while(!feof(fin))
    {
        fgets(templine,255,fin);
        
        if(strstr(templine,"/*"))
        {
            while(!feof(fin))
            {
                fgets(templine,255,fin);
                if(strstr(templine,"*/"))
                    break;
                    
            }
            
            if(feof(fin))
                break;
            continue;
        }
        if(templine[0]=='/'&&templine[1]=='/')
            continue;
        if(strstr(templine,"TIM")!=NULL)
        {
            sscanf(templine,"TIM %lf",&time);
            
        }
        if(strstr(templine,tag)!=NULL)
        {
            sprintf(commands[numCommands++],"%lf %s",time,templine+4);
            
            
        }
    }
    fclose(fin);
}


char* Timeline::readNextCommand()
{
    if(currentCommand==numCommands)
        return " ";
    return commands[currentCommand++];
    

}

void Timeline::displayLines()
{
    for(int i=0; i<numCommands; i++)
    { printf((const char*)commands);}
    
}

Timeline::~Timeline()
{
 /*   for(vector<char*>::iterator it = lines.begin(); it!=lines.end(); ++it)
    { delete[] *it;}
*/
}
