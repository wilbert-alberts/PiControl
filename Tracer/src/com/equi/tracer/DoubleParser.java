package com.equi.tracer;

public class DoubleParser implements Parser{
	enum State {
		BUSY,
		READY
	}
	public DoubleParser() {
		state = State.BUSY;
		cursor = 0;
		lValue = 0;
		factor = 1;
	}
	
	public Object getToken() {
		if (state != State.READY)
			throw new IllegalStateException();
		return result;
    }
	
	public Result accept(char c) {
		switch (state) {
		case BUSY:
			lValue += factor*c;
			factor *= 256;
			cursor++;
			if (cursor == 8) {
				result = new Double(Double.longBitsToDouble(lValue));
				state = State.READY;
				return Result.READY;
			}
			return Result.BUSY;
		case READY:
			throw new IllegalStateException("DoubleParser.accept invoked while READY");
		default:
			throw new IllegalStateException("DoubleParser.accept invoked with illegal state: " + state);
		}
	}
	
	Double result;
	State  state;
	int    cursor;
	long   lValue;
	long   factor;
}
