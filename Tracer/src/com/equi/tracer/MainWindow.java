package com.equi.tracer;

import info.monitorenter.gui.chart.ITrace2D;
import info.monitorenter.gui.chart.ZoomableChart;
import info.monitorenter.gui.chart.traces.Trace2DLtd;

import java.awt.BorderLayout;
import java.awt.EventQueue;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;
import java.util.Timer;
import java.util.TimerTask;

import javax.swing.JFrame;
import javax.swing.JPanel;

import com.equi.tracer.TraceServer.IResultHandler;

public class MainWindow implements IResultHandler {

	private JFrame frmWilbertsStreamTracer;
	private ZoomableChart panel; 
	private Map<Integer, ITrace2D> traces = new HashMap<Integer, ITrace2D>();
	private int traceCounter;
	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					
					MainWindow window = new MainWindow();
					TraceServer server = new TraceServer(window);
					Thread t = new Thread(server);
					t.start();
					window.frmWilbertsStreamTracer.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the application.
	 */
	public MainWindow() {
		initialize();
	}

	/**
	 * Initialize the contents of the frame.
	 */
	private void initialize() {
		frmWilbertsStreamTracer = new JFrame();
		frmWilbertsStreamTracer.setTitle("Wilbert's stream tracer");
		frmWilbertsStreamTracer.setBounds(100, 100, 450, 300);
		frmWilbertsStreamTracer.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frmWilbertsStreamTracer.getContentPane().setLayout(new BorderLayout(0, 0));
		
		panel = new ZoomableChart();
		frmWilbertsStreamTracer.getContentPane().add(panel);
	}

	@Override
	public void traceReceived(Set<TraceEntry> traced) {
		for (TraceEntry e: traced) {
			Integer id = new Integer(e.getTraceID());
			double val = e.getTracedValue();
			
			if (!traces.containsKey(id)) {
				ITrace2D tr = new Trace2DLtd(1000);
				traces.put(id, tr);
				panel.addTrace(tr);
			}
			
			ITrace2D tr = traces.get(id);
			tr.addPoint(traceCounter, val);
		}
		traceCounter++;
	}
}
