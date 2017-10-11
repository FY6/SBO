#ifdef VOICE_RECOG_DLL
#define VOICERECOGNIZEDLL_API __declspec(dllexport)
#else
#define VOICERECOGNIZEDLL_API
#endif

#ifndef VOICE_PLAY_H
#define VOICE_PLAY_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*vp_pPlayerStartListener)(void *_listener);
typedef void (*vp_pPlayerEndListener)(void *_listener);

/************************************************************************/
/* ��������˷��Ͷ���
/************************************************************************/
VOICERECOGNIZEDLL_API void * vp_createVoicePlayer();

/************************************************************************/
/* ���Ͷ˱��벢��������
/* �ú������첽����ʽ���������������̣߳��ж��Ƿ�������������vp_isStopped
/* _dataΪ�������ݣ���ʮ�����Ƶ���ʽ����
/* _dataLenΪ�跢�͵��������ݳ���
/************************************************************************/
VOICERECOGNIZEDLL_API void vp_play(void *_player, char *_data, int _dataLen, int _playCount, int _muteInterval);

//���ò��ż�����
VOICERECOGNIZEDLL_API void vp_setPlayerListener(void *_player, void *_listener, vp_pPlayerStartListener _startListener, vp_pPlayerEndListener _endListener);

/************************************************************************/
/* �����������Ͷ�����
/* _volumeΪ0-1֮��ĸ�������0Ϊ������1Ϊ���
/************************************************************************/
VOICERECOGNIZEDLL_API void vp_setVolume(void *_player, double _volume);

/************************************************************************/
/* �����������Ͷ�Ƶ��
/************************************************************************/
VOICERECOGNIZEDLL_API void vp_setFreqs(void *_player, int *_freqs, int _freqCount);

/************************************************************************/
/* play�������첽�ķ�ʽ���Ų����ͣ������ٶ���֮ǰ��ȷ�������Ѿ����
/************************************************************************/
VOICERECOGNIZEDLL_API int vp_isStopped(void *_player);

/************************************************************************/
/* �����������Ͷ˶���
/************************************************************************/
VOICERECOGNIZEDLL_API void vp_destoryVoicePlayer(void *_player);





//Ӧ�ò㷢�ͷ��ӿ�

//����4bit����
int vp_encodeData(char *_data, int _dataLen, char *_result);

VOICERECOGNIZEDLL_API void vp_playString(void *_player, char *_str, int _playCount, int _muteInterval);

/************************************************************************/
/* ���Ͷ˱��벢����wifi��Ϣ����mac,pwd����ʽ
/************************************************************************/
VOICERECOGNIZEDLL_API void vp_playWiFi(void *_player, char *_mac, int _macLen, char *_pwd, int _pwdLen, int _playCount, int _muteInterval);

/************************************************************************/
/* ���Ͷ˱��벢����wifi��Ϣ����ssid,pwd����ʽ
/************************************************************************/
VOICERECOGNIZEDLL_API void vp_playSSIDWiFi(void *_player, char *_ssid, int _ssidLen, char *_pwd, int _pwdLen, int _playCount, int _muteInterval);

/************************************************************************/
/* ���Ͷ˱��벢�����ֻ���־��Ϣ����imei,�ֻ����֣�
/************************************************************************/
VOICERECOGNIZEDLL_API void vp_playPhone(void *_player, char *_imei, int _imeiLen, char *_phoneName, int _nameLen, int _playCount, int _muteInterval);


#ifdef __cplusplus
}
#endif


#endif
