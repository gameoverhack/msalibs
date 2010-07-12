/***********************************************************************
 
 Copyright (c) 2008, 2009, 2010 Memo Akten, www.memo.tv
 *** The Mega Super Awesome Visuals Company ***
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of MSA Visuals nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ***********************************************************************/

/***************
 DEPENDENCIES:
 - none
 ***************/ 


#pragma once

namespace MSA {
	
	class Color {
	public:
		
		float r, g, b, a;
		
		Color() {
			set(1, 1, 1, 1);
		}
		
		Color(float r, float g, float b, float a = 1) {
			set(r, g, b, a);
		}
		
		
		Color( const Color & col){
			set(col.r, col.g, col.b, col.a);
		}
		
		
		
		void set(float r, float g, float b, float a = 1) {
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}
		
		void setGL() {
			glColor4f(r, g, b, a);
		}
		
		void setClamp(float r, float g, float b, float a = 1) {
			set(r, g, b, a);
			clamp();
		}
		
		void clamp() {
			if(r > 1) r = 1;
			if(g > 1) g = 1;
			if(b > 1) b = 1;
			if(a > 1) a = 1;
		}
		
		Color lerpTo(const Color& target, float t ) {
			return (*this * t) + (target * (1-t));
		}
		
		
		float getLuminance() {
			return (0.2126*r) + (0.7152*g) + (0.0722*b);
		}
		
		
		// H [0..360], S and V [0..1]
		void setHSV(float h, float s, float v, float a = 1) {
			h = int(h) % 360;
			s = min(s, 1.0f);
			v = min(v, 1.0f);
			
			int i = (int)floor(h/60.0f) % 6;
			float f = h/60.0f - floor(h/60.0f);
			float p = v * (1 - s);
			float q = v * (1 - s * f);
			float t = v * (1 - (1 - f) * s);
			
			switch (i) {
				case 0: set(v, t, p, a); break;
				case 1: set(q, v, p, a); break;
				case 2: set(p, v, t, a); break;
				case 3: set(p, q, v, a); break;
				case 4: set(t, p, v, a); break;
				case 5: set(v, p, q, a); break;
			}
		}
		
		// assumes RGB is normalized [0..1]
		// returns H [0..360], S and V [0..1]
		void getHSV(Color& outHSV) {
			float h, s, v;
			float y, r1,g1,b1;
			
			v = r;
			if (v<g) v=g;
			if (v<b) v=b;
			y = r;
			if (y>g) y=g;
			if (y>b) y=b;
			if (v != 0) s = (v-y)/v;
			else s = 0;
			if (s == 0) {
				h = 0;
				s = 0;
				v = (int)(v*100);
			}
			r1 = (v-r)/(v-y);
			g1 = (v-g)/(v-y);
			b1 = (v-b)/(v-y);
			if (r == v){
				if (g == y) h = 5.+b1;
				else h = 1.-g1;
			}
			else if (g == v){
				if (b == y) h = r1+1.;
				else h = 3.-b1;
			}
			else{
				if (r == y) h = 3.+g1;
				else h = 5.-r1;
			}
			// convert it all
			h = h * 60.;
			if (h >= 360.) h = h-360.;
			outHSV.set(h, s, v, a);
		}
		
		
		//equalitg
		bool operator==( const Color& col ) {
			return (r == col.r) && (g == col.g) && (b == col.b) && (a == col.a);
		}
		
		//inequalitg
		bool operator!=( const Color& col ) {
			return (r != col.r) || (g != col.g) || (b != col.b) || (a != col.a);
		}
		
		//Set
		Color & operator=( const Color& col ){
			set(col.r, col.g, col.b, col.a);
			return *this;
		}
		
		// Add
		Color operator+( const Color& col ) const {
			return Color( r+col.r, g+col.g, b+col.b, a+col.a );
		}
		
		
		Color & operator+=( const Color& col ) {
			r += col.r;
			g += col.g;
			b += col.b;
			a += col.a;
			return *this;
		}
		
		Color & operator+=( const float & val ) {
			r += val;
			g += val;
			b += val;
			a += val;
			return *this;
		}
		
		// Subtract
		Color operator-(const Color& col) const {
			return Color( r-col.r, g-col.g, b-col.b, a-col.a);
		}
		
		Color & operator-=( const Color& col ) {
			r -= col.r;
			g -= col.g;
			b -= col.b;
			a -= col.a;
			return *this;
		}
		
		Color & operator-=( const float & val ) {
			r -= val;
			g -= val;
			b -= val;
			a -= val;
			return *this;
		}
		
		// Multiply
		Color operator*(const float& val) const {
			return Color( r*val, g*val, b*val, a*val);
		}
		
		Color & operator*=( const Color& col ) {
			r*=col.r;
			g*=col.g;
			b*=col.b;
			a*=col.a;
			return *this;
		}
		
		Color & operator*=( const float & val ) {
			r*=val;
			g*=val;
			b*=val;
			a*=val;
			return *this;
		}
		
		
		// Divide
		Color operator/( const Color& col ) const {
			return Color( col.r!=0 ? r/col.r : r , col.g!=0 ? g/col.g : g, col.b!=0 ? b/col.b : b, col.a!=0 ? a/col.a : a);
		}
		
		Color operator/( const float &val ) const {
			if( val != 0){
				return Color( r/val, g/val, b/val, a/val );
			}
			return Color(r, g, b, a);
		}
		
		Color& operator/=( const Color& col ) {
			col.r!=0 ? r/=col.r : r;
			col.g!=0 ? g/=col.g : g;
			col.b!=0 ? b/=col.b : b;
			col.a!=0 ? a/=col.a : a;
			
			return *this;
		}
		
		Color& operator/=( const float &val ) {
			if( val != 0 ){
				r /= val;
				g /= val;
				b /= val;
				a /= val;
			}
			
			return *this;
		}
	};
}
