#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <speex/speex_preprocess.h>

#define FSIZE 640	      
#define SILENCE 15
	      
main (int argc, char *argv[])
{
  FILE *fp;
  int i,n,tot,posn=0,on=0,silence=0,ssize;
  int err;
  short buf[FSIZE*2];
  snd_pcm_t *handle;
  snd_pcm_hw_params_t *hw_params;
  int rate=16000;
  short sample[1000000];
  short s[FSIZE],s1[FSIZE],s2[FSIZE];
  SpeexPreprocessState *st;
  float e;

  st = speex_preprocess_state_init(FSIZE, rate);
  i=1;
  speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DENOISE, &i);
  i=1;
  speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_AGC, &i);
  e=8000;
  speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_AGC_LEVEL, &e);
  i=1;
  speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB, &i);
  e=.0;
  speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB_DECAY, &e);
  e=.0;
  speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB_LEVEL, &e);
  

  snd_pcm_open(&handle, "default", SND_PCM_STREAM_CAPTURE, 0);	
  snd_pcm_hw_params_malloc(&hw_params);			 
  snd_pcm_hw_params_any(handle, hw_params);
  snd_pcm_hw_params_set_access(handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);	
  snd_pcm_hw_params_set_format(handle, hw_params, SND_PCM_FORMAT_S16_LE);	
  snd_pcm_hw_params_set_rate_near(handle, hw_params, &rate, 0);
  snd_pcm_hw_params_set_channels(handle, hw_params, 2);
  snd_pcm_hw_params(handle, hw_params);
  snd_pcm_hw_params_free(hw_params);
  snd_pcm_prepare(handle);
  
  
  loop:
    snd_pcm_readi(handle, buf, FSIZE);
    for (n=0; n<FSIZE; n++) s[n]=buf[n*2];
    speex_preprocess_run(st, s); // denoise sample[]....
    tot=0; for (i=0; i<FSIZE; i++) tot+=abs(s[i]);
    if (tot>30000 && posn>100) {
     if (on==0) {on=1; silence=0; ssize=FSIZE*2; 
     		bcopy(s2,sample,FSIZE*2);
		bcopy(s1,&sample[FSIZE],FSIZE*2);}    
    } else  silence++;
    if (on) {bcopy(s,&sample[ssize],FSIZE*2); ssize+=FSIZE;}
    if (on && silence>SILENCE) { // finish sample, write it out
      silence=0; on=0;
      fp=fopen("/tmp/a.raw","w"); fwrite(sample,ssize,2,fp); fclose(fp);
      ssize=0;
    }
    
    printf("tot=%i on=%i silence=%i\n",tot,on,silence);
    posn++;
    bcopy(s1,s2,FSIZE*2); bcopy(s,s1,FSIZE*2);
  goto loop;	
  snd_pcm_close(handle);
  exit(0);
}
