package com.equi.tracer;

public interface Parser {
	enum Result {
		ERROR,
		BUSY,
		READY
	}
	public Object getToken();
	public Result accept(char c);
}
