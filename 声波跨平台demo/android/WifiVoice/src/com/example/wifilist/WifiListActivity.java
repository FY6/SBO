/************************************************************************ 
android/iphone/windows/linux����ͨѶ��
����ͨѶ�������� 
׼ȷ��95%���ϣ���ʵһ���ǲ������ġ� 
�ӿڷǳ��򵥣���������ʾ����3���ӾͿ��������Ӧ����������ͨѶ���� 
��������ǿ�����������������ô���ţ��źŶ���׼ȷ�� 
�����ı���Ϊ16���ƣ���ͨ������ɴ����κ��ַ� 
���ܷǳ�ǿ��û�����в��˵�ƽ̨������ͨ���ڴ���Ż�����ʱ����벻�ٷ������ڴ棬��7*24Сʱ���� 
��֧���κ�ƽ̨��������ƽ̨android, iphone, windows, linux, arm, mipsel����ʾ�� 
����ɲ鿴��http://blog.csdn.net/softlgh 
����: ҹ���� QQ:3116009971 �ʼ���3116009971@qq.com 
************************************************************************/  
package com.example.wifilist;

import java.util.ArrayList;
import java.util.List;

import android.content.res.Configuration;
import android.media.AudioManager;
import android.media.ToneGenerator;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.app.Activity;
import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.*;
import voice.SSIDWiFiInfo;
import voice.decoder.DataDecoder;
import voice.decoder.VoiceRecognizer;
import voice.decoder.VoiceRecognizerListener;
import voice.encoder.DataEncoder;
import voice.encoder.VoicePlayer;

public class WifiListActivity extends Activity {
    static {
        System.loadLibrary("voiceRecog");
    }

    private final static int MSG_RECG_START = 2;
    private final static int MSG_RECG_TEXT = 1;
    class MyHandler extends Handler
    {
        private TextView mRecognisedTextView;
        public MyHandler(TextView textView) {
            mRecognisedTextView = textView;
        }
        @Override
        public void handleMessage(Message msg) {
            if(msg.what == MSG_RECG_TEXT)
            {
                String s = (String)msg.obj;
                mRecognisedTextView.setText(s);
                if(s != null && s.length() > 0)
                {
                    toneGenerator.startTone(ToneGenerator.TONE_PROP_BEEP);
                    Toast toast = Toast.makeText(WifiListActivity.this, s, Toast.LENGTH_SHORT);
                    toast.show();
                }
            }
            else if(msg.what == MSG_RECG_START)
            {
                mRecognisedTextView.setText("����ʶ����......");
            }
            super.handleMessage(msg);
        }
    }

    private static String TAG = "WiFiListActivity";
	private WifiManager wifiManager;
	List<ScanResult> list;

