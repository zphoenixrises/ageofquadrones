#include "Timeline.h"




Timeline::Timeline(string tag)
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
    
 //   printf("Executing %s:\n",tag);
    while(!feof(fin))
    {
        fgets(templine,255,fin);
        if(templine[0]=='/'&&templine[1]=='/')
            continue;
        if(templine[0]=='/'&&templine[1]=='*')
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
        //printf(templine);
        if(strstr(templine,"TIM")==templine)
        {
       //     printf(templine);
            sscanf(templine,"TIM %lf",&time);
            
        }
        if(strstr(templine,tag.c_str())==templine)
        {
            sprintf(commands[numCommands++],"%lf %s",time,templine+4);
          //  printf(commands[numCommands-1]);
             
        }
    }
    fclose(fin);
    printf("Displaying %s:\n",tag.c_str());
    displayLines();
    printf("\n");
}


char* Timeline::readNextCommand(double &peektime)
{
    if(currentCommand==numCommands)
        return " ";
    if(currentCommand<numCommands-1)
        sscanf(commands[currentCommand+1],"%lf",&peektime);
    else
        peektime = 240;
    return commands[currentCommand++];
    

}

void Timeline::displayLines()
{
    for(int i=0; i<numCommands; i++)
    { printf(commands[i]);}
    
}

Timeline::~Timeline()
{
 /*   for(vector<char*>::iterator it = lines.begin(); it!=lines.end(); ++it)
    { delete[] *it;}
*/
}
