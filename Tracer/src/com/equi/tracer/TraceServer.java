package com.equi.tracer;

import java.io.IOException;
import java.io.InputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Set;

public class TraceServer implements Runnable {
	interface IResultHandler {
		public void traceReceived(Set<TraceEntry> traces);
	}

	private IResultHandler handler;
	
	TraceServer(IResultHandler resulthandler) {
		this.handler = resulthandler;
	}
	public void run() {
		ServerSocket socket;
		Socket lsocket;
		try {
			socket = new ServerSocket(9786);
			
			while (true) {
				lsocket = socket.accept();
				
				InputStream instream = lsocket.getInputStream();
				TraceParser tp = new TraceParser();
				Parser.Result r;
				Set<TraceEntry> result;
				
				int c;
				do {
					c = instream.read();
					//System.out.println("Read: " + c + " - " + (char)(c));
					r = tp.accept((char)c);
					switch (r) {
					case BUSY:
						break;
					case ERROR:
						System.err.println("Encountered error while parsing");
						c = -1;
						break;
					case READY:
						result = ((Set<TraceEntry>) tp.getToken());
						handleResult(result);
						tp = new TraceParser();
						break;
					default:	
						throw new RuntimeException();
							
					}
				} while (c>=0);
				
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
		
	}

	private void handleResult(Set<TraceEntry> result) {
		if (handler != null) {
			handler.traceReceived(result);
		}
	}

}
