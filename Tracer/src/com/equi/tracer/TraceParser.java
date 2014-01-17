package com.equi.tracer;

import java.util.HashSet;
import java.util.Set;

public class TraceParser implements Parser {
	enum State {
		HEADER, LENGTH, ENTRY, TRAILER, READY
	}

	public TraceParser() {
		state = State.HEADER;
		result = new HashSet<TraceEntry>();
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
		case HEADER:
			r = headerParser.accept(c);
			switch (r) {
			case READY:
				state = State.LENGTH;
				return Result.BUSY;
			case BUSY:
				return Result.BUSY;
			case ERROR:
				return Result.ERROR;
			}
			break;
		case LENGTH:
			r = lengthParser.accept(c);
			switch (r) {
			case READY:
				nrEntries = ((Integer) lengthParser.getToken()).intValue();
				if (nrEntries > 0)
					state = State.ENTRY;
				else
					state = State.TRAILER;
				return Result.BUSY;
			case BUSY:
				return Result.BUSY;
			case ERROR:
				return Result.ERROR;
			}
			break;
		case ENTRY:
			r = entryParser.accept(c);
			switch (r) {
			case READY:
				result.add((TraceEntry) entryParser.getToken());
				cursor++;
				if (cursor == nrEntries) {
					state = State.TRAILER;
				}
				entryParser = new TraceEntryParser();
				return Result.BUSY;
			case BUSY:
				return Result.BUSY;
			case ERROR:
				return Result.ERROR;
			}
			break;
		case TRAILER:
			r = trailerParser.accept(c);
			switch (r) {
			case READY:
				state = State.READY;
				return Result.READY;
			case BUSY:
				return Result.BUSY;
			case ERROR:
				return Result.ERROR;
			}
			break;
		case READY:
			return Result.ERROR;
		}
		return Result.ERROR;
	}

	Set<TraceEntry> result;
	State state;

	StringParser headerParser = new StringParser("walb");
	StringParser trailerParser = new StringParser("blaw");
	IntParser lengthParser = new IntParser();
	TraceEntryParser entryParser = new TraceEntryParser();

	int nrEntries;
	int cursor;
	boolean resultAvailable;
}
