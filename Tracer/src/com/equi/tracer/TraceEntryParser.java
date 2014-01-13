package com.equi.tracer;

public class TraceEntryParser implements Parser {
	enum State {
		ID,
		VALUE,
		READY
	}
	
	public TraceEntryParser() {
		state = State.ID;
	}
	
	@Override
	public Object getToken() {
		if (state != State.READY)
			throw new IllegalStateException();
		return result;
	}

	@Override
	public Result accept(char c) {
		Result r;
		switch (state) {
		case ID:
			r = intParser.accept(c);
			switch (r) {
			case READY:
				id = ((Integer)intParser.getToken()).intValue(); 
				state = State.VALUE;
				return Result.BUSY;
			case BUSY:
				return Result.BUSY;
			default:
				return Result.ERROR;
			}
		case VALUE:
			r = dblParser.accept(c);
			switch (r) {
			case READY:
				value = ((Double)dblParser.getToken()).doubleValue();
				result = new TraceEntry(id, value);
				state = State.READY;
				return Result.READY;
			case ERROR:
				return Result.ERROR;
			case BUSY:
				return Result.BUSY;
			}
		default:
			return Result.ERROR;
		}
	}

	State state;
	IntParser intParser = new IntParser();
	DoubleParser dblParser = new DoubleParser();
	TraceEntry result;
	int id;
	double value;
	
}
