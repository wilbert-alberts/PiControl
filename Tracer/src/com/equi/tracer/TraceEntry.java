package com.equi.tracer;

public class TraceEntry {
	public TraceEntry(int id, double v) {
		this.id = id;
		this.val = v;
	}
	public int getTraceID() {
		return id;
	}
	
	public double getTracedValue() {
		return val;
	}
	
	public String toString() {
		return "(" + id + ": " + val + ")"; 
	}
	
	int id;
	double val;
	
}
