package ca.photobotics.client.main;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;

import android.app.Activity;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends Activity {

	EditText statusText;
	TextView commandText;
	static AsyncSocketConnect task;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		task = new AsyncSocketConnect();
		task.execute();
		statusText = (EditText) findViewById(R.id.status);
		commandText = (TextView) findViewById(R.id.commandtxt);
		// Button buttonSend = (Button) findViewById(R.id.takepic);
		// buttonSend.setOnClickListener(buttonSendOnClickListener);
		Button buttonSend = (Button) findViewById(R.id.sendcmd);
		buttonSend.setOnClickListener(buttonPicClickListener);
	}

	public void takePic(View view) {

		statusText.setText("starting socket");
		//AsyncSocketConnect task = new AsyncSocketConnect();
		
		task.SendDataToNetwork("please Work");

	}

	private OnClickListener buttonSendOnClickListener = new OnClickListener() {

		@Override
		public void onClick(View v) {
			// TODO Auto-generated method stub
			statusText.setText("Send");

		}
	};

	private OnClickListener buttonPicClickListener = new OnClickListener() {

		@Override
		public void onClick(View v) {
			// TODO Auto-generated method stub
			statusText.setText("Command");
		}
	};

	private class AsyncSocketConnect extends AsyncTask<Void, Void, Void> {

		String serverip = "192.168.7.2";
		int serverport = 8888;
		Socket socket;
		public DataInputStream socketInputStream;
		public DataOutputStream socketOutputStream;
		public int message;

		@Override
		protected void onPreExecute() {
			Log.i("AsyncSocketConnect",
					"AsyncSocketConnet PreExecute is started");
		}

		@Override
		protected Void doInBackground(Void... params) {

			try {
				Log.i("AsyncTank", "doInBackgoung: Creating Socket");
				socket = new Socket(serverip, serverport);
			} catch (Exception e) {
				Log.i("AsyncTank", "doInBackgoung: Cannot create Socket");
			}
			if (socket.isConnected()) {
				try {

					socketInputStream = new DataInputStream(socket.getInputStream());
					socketOutputStream = new DataOutputStream(socket.getOutputStream());
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
		
		public void SendDataToNetwork(String cmd) { //You run this from the main thread.
            try {
                if (socket.isConnected()) {
                    Log.i("AsyncTask", "SendDataToNetwork: Writing received message to socket");
                    socketOutputStream.write(cmd.getBytes());
                } else {
                    Log.i("AsyncTask", "SendDataToNetwork: Cannot send message. Socket is closed");
                }
            } catch (Exception e) {
                Log.i("AsyncTask", "SendDataToNetwork: Message send failed. Caught an exception");
            }
        }
		
		
	}
}