package com.equi.tracer;

import java.awt.Color;

public class DividingColorIterator {
	public DividingColorIterator() {
		nominator = 1;
		denominator = 2;
	}
	
	public Color next() {
		float h = nominator / denominator;
		Color result = Color.getHSBColor(h, 1.0f, 0.8f);
		nominator += 2;
		if (nominator > denominator) {
			nominator = 1;
			denominator *=2;
		}
		
		return result;
	}
	
	private float nominator;
	private float denominator;
}
