// cmdmp3win
// A command-line MP3 player for Windows
// (window-mode version)
//
// License: MIT / X11
// Copyright (c) 2009, 2015, 2022-2024 by James K. Lawless
// jimbo@radiks.net  https://jiml.us
// See https://jiml.us/license2022.htm
//
// To build, use the following command:
//   gcc cmdmp3win.c -lwinmm -lshlwapi -luser32 -mwindows -o cmdmp3win.exe -v
//   cl cmdmp3win.c winmm.lib shlwapi.lib user32.lib /Fe:cmdmp3win-vs.exe

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <shlwapi.h>

char msg[256];
char *title="cmdmp3win v2.30";

char *parse_arg(char *);
void sendCommand(char *);

int WINAPI WinMain( HINSTANCE hInstance, 
					HINSTANCE hPrevInstance, 
					LPSTR lpCmdLine, 
					int nCmdShow ) {
    
    char cmdBuff[MAX_PATH + 64];
    char *arg;
    
    arg=parse_arg(lpCmdLine);
    TCHAR szFileName[MAX_PATH];
    GetModuleFileName(NULL, szFileName, MAX_PATH);
    PathStripPath(szFileName);
    
    if(arg==NULL) {
        sprintf(msg,"Syntax:\n\t%s \"c:\\path to file\\file.mp3\"\n\t   or\n\t%s \"url\"\n\n",szFileName,szFileName);
        MessageBox(NULL,msg,title,MB_OK);
        return 1;
    }
    sendCommand("Close All");
    sprintf(cmdBuff,"Open \"%s\" Type MPEGVideo Alias theMP3",arg);
    sendCommand(cmdBuff);

    printf("Playing: \"%s\"\n",arg);
    sendCommand("Play theMP3 Wait");
    return 0;
}

    // parse a single command-line argument.
    // remove enclosing double-quotes if present
    // return a new string
char *parse_arg(char *s) {
    char *p;
    char c;
    p=strdup(s);
    while((*p==' ')||(*p=='\t'))
        p++;
    c=*p;
    if(c=='\"')
        p=strtok(p+1,"\"");
    else
        p=strtok(p," \t");
    return p;
}

   // Send a string to the Media Control Interface
   // If an error occurs, display it and the string
   // that produced the error.
void sendCommand(char *s) {
   int i;
   i=mciSendString(s,NULL,0,0);
   if(i) {
         sprintf(msg,"Error %d when sending %s\n",i,s);
         MessageBox(NULL,msg,"cmdmp3win",MB_OK);
   }
}
