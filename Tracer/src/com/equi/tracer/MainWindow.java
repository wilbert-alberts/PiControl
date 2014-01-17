package com.equi.tracer;

import info.monitorenter.gui.chart.ITrace2D;
import info.monitorenter.gui.chart.ZoomableChart;
import info.monitorenter.gui.chart.traces.Trace2DLtd;
import info.monitorenter.gui.util.ColorIterator;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.EventQueue;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Properties;
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
	private DividingColorIterator colors = new DividingColorIterator();
	
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
		updateParnames();
		installNameUpdater();
	}

	private void installNameUpdater() {
		TimerTask task = new TimerTask() {
			
			@Override
			public void run() {
				updateParnames();
			}
		};
		
		Timer t = new Timer();
		t.schedule(task, 1000, 3000);
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
		Set<Integer> currentTraces = new HashSet<Integer>();
		currentTraces.addAll(traces.keySet());
		for (TraceEntry e: traced) {
			Integer id = new Integer(e.getTraceID());
			currentTraces.remove(id);
			double val = e.getTracedValue();
			
			if (!traces.containsKey(id)) {
				ITrace2D tr = new Trace2DLtd(1000);
				tr.setColor(colors.next());
				traces.put(id, tr);
				panel.addTrace(tr);
			}
			
			ITrace2D tr = traces.get(id);
			tr.addPoint(traceCounter, val);
		}
		for (Integer t: currentTraces) {
			panel.removeTrace(traces.get(t));
			traces.remove(t);
		}
		traceCounter++;
	}
	
	public void updateParnames() {
		Properties pr = new Properties();
		try {
			FileInputStream prfile = new FileInputStream("parnames.ini");
		
			pr.load(prfile);
			for (String id: pr.stringPropertyNames()) {
				String name = pr.getProperty(id);
				
				Integer idval = new Integer(id);
				if (traces.containsKey(idval)) {
					traces.get(idval).setName(name);
				}
			}
		}
		catch (FileNotFoundException ex) {
			//System.err.println("Warning: no parnames.ini found; unable to display traced parameter names.");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