    VoicePlayer player;//����ͨѶ������
    VoiceRecognizer recognizer;//����ͨѶʶ����
    Handler handler;
    ToneGenerator toneGenerator =
            new ToneGenerator(
                    AudioManager.STREAM_SYSTEM, ToneGenerator.MAX_VOLUME);

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		init();
	}

	private void init() {
		wifiManager = (WifiManager) getSystemService(Context.WIFI_SERVICE);
		list = wifiManager.getScanResults();
		ListView listView = (ListView) findViewById(R.id.listView);
		if (list == null) {
			Toast.makeText(this, "wifiδ�򿪣�", Toast.LENGTH_LONG).show();
		}else {
			listView.setAdapter(new MyAdapter(this,list));
		}

        //����
        final EditText pwdText = (EditText) this.findViewById(R.id.pwdText);
        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                ScanResult wifi = list.get(position);
                player.play(DataEncoder.encodeSSIDWiFi(wifi.SSID, pwdText.getText().toString()), 1, 200);
            }
        });
        final TextView recognisedTextView = (TextView) findViewById(R.id.regtext);
        handler = new MyHandler(recognisedTextView);

        autoSetAudioVolumn();
        
        //Ƶ������
        int[] freqs = new int[19];
        int baseFreq = 4000;
        for(int i = 0; i < freqs.length; i ++)
        {
        	freqs[i] = baseFreq + i * 150;
        }
        
        //��������ͨѶ������
        player = new VoicePlayer();
        player.setFreqs(freqs);

        //��������ͨѶʶ����
        recognizer = new VoiceRecognizer();
        recognizer.setFreqs(freqs);

        recognizer.setListener(new VoiceRecognizerListener() {
            @Override
            public void onRecognizeStart(float _soundTime) {
                handler.sendMessage(handler.obtainMessage(MSG_RECG_START));
            }

            @Override
            public void onRecognizeEnd(float _soundTime, int _result, String _hexData) {
                String data = "";
                if(_result == VoiceRecognizer.Status_Success)
                {               	
                	byte[] hexData = _hexData.getBytes();
                	int infoType = DataDecoder.decodeInfoType(hexData);
                	if(infoType == DataDecoder.IT_STRING)
                	{
                		data = DataDecoder.decodeString(_result, hexData);
                	}
                	else if(infoType == DataDecoder.IT_SSID_WIFI)
                	{
                		SSIDWiFiInfo wifi = DataDecoder.decodeSSIDWiFi(_result,  hexData);
                		data = "ssid:" + wifi.ssid + ",pwd:" + wifi.pwd; 
                	}
                	else
                	{
                		data = "δ֪����";
                	}
                }
                handler.sendMessage(handler.obtainMessage(MSG_RECG_TEXT, data));
            }
        });
        recognizer.start();
	}

    @Override
    protected void onPause() {
        super.onPause();
        recognizer.stop();
    }

    @Override
    protected void onResume() {
        super.onResume();
        recognizer.start();
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
    }

    //��������Ϊ60%
    public void autoSetAudioVolumn()
    {
        AudioManager mAudioManager = (AudioManager) getSystemService(Context.AUDIO_SERVICE);
        int max = mAudioManager.getStreamMaxVolume( AudioManager.STREAM_MUSIC );
        mAudioManager.setStreamVolume(AudioManager.STREAM_MUSIC, (int)(max*0.6), 0);
    }

	public class MyAdapter extends BaseAdapter {

		LayoutInflater inflater;
		List<ScanResult> list;
		public MyAdapter(Context context, List<ScanResult> list) {
			// TODO Auto-generated constructor stub
			this.inflater = LayoutInflater.from(context);
			this.list = list;
		}

		@Override
		public int getCount() {
			// TODO Auto-generated method stub
			return list.size();
		}

		@Override
		public Object getItem(int position) {
			// TODO Auto-generated method stub
			return position;
		}

		@Override
		public long getItemId(int position) {
			// TODO Auto-generated method stub
			return position;
		}

		@Override
		public View getView(int position, View convertView, ViewGroup parent) {
			// TODO Auto-generated method stub
			View view = null;
			view = inflater.inflate(R.layout.item_wifi_list, null);
			ScanResult scanResult = list.get(position);
			TextView textView = (TextView) view.findViewById(R.id.textView);
			textView.setText(scanResult.SSID);
			TextView signalStrenth = (TextView) view.findViewById(R.id.signal_strenth);
			signalStrenth.setText(String.valueOf(Math.abs(scanResult.level)));
			ImageView imageView = (ImageView) view.findViewById(R.id.imageView);
			//�ж��ź�ǿ�ȣ���ʾ��Ӧ��ָʾͼ��
			if (Math.abs(scanResult.level) > 100) {
				imageView.setImageDrawable(getResources().getDrawable(R.drawable.stat_sys_wifi_signal_0));
			} else if (Math.abs(scanResult.level) > 80) {
				imageView.setImageDrawable(getResources().getDrawable(R.drawable.stat_sys_wifi_signal_1));
			} else if (Math.abs(scanResult.level) > 70) {
				imageView.setImageDrawable(getResources().getDrawable(R.drawable.stat_sys_wifi_signal_1));
			} else if (Math.abs(scanResult.level) > 60) {
				imageView.setImageDrawable(getResources().getDrawable(R.drawable.stat_sys_wifi_signal_2));
			} else if (Math.abs(scanResult.level) > 50) {
				imageView.setImageDrawable(getResources().getDrawable(R.drawable.stat_sys_wifi_signal_3));
			} else {
				imageView.setImageDrawable(getResources().getDrawable(R.drawable.stat_sys_wifi_signal_4));
			}
			return view;
		}

	}

}
