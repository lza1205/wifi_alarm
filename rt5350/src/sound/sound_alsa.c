

/******************************************************
ALSA �����ļ򵥲���

*******************************************************/



#include "all.h"
#include "client.h"

/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <alsa/asoundlib.h>

int frames_size;


/* ������ص� */
snd_pcm_uframes_t playback_frames;
snd_pcm_t *playback_handle;


/* ��ʼ�� */
int alsa_playback_init(void)
{
	int rc;
    int size;
    snd_pcm_hw_params_t *params;
    unsigned int val;
    int dir;
    char *buffer;
	
    /* Open PCM device for playback. */
    /* �� PCM playback �豸 */
    rc = snd_pcm_open(&playback_handle, "default",
    SND_PCM_STREAM_PLAYBACK, 0);
    if (rc < 0) {
        fprintf(stderr,
                "unable to open pcm device: %s\n",
                snd_strerror(rc));
        return (-1);
    }

    /* Allocate a hardware parameters object. */
    /* ����һ��Ӳ�������ṹ�� */
    snd_pcm_hw_params_alloca(&params);

    /* Fill it in with default values. */
    /* ʹ��Ĭ�ϲ��� */
    snd_pcm_hw_params_any(playback_handle, params);

    /* Set the desired hardware parameters. */
    /* ����Ӳ������ */

    /* Interleaved mode */
    snd_pcm_hw_params_set_access(playback_handle, params,
                                SND_PCM_ACCESS_RW_INTERLEAVED);

    /* Signed 16-bit little-endian format */
    /* ���ݸ�ʽΪ 16λ С�� */
    snd_pcm_hw_params_set_format(playback_handle, params,
                                SND_PCM_FORMAT_S16_LE);

    /* Two channels (stereo) */
    /* �������� */
    snd_pcm_hw_params_set_channels(playback_handle, params, 2);

    /* 44100 bits/second sampling rate (CD quality) */
    /* ������Ϊ 44100 */
    val = 11025;
    snd_pcm_hw_params_set_rate_near(playback_handle, params,
              &val, &dir);

    /* Set period size to 32 frames. */
    /* ����һ������Ϊ 32 ֡ */
    playback_frames = 32;
    snd_pcm_hw_params_set_period_size_near(playback_handle,
          params, &playback_frames, &dir);

    /* Write the parameters to the driver */
    /* ��ǰ�����úõĲ���д�뵽playback�豸 */
    rc = snd_pcm_hw_params(playback_handle, params);
    if (rc < 0) {
        fprintf(stderr,
            "unable to set hw parameters: %s\n",
            snd_strerror(rc));
    	return (-1);
    }

    /* Use a buffer large enough to hold one period */
    /* �õ�һ�����ڵ����ݳ��� */
    snd_pcm_hw_params_get_period_size(params, &playback_frames,
                &dir);

	frames_size = playback_frames * 4; /* 2 bytes/sample, 2 channels */

	return 0;
}



int alsa_playback(char *buffer)
{
	int rc;
    /* ������Щ���� */
    rc = snd_pcm_writei(playback_handle, buffer, playback_frames);
    if (rc == -EPIPE) {
        /* EPIPE means underrun */
        fprintf(stderr, "underrun occurred\n");
        snd_pcm_prepare(playback_handle);
		return -1;
    } else if (rc < 0) {
        fprintf(stderr,
                "error from writei: %s\n",
                snd_strerror(rc));
		return -1;
    }  else if (rc != (int)playback_frames) {
        fprintf(stderr,
                "short write, write %d frames\n", rc);
		return -1;
    }
	return 0;
}


void alsa_playback_close(void)
{
	snd_pcm_drain(playback_handle);
    snd_pcm_close(playback_handle);
}



snd_pcm_t *capture_handle;
snd_pcm_uframes_t capture_frames;




int alsa_capture_init(void)
{
    int rc;
    int size;
    snd_pcm_hw_params_t *params;
    unsigned int val;
    int dir;
    
    /* Open PCM device for recording (capture). */
    /* �� PCM capture �����豸 */
    rc = snd_pcm_open(&capture_handle, "default",
                        SND_PCM_STREAM_CAPTURE, 0);
    if (rc < 0) {
        fprintf(stderr,
                "unable to open pcm device: %s\n",
                snd_strerror(rc));
        return (-1);
    }

    /* Allocate a hardware parameters object. */
    /* ����һ��Ӳ�������ṹ�� */
    snd_pcm_hw_params_alloca(&params);

    /* Fill it in with default values. */
    /* ʹ��Ĭ�ϲ��� */
    snd_pcm_hw_params_any(capture_handle, params);

    /* Set the desired hardware parameters. */

    /* Interleaved mode */
    snd_pcm_hw_params_set_access(capture_handle, params,
                          SND_PCM_ACCESS_RW_INTERLEAVED);

    /* Signed 16-bit little-endian format */
    /* 16λ С�� */
    snd_pcm_hw_params_set_format(capture_handle, params,
                                  SND_PCM_FORMAT_S16_LE);

    /* Two channels (stereo) */
    /* ˫ͨ�� */
    snd_pcm_hw_params_set_channels(capture_handle, params, 2);

    /* 44100 bits/second sampling rate (CD quality) */
    /* ������ */
    val = 11025;
    snd_pcm_hw_params_set_rate_near(capture_handle, params,
                                      &val, &dir);

    /* Set period size to 32 frames. */
    /* һ�������� 32 ֡ */
    capture_frames = 32;
    snd_pcm_hw_params_set_period_size_near(capture_handle,
                                  params, &capture_frames, &dir);

    /* Write the parameters to the driver */
    /* ������Ч */
    rc = snd_pcm_hw_params(capture_handle, params);
    if (rc < 0) {
        fprintf(stderr,
                "unable to set hw parameters: %s\n",
                snd_strerror(rc));
        return (-1);
    }

    /* Use a buffer large enough to hold one period */
    /* �õ�һ�����ڵ����ݴ�С */
    snd_pcm_hw_params_get_period_size(params,
                                          &capture_frames, &dir);

	frames_size = capture_frames * 4; /* 2 bytes/sample, 1 channels */

	return 0;
}



int alsa_capture(char *buffer)
{
	int rc;
	
	/* �������� */
	rc = snd_pcm_readi(capture_handle, buffer, capture_frames);
	if (rc == -EPIPE) {
		/* EPIPE means overrun */
		fprintf(stderr, "overrun occurred\n");
		snd_pcm_prepare(capture_handle);
		return -1;
	} else if (rc < 0) {
		fprintf(stderr,
			  "error from read: %s\n",
			  snd_strerror(rc));
		return -1;
	} else if (rc != (int)capture_frames) {
		fprintf(stderr, "short read, read %d frames\n", rc);
		return -1;
	}

	return rc;
}


void alsa_capture_close(void)
{
	snd_pcm_drain(capture_handle);
    snd_pcm_close(capture_handle);
}


