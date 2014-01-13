package com.equi.tracer;

public class StringParser implements Parser{
	enum State {
		BUSY,
		READY
	}
	public StringParser(String s) {
		token = s;
		state = State.BUSY;
		cursor = 0;
	}
	
	public String getToken() {
		if (state != State.READY)
			throw new IllegalStateException();
		return token;
    }
	
	public Result accept(char c) {
		switch (state) {
		case BUSY:
			if (c == token.charAt(cursor)) {
				cursor++;
				if (cursor == token.length()) {
					state = State.READY;
					return Result.READY;
				}
				return Result.BUSY;
			}
			else {
				System.err.println("StringParser encountered unexpected character");
				return Result.ERROR;				
			}
		case READY:
			throw new IllegalStateException("StringParser.accept called while READY.");
		default:
			throw new IllegalStateException("StringParser.accept called with unexpected State" + state);				
		}
	}
	
	String token;
	int    cursor;
	State  state;
}
