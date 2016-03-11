// generate USB keycodes based on /tmp/a.txt text.
// see https://www.win.tue.nl/~aeb/linux/kbd/scancodes-14.html for codes.
#include <stdio.h>
main()
{
  FILE *fp,*fp_hid;
  int i,code;
  char c;
  char report[8];
  
  for (i=0; i<8; i++) report[i]=0;
  fp=fopen("/tmp/a.txt","r");
  while (!feof(fp)) {
    fp_hid=fopen("/dev/hidg0","w");
    c=fgetc(fp);
    code=0;
    if (c>64 && c<91) code=c-'A'+4; // uppercase letters
    if (c>96 && c<123) code=c-'a'+4; // lowercase letters
    if (c==32) code=44; // space
    if (c>48 && c<58) code=c-19; // number 1-9
    // push SHIFT if uppoercase......
    if (c>64 && c<91) report[0]=2;
    report[2]=code; fwrite(report,8,1,fp_hid);
    report[0]=0;  report[2]=0; fwrite(report,8,1,fp_hid);
    fclose(fp_hid);
  }
  // send a final newline....
  fp_hid=fopen("/dev/hidg0","w");
  report[2]=40;  fwrite(report,8,1,fp_hid);
  report[0]=0;  report[2]=0; fwrite(report,8,1,fp_hid);
  fclose(fp_hid);
  fclose(fp);
}
