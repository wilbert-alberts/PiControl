package com.equi.tracer;


public class IntParser implements Parser{
	enum State {
		BUSY,
		READY
	}
	
	public IntParser() {
		state = State.BUSY;
		factor = 1;
	}
	
	public Object getToken() {
		return new Integer(result);
	}
	
	public Result accept(char c) {
		switch (state) {
		case BUSY:
			cursor++;
			result += factor*c;
			factor *= 256;
			if (cursor == 4) {				
				state = State.READY;
				return Result.READY;
			}
			return Result.BUSY;
		case READY:
			throw new IllegalStateException("IntParser.accept invoked while READY.");
		default:
			throw new IllegalStateException("IntParser.accept invoked with illegal state" + state);		
		}
	}
	
	State   state;
	int     result;
	int     cursor;
	int     factor;
}
