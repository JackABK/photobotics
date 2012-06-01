package ca.photobotics.client.main;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;

import photobotics.socket.client.R;

import android.app.Activity;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends Activity {
	
	EditText textOut;
	TextView textIn;
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
	
	
	textOut = (EditText) findViewById(R.id.status);
	Button buttonSend = (Button) findViewById(R.id.takepic);
	textIn = (TextView) findViewById(R.id.commandtxt);
	buttonSend.setOnClickListener(buttonSendOnClickListener);
	}
	
}

class AsyncSocketConnect extends AsyncTask<Void, Void, Void> {

	public static String serverip = "192.168.1.21";
	public static int serverport = 8888;
	Socket s;
	public DataInputStream dis;
	public DataOutputStream dos;
	public int message;

	@Override
	protected Void doInBackground(Void... params) {

		try {
			Log.i("AsyncTank", "doInBackgoung: Creating Socket");
			s = new Socket(serverip, serverport);
		} catch (Exception e) {
			Log.i("AsyncTank", "doInBackgoung: Cannot create Socket");
		}
		if (s.isConnected()) {
			try {
				dis = (DataInputStream) s.getInputStream();
				dos = (DataOutputStream) s.getOutputStream();
				Log.i("AsyncTank",
						"doInBackgoung: Socket created, Streams assigned");

			} catch (IOException e) {
				// TODO Auto-generated catch block
				Log.i("AsyncTank",
						"doInBackgoung: Cannot assign Streams, Socket not connected");
				e.printStackTrace();
			}
		} else {
			Log.i("AsyncTank",
					"doInBackgoung: Cannot assign Streams, Socket is closed");
		}

		return null;
	}

	public void writeToStream(double lat, double lon) {
		try {
			if (s.isConnected()) {
				Log.i("AsynkTask", "writeToStream : Writing lat, lon");
				dos.writeDouble(lat);
				dos.writeDouble(lon);
			} else {
				Log.i("AsynkTask",
						"writeToStream : Cannot write to stream, Socket is closed");
			}
		} catch (Exception e) {
			Log.i("AsynkTask", "writeToStream : Writing failed");
		}
	}

	public int readFromStream() {
		try {
			if (s.isConnected()) {
				Log.i("AsynkTask", "readFromStream : Reading message");
				message = dis.readInt();
			} else {
				Log.i("AsynkTask",
						"readFromStream : Cannot Read, Socket is closed");
			}
		} catch (Exception e) {
			Log.i("AsynkTask", "readFromStream : Writing failed");
		}
		return message;
	}

}