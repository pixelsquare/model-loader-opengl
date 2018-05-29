#ifndef _MODELPARSER_H_
#define _MODELPARSER_H_

void ExtractVertexIdx(char *Face, char *Target)
{
 int i=0;
 while (Face[i] != '\0')
 {
  if (Face[i] == '/')
  {
   Target[i] = '\0';
   break;
  }
  else
   Target[i] = Face[i];
  i++;
 }
 return;
}


void ExtractNormalIdx(char *Face, char *Target)
{
 int sCount=0;//Slash count
 int i=0;
 int j=0;
 
 while (Face[i] != '\0')
 {
  if (Face[i] == '/')
  {
   sCount++;
  }
  else
  {
   if (sCount == 2)
   {
    Target[j] = Face[i];
    j++;
   }
  }
  i++;
 }
 Target[j] = '\0';
 return;
}

bool ContainSlash(char *String, int count)
{
 for (int i=0; i<count; i++)
 {
  if (String[i] == '/')
   return true;
  else if (String[i] == '\0')
   break;
 }

 return false;
}

#endif